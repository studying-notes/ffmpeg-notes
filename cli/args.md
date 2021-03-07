---
date: 2020-09-15T22:20:49+08:00  # 创建日期
author: "Rustle Karl"  # 作者

# 文章
title: "FFmpeg 命令参数列表"  # 文章标题
url:  "posts/ffmpeg/cli/args"  # 设置网页链接，默认使用文件名
tags: [ "ffmpeg" ]  # 自定义标签
series: [ "FFmpeg 从入门到放弃"]  # 文章主题/文章系列
categories: [ "学习笔记"]  # 文章分类

# 章节
weight: 20 # 文章在章节中的排序优先级，正序排序
chapter: false  # 将页面设置为章节

index: true  # 文章是否可以被索引
draft: false  # 草稿
---

## 显示功能/信息

| 参数 | 作用 |
| ---- | ---- |
| -formats | 显示可用格式 |
| -muxers | 显示可用的 muxers |
| -demuxers | 显示可用的多路分解器 |
| -devices | 显示可用设备 |
| -codecs | 显示可用的编解码器 |
| -decoders | 显示可用的解码器 |
| -encoders | 显示可用的编码器 |
| -bsfs | 显示可用的位流过滤器 |
| -protocols | 显示可用协议 |
| -filters | 显示可用的过滤器 |
| -pix_fmts | 显示可用的像素格式 |
| -layouts | 显示标准的频道布局 |
| -sample_fmts | 显示可用的音频样本格式 |
| -colors | 显示可用的颜色名称 |
| -sources device | 列出输入设备的来源 |
| -sinks device | 列出输出设备的接收器 |
| -hwaccels | 显示可用的硬件加速方法 |

### 列出支持的滤镜

```shell
ffmpeg -filters | grep draw
```

### 日志记录级别

`-loglevel [repeat+]loglevel | -v [repeat+]loglevel`

设置库的日志记录级别。加入 `repeat+` 表示重复日志输出不应该被压缩到所述第一条日志和最后的日志重复 N 次将被省略。如果 `repeat` 单独使用，但没有预设的记录级别，将使用默认记录级。如果给定多个日志级别参数，使用 `repeat` 不会改变日志级别。 `loglevel` 是一个字符串或数字，可为以下值之一：

- `quiet, -8` 保持沉默
- `panic, 0` 只显示可能导致程序崩溃的致命错误
- `fatal, 8` 只显示致命错误，这些错误会导致进程绝对无法继续
- `error, 16` 显示所有的错误，包括那些可以修复的
- `warning, 24` 显示所有警告和错误，将显示任何有关可能不正确或不正常事件的信息
- `info, 32` 显示处理过程中的信息。不单单是警告和错误，默认值
- `verbose, 40` 与 `info` 类似 ，但更详细
- `debug, 48` 显示一切信息，包括调试信息

## 全局选项

| 参数 | 作用 |
| ---- | ---- |
| -y   | 覆盖输出文件 |
| -n | 从不覆盖输出文件 |
| -hide_banner | 隐藏标语及细节，只显示媒体文件信息 |
| -max_alloc bytes | 设置单个分配块的最大大小 |
| -ignore_unknown | 忽略未知的流类型 |
| -filter_threads | 非复杂过滤器线程的数量 |
| -lavfi, -filter_complex  | 复杂滤镜 |
| -filter_complex_threads | -filter_complex 的线程数 |
| -stats | 在编码过程中打印进度报告 |
| -max_error_rate maximum error rate | 最大错误率的错误率 `0.0` 无错误 `1.0` 100％ 错误），高于此比率将返回错误而不是成功 |
| -bits_per_raw_sample number | 设置每个原始样本的位数 |
| -vol volume | 更改音频音量（256 =正常） |

## 文件选项

| 参数 | 作用 |
| ---- | ---- |
| -f fmt | 指定音频/视频格式 |
| -i filename | 指定输入文件名 |
| -t duration | 记录或转码音频/视频的时长（秒） |
| -ss time_off | 设置开始时间偏移（秒），支持 `hh:mm:ss[.xxx]` 格式 |
| -fs limit_size | 设置文件大小的上限（字节） |
| -map | 指定输出文件的流映射关系 |
| -c codec | 手动指定输出文件的编码 |
| -codec codec | 编解码器名称 |
| -pre preset | 预设名称 |
| -map_metadata outfile[,metadata]:infile[,metadata] | 设置元数据信息 |
| -to time_stop | 记录或转码停止时间 |
| -sseof time_off | 设置相对于 EOF 的开始时间偏移 |
| -seek_timestamp | 使用 `-ss` 按时间戳启用/禁用查找 |
| -timestamp time | 设置录制时间戳 |
| -metadata string=string | 添加元数据 |
| -program title=string:st=number... | 添加具有指定流的程序 |
| -target type | 指定目标文件类型 |
| -apad | 音频垫 |
| -frames number | 设置要输出的帧数 |
| -filter filter_graph | 设置流 filtergraph |
| -filter_script filename | 从文件中读取流 filtergraph 描述 |
| -reinit_filter | 输入参数更改时重新初始化 filtergraph |
| -discard | 丢弃 |
| -disposition | 处置 |

- `-map 1:0 -map 1:1` 将第二个输入文件的第一个流和第二个流写入输出文件；
- 其中 `-c:v` 用于指定视频编码，`-c:a` 指定音频编码；视频文件的后缀如 mp4、mkv、avi 等只是表示用来装载媒体流的“容器”类型，而编码时使用的编码方式则另需指定；
- `-c:v copy` 表示复制输入文件中的视频流到输出文件，不重新进行编码；
- `-c:av copy` 表示输入文件中的视频流和音频流同时复制到输出文件，只改变文件容器。

## 视频选项

| 参数 | 作用 |
| ---- | ---- |
| -vn | 取消视频的输出 |
| -bitexact | 使用标准比特率 |
| -b bitrate, -vb | 视频比特率 -b:v |
| -r rate | 设置帧速率（Hz值，分数或缩写） |
| -s size | 设置帧大小（WxH 或缩写） |
| -aspect aspect | 宽高比设置的宽高比（4：3、16：9或1.3333、1.7777） |
| -vframes number | 设置要输出的视频帧数 |
| -bits_per_raw_sample number | 设置每个原始样本的位数 |
| -vcodec codec | 强制视频编解码器 |
| -timecode hh:mm:ss[:;.]ff | 设置初始 TimeCode 值 |
| -pass n | 选择通行证编号 1-3 |
| -vf filter_graph | 设置视频过滤器 |
| -dn | 禁用数据 |

- -croptop size：设置顶部切除尺寸
- -cropbottom size：设置底部切除尺寸
- -cropleft size：设置左切除尺寸
- -cropright size：设置右切除尺寸
- -padtop size：设置顶部补齐尺寸
- -padbottom size：底补齐
- -padleft size：左补齐
- -padright size：右补齐
- -padcolor color：颜色补齐

## 音频选项

| 参数 | 作用 |
| ---- | ---- |
| -an | 取消音频的输出 |
| -ab bitrate | 音频比特率 -b:a |
| -aframes number | 设置要输出的音频帧数 |
| -aq quality | 设置音频质量（特定于编解码器） |
| -ar rate | 设置音频采样率（以 Hz 为单位） |
| -ac channels | 设置音频频道数 |
| -acodec codec | 强制音频编解码器 |
| -vol volume | 更改音频音量，默认 256 <百分比> |
| -af filter_graph | 设置音频过滤器 |

## 字幕选项

| 参数 | 作用 |
| ---- | ---- |
| -s size | 设置帧大小（WxH或缩写） |
| -sn | 禁用字幕 |
| -scodec codec | 强制字幕编解码器 |
| -stag fourcc/tag | 强制字幕 tag/fourcc |
| -fix_sub_duration | 修复字幕的持续时间 |
| -canvas_size size | 设置画布大小（WxH 或缩写） |
| -spre preset | 将字幕选项设置为指示的预设 |
