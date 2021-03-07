---
date: 2020-09-15T22:20:49+08:00  # 创建日期
author: "Rustle Karl"  # 作者

# 文章
title: "FFmpeg Fliter 详解"  # 文章标题
url:  "posts/ffmpeg/cli/filter"  # 设置网页链接，默认使用文件名
tags: [ "ffmpeg", "filter" ]  # 自定义标签
series: [ "FFmpeg 从入门到放弃"]  # 文章主题/文章系列
categories: [ "学习笔记"]  # 文章分类

# 章节
weight: 20 # 文章在章节中的排序优先级，正序排序
chapter: false  # 将页面设置为章节

index: true  # 文章是否可以被索引
draft: false  # 草稿
---

> FFmpeg 的 Filters 功能由 libavfilter 库提供。

- [视频 Filter 详解](video.md)
- [音频 Fliter 详解](audio.md)

## -vf vs -filter_complex

两种的转换

```shell
ffmpeg -y -v error -i G:/hls/test3.mp4 -i G:/hls/logo.png -filter_complex overlay=x=10:y=10 -vcodec libx264 -acodec copy -filter:v drawtext="text='hello':x=50+n*2:y=50" G:/hls/output/test3.mp4
```


```shell
ffmpeg -y -v error -i G:/hls/test3.mp4 -i G:/hls/logo.png -filter_complex overlay=x=10:y=10,drawtext=fontfile=/Windows/Fonts/DejaVuSans.ttf:fontcolor=white:fontsize=38:text='hello':x=50+n*2:y=50" -vcodec libx264 -acodec copy G:/hls/output/test3.mp4
```


```shell
ffmpeg -i video.mp4 -filter_complex \
"[0:a]aformat=channel_layouts=mono,showwavespic=s=600x200,colorkey=black,
        colorchannelmixer=rr=156/255:gg=244/255:bb=47/255[fg]; \
color=s=600x200:color=#44582c,drawgrid=width=iw/10:height=ih/5:color=#9cf42f@0.1[bg]; \
[bg][fg]overlay=format=rgb" -frames:v 1 output.png

ffmpeg -i output.png -vf  "crop=600:40:0:80" output-cropped.png
```

```shell
ffmpeg -i video.mp4 -filter_complex \
"[0:a]aformat=channel_layouts=mono,showwavespic=s=600x200,colorkey=black, \
      colorchannelmixer=rr=156/255:gg=244/255:bb=47/255[fg]; \
      color=s=600x200:color=#44582c,drawgrid=width=iw/10:height=ih/5:color=#9cf42f@0.1[bg]; \
 [bg][fg]overlay=format=rgb,crop=600:40:0:80" -frames:v 1 output-cropped.png
```


```shell
env DISPLAY=:1 ffmpeg -y -hide_banner -i media/0.mp4 -i media/1.mp4 -i media/2.mp4 -filter_complex "[0:v]split[v00split1][v00split2];[1:v]split[v01split1][v01split2];[2:v]split[v02split1][v02split2];[v00split1]trim=0.00:3.00[v00trim1];[v00split2]trim=3.00:4.00[v00trim2];[v00trim2]setpts=PTS-STARTPTS[v00trim2setpts];[v01split1]trim=0.00:3.00[v01trim1];[v01split2]trim=3.00:4.00[v01trim2];[v01trim2]setpts=PTS-STARTPTS[v01trim2setpts];[v02split1]trim=0.00:3.00[v02trim1];[v02split2]trim=3.00:4.00[v02trim2];[v02trim2]setpts=PTS-STARTPTS[v02trim2setpts];[v00trim2setpts][v01trim1]gltransition=duration=1.00:source=transitions/burn.glsl[gl0];[v01trim2setpts][v02trim1]gltransition=duration=1.00:source=transitions/doorway.glsl[gl1];[v00trim1][gl0][gl1][v02trim2setpts]concat=n=4[gl_output];[gl_output]drawtext=fontsize=120:fontfile=media/font.ttf:text='Logo':fontcolor=black:x=0:y=0[dt_output]" -map "[dt_output]" -vcodec libx264 -q:a 0 -q:v 0 -pixel_format yuv420p -profile:v baseline -preset slow -movflags faststart out.mp4
```


## Filter 描述格式

```shell
[输入流或标记名] filter 参数[临时标记名];[输入流或标记名] filter 参数[临时标记名]
```

## Filter 简介

一个 Filter 可以有多个输入和输出，以一个 filtergraph 为例：

```
                [main]
input --> split ---------------------> overlay --> output
            |                             ^
            |[tmp]                  [flip]|
            +-----> crop --> vflip -------+
```

这个 filtergraph 将输入流复制成相同的两个流，然后将一个流送入 crop filter 和 vflip filter，最后通过通过 overlay filter 覆盖在另一个流之上。这个过程可以用命令表达：

```shell
ffmpeg -i INPUT -vf "split[main][tmp];[tmp]crop=iw:ih/2:0:0,vflip[flip];[main][flip]overlay=0:H/2" OUTPUT

# 试验
ffmpeg -i video.mp4 -vf "split[main][tmp];[tmp]crop=iw:ih/2:0:0,vflip[flip];[main][flip]overlay=0:H/2" video_filter.mp4
```



输出结果是一个以上半部分为镜像的对称视频。

Filters 对于在同一个线性链处理中用 `,` 分割，而在不同线性链处理中用 `;` 分割。

## Filtergraph 描述

Filtergraph 是 Filters 彼此连接的有向图。它可以包含循环，一对 Filters 之间也可以有多个连接。每一个连接有一个输入端接收输出，一个输出端发送输出。

一个没有输入端的 Filter 被称为 `source`，没有输出端的被称为 `sink`。

## Filtergraph 语法

Filtergraph 用文本表示，其由一些 ffmpeg 和 ffplay 通用的选项 `-filter/-vf/-af` 和 `-filter_complex`（ffmpeg）以及 `-vf/-af`（ffplay）外加定义与 `libavfilter/avfilter.h` 的 `avfilter_graph_parse_ptr()` 等来描述。

- 一个 filterchain 包含序列链接起来的 filter，这个序列由 `,` 分隔各个 filter 描述
- 一个 Filtergraph 包含序列 filterchain，这个序列有 `;` 分隔各个 filterchain 描述
- 一个 filter 由一个字符串表单表示：`[in_link_1]...[in_link_N]filter_name=arguments[out_link_1]...[out_link_M]`

这里 `filter_name` 是 filter 类名字，用于指定 filter 实例。其后的 `=arguments` 用于指定 filter 选项。`arguments` 是用于初始化 filter 的参数，它可能是下面两类表单中的一个：

- 一个 `:` 分隔的 `key=value` 列表
- 一个 `:` 分隔的列表 `value` 值，在这种情况下，键（key）假定为选项名声明顺序，如 `fade`  filter 按顺序声明了3个选项 `type`、`start_frame` 和 `nb_frames`，则参数 `in:0:30` 意味着 `type=in:start_frame=0:nb_frames=30`。
- 前面二者的混合。这种情况下，键值对必须在前，然后接遵循相同约束的若干值。在键值对中可以按任意顺序设置优先顺序（后接值按最后一个键值对顺序延续设置）。

如果选项的值本身就是一个列表（例如 `format`  filter 有一个像素格式列表选项），则这种列表通常用 “|” 分隔。

列表参数可以被 `'`（单引号）包含起来。字符 `\` 作为转义字符用于引号包含的文本。否则参数字符串在遇到特殊字符（例如 ’[]=;,’）处被看作终止。

在 filter 名和参数前和后有一个连接标签列表。一个连接标签允许命名1个名字的连接，其作为 filter 的输入或者输出端口。以下预订标签 `in_link_1 ... in_link_N` 作为 filter 的输入端，`out_link_1 ... out_link_M` 作为 filter 的输出端。

当中一个 Filtergraph 中找到相同名字的连接标签时，一个在相应输入端和输出端之间的连接被创建（即认为它们是连接在一起的，如果用做一个 filter 的输出，又用着一个 filter 的输入，则表示从前一个 filter 输出到后一个 filter）

如果一个输出端没有命名标签，它默认连接到 filterchain 上后面 filter 中第一个没有命名标签的输入端。例如：

```
nullsrc,split[L1],[L2]overlay,nullsink
```

这里 `split` 有两路输出，`overlay` 有两路输入，`split` 的第一路输出被命名为标签 ”L1”，`overlay` 的第一路输入被命名为标签 ”L2”。则 `split` 的第二路输出链接到 `overlay` 的第二路输入（它们都没有用标签命名）。

在一个 filter 描述中，如果第一个 filter 的输入没有指定，则假定为 ”in”，如果最后一个 filter 输出没有指定，则假定为 ”out”。

在一个完整的 filterchain 上，所有未标签命名的输入和输出必须被连接匹配。 Filtergraph 中如果所有 filter 的输入和输出都被连接则被认为是有效的。

如果格式要求，则 `libavfilter` 会自动插入 `scale`  filter 。对于 Filtergraph 描述，可以通过 `sws_flags=flags` 来指定 `swscale` 标志实现自动插入放缩。

这里有一个 `BNF` 描述的 Filtergraph 语法：

```
NAME ::= sequence of alphanumeric characters and '_'LINKLABEL::= "[" NAME "]"LINKLABELS   ::= LINKLABEL [LINKLABELS]FILTER_ARGUMENTS ::= sequence of chars (possibly quoted)FILTER   ::= [LINKLABELS] NAME ["=" FILTER_ARGUMENTS] [LINKLABELS]FILTERCHAIN  ::= FILTER [,FILTERCHAIN]FILTERGRAPH  ::= [sws_flags=flags;] FILTERCHAIN [;FILTERGRAPH]
```

### Filtergraph 中的转义

- 第一层的转义在每个 filter 选项值中，其可能包含特殊字符 ”:” 来分隔值，或者一个转义符 ”\”
- 第二层的转义在整个 filter 描述，其可能包含转义符”\”或者特殊字符”[],;” ，它们被用于 Filtergraph 的描述
- 第三层的转义，是处理 Shell 中需要转义的字符

## Filter 时间内置变量

![](../../imgs/filter.png)

部分 Filter 支持根据表达式结果判断是否启用的选项，在发送帧之前计算表达式，若非 0 值则启用，否则跳过。

例如，在 3s 到 20s 之间启用 smartblur filter，

```shell
smartblur=enable='between(t,10,3*60)'

curves=enable='gte(t,3)':preset=cross_process
```
