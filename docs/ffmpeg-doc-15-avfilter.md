---
date: 2021-03-03T14:09:27+08:00  # 创建日期
author: "Rustle Karl"  # 作者

# 文章
title: "Ffmpeg Doc 15 Avfilter"  # 文章标题
# description: "文章描述"
url:  "posts/ffmpeg/docs/ch15_avfilter"  # 设置网页永久链接
tags: [ "ffmpeg"]  # 标签
series: [ "FFmpeg 从入门到放弃"]  # 文章主题/文章系列
categories: [ "学习笔记"]  # 文章分类

# 章节
weight: 20 # 排序优先级
chapter: false  # 设置为章节

index: true  # 是否可以被索引
toc: true  # 是否自动生成目录
draft: false  # 草稿
---

## 滤镜入门

FFmpeg 通过 `libavfilter` 库实现滤镜功能。在 `libavfilter` 中，一个滤镜可以有多个输入和多个输出。为了尽可能介绍清楚，我们假定有下面的滤镜链图。

> 
                    [main]
	input --> split ---------------------> overlay --> output
            |                             ^
            |[tmp]                  [flip]|
            +-----> crop --> vflip -------+

在这个滤镜链图中，利用 `split` 滤镜把输入流分离成了两路流，其中一路通过 `crop` 滤镜和 `vfilp` 滤镜的同一路级联应用，再同另外一路一起通过 `overlay` 滤镜处理的流合成进行输出。则可以采用如下的命令行实现：
    
    ffmpeg  -i INPUT -vf "split [main][tmp]; [tmp] crop=iw:ih/2:0:0, vflip [flip]; [main][flip] overlay=0:H/2" OUTPUT
    
这样最终输出将是视频上部是原始，下部是上部的镜像。（倒影效果）

同一路的滤镜间用逗号(',')进行分割，不同路的滤镜间用分号进行分割。在这个例子里面 `crop` 和 `vflip` 是在同一路中的滤镜，`split` 和 `overlay` 则不是同一路的（同一路的级联是对连续的视频进行，如果涉及到一输多、多输一或者多输多则都不是在同一路的，即不是同一路级联）。可以通过在方括号中的标签名来命名处理的链路。这个例子里，`split` 滤镜生成了两路就通过 `[main]` 和 `[tmp]` 进行了标签命名以方便后续处理。

其中被 `split` 处理输出的第二路流被命名为 `[tmp]`，它又被 `crop` 滤镜处理裁去下半部视频，然后通过 `vflip` 进行了水平镜像（垂直翻转，即把视频镜像到下半部了）。这是整个输出被命名为 `flip`。再把 `[main]` 与 `flip` 通过 `overlay` 进行覆盖合成，即把源输入通过 `split` 滤镜获得的 `[main]` 的上半部分覆盖到由 `crop` 和 `vflip` 滤镜级联处理的输出（这里的 `[flip]` ）上最终得到了镜像结果。

一些滤镜支持参数列表： 滤镜名=由冒号(':')隔开的多个参数。

还存在所谓的源过滤器（即没有输入音频/视频 的过滤器），以及槽过滤器（即没有任何音频/视频 输出的过滤器）。

## 滤镜链图介绍

一个滤镜链图（filtergraph）是连接滤镜的有向图。它可以包含循环动作，也可以在多个滤镜间形成链路，每个链接都有一个连接到滤镜的输入和一个连接到滤镜的输出。

滤镜链图中的每个滤镜都是一个滤镜注册类应用程序的实例，它定义了滤镜的功能、输入接口和输出接口。

如果滤镜没有输入端（接口），则被称作“源”，如果滤镜没有输出端则被称作“槽”（这样的滤镜用于描述/测试等场景，而不用于实际处理）。

### 滤镜链图语法

滤镜链图采用文本表示，其有由一些ffmpeg和ffplay通用的选项 `-filter/-vf/-af` 和 `-filter_complex` （ ffmpeg ）以及 `-vf/-af` （ ffplay ）外加定义与 `libavfilter/avfilter.h` 的 `avfilter_graph_parse_ptr()` 等来描述。

一个滤镜链包含序列链接起来的滤镜，这个序列由 “,” 分隔各个滤镜描述。

一个滤镜链图包含序列滤镜链，这个序列有“;”分隔各个滤镜链描述。

一个滤镜由一个字符串表单表示：` [in_link_1]...[in_link_N]filter_name=arguments[out_link_1]...[out_link_M] `。

这里 `filter_name` 是滤镜类名字，用于指定滤镜实例（它注册于程序中）。其后的 `=arguments` 用于指定滤镜选项。`arguments` 是用于初始化滤镜的参数，它可能有下面两类表单中的一个：

- 一个 “:” 分隔的 `key = value` 列表
- 一个 “:” 分隔的列表 `value` 值，在这种情况下，键（ key ）假定为选项名声明顺序，如 `fade` 滤镜按顺序声明了 3 个选项 `type`、`start_frame` 和 `nb_frames`，则参数 `in:0:30` 意味着 `type` 为 `in`，`start_frame` 为 `0`，`nb_frames` 为 `30`。
- 前面二者的混合。这种情况下，键值对必须在前，然后接遵循相同约束的若干值。在键值对中可以按任意顺序设置优先顺序。（后接值按最后一个键值对顺序延续设置）。

如果选项的值本身就是一个列表（例如 `format` 滤镜有一个像素格式列表选项），则这种列表通常用“|”分隔。

列表参数可以被 `'` （单引号）包含起来。字符 ` \ ` 作为转义字符用于引号包含的文本。否则参数字符串在遇到特殊字符（例如'[]=;,'）处被看作终止。

在滤镜名和参数前 和 后 有一个连接标签列表。一个连接标签允许命名1个名字的连接，其作为滤镜的输入或者输出端口。以下预订标签`in_link_1 ... in_link_N`作为滤镜的输入端，`out_link_1 ... out_link_M`作为滤镜的输出端。

当中一个滤镜链图中找到相同名字的连接标签时，一个在相应输入端和输出端之间的连接被创建（即认为它们是连接在一起的，如果用做一个滤镜的输出，又用着一个滤镜的输入，则表示从前一个滤镜输出到后一个滤镜）。

如果一个输出端没有命名标签，它默认连接到滤镜链上后面滤镜中第一个没有命名标签的输入端。例如：

	nullsrc, split[L1], [L2]overlay, nullsink

这里 `split` 有两路输出，`overlay` 有两路输入，`split` 的第一路输出被命名为标签 "L1",`overlay` 的第一路输入被命名为标签 "L2"。则 `split` 的第二路输出链接到 `overlay` 的第二路输入（它们都没有用标签命名）。

在一个滤镜描述中，如果第一个滤镜的输入没有指定，则假定为 "in"，如果最后一个滤镜输出没有指定，则假定为 "out"。

在一个完整的滤镜链上，所有未标签命名的输入和输出必须被连接（匹配）。滤镜链图中如果所有滤镜的输入和输出都被连接则被认为是有效的。

如果格式要求，则 `Libavfilter` 会自动插入 `scale` 滤镜。对于滤镜链图描述，可以通过 `sws_flags=flags` 来指定 `swscale` 标志实现自动插入放缩。

这里有一个 `BNF` 描述的滤镜链图语法：

    NAME ::= sequence of alphanumeric characters and '_'
    LINKLABEL::= "[" NAME "]"
    LINKLABELS   ::= LINKLABEL [LINKLABELS]
    FILTER_ARGUMENTS ::= sequence of chars (possibly quoted)
    FILTER   ::= [LINKLABELS] NAME ["=" FILTER_ARGUMENTS] [LINKLABELS]
    FILTERCHAIN  ::= FILTER [,FILTERCHAIN]
    FILTERGRAPH  ::= [sws_flags=flags;] FILTERCHAIN [;FILTERGRAPH]

### 注意滤镜链图中的转义

滤镜链图成分需要包含多层的转义。

第一层的转义效果在每个滤镜选项值中，其可能包含特殊字符":"来分隔值，或者一个转义符"\"

第二层的转义在整个滤镜描述，其可能包含转义符"\"或者特殊字符"[],;" ，它们被用于滤镜链图的描述

实际上，当你在 shell 命令行上描述滤镜链图时还可能遇见第三层的转义（处理 shell 中需要转义的字符）

例如下面的字符需要嵌入 `drawtext` 滤镜描述的 `text` 值

    this is a 'string': may contain one, or more, special characters

这个字符串包含`'`字符需要被转义，还有`:`这里也需要被转义，方法是：

    text=this is a \'string\'\: may contain one, or more, special characters

当嵌入滤镜描述时发生第二层的转义，为了转义所有的滤镜链图特殊字符，需要按下例处理：

    drawtext=text=this is a \\\'string\\\'\\: may contain one\, or more\, special characters

**注意**对于\'转义中的每个特殊字符都需要再次转义，就成了\\\'

最终当在 shell 命令行中写这个滤镜链图时再次转义。例如需要对每个特殊字符和转义处理的 `\` 等进行转义，最终为：
    
    -vf "drawtext=text=this is a \\\\\\'string\\\\\\'\\\\: may contain one\\, or more\\, special characters"

## 时间线编辑

一些滤镜支持常规的 `enable` 选项。对于支持时间线编辑的滤镜，这个选项可以被设置为一个表达式，其通过评估之前的情况来决定是否把帧画面发送给滤镜。如果表达式计算结果为非 0 值，则表明滤镜被使用，否则滤镜将被跳过（即把帧画面直接送到滤镜链图的下一个滤镜中）

表达式中可以出现下面的值：

- t 时间戳，单位秒。如果输入时间戳未知则为 `NAN`
- n 输入帧的序数，从 0 开始计数
- pos 输入帧在文件中的偏移位置，如果未知则为 `NAN`
- w, h 视频输入帧的宽和高

此外，这些滤镜 `enable` 选项状态也可以用于表达式。

类似其他选项，这个 `enable` 选项有相同的规则。

例如，要在 10 秒到 3 分钟允许一个 blur （模糊）滤镜（ `smartblur` ），然后 `curves` 滤镜在 3 秒之后：

	smartblur= nable='between(t,10,3*60)',
	curves=enable='gte(t,3)':preset=cross_process
