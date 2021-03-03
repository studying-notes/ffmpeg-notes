---
date: 2021-03-03T13:01:42+08:00  # 创建日期
author: "Rustle Karl"  # 作者

# 文章
title: "Ffmpeg Doc 03 Capture"  # 文章标题
# description: "文章描述"
url:  "posts/ffmpeg/docs/ch03_capture"  # 设置网页永久链接
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

## 视频和音频抓取

如果你指定了输入格式和设备，ffmpeg 可以直接抓取视频和音频：

> 
    ffmpeg -f oss -i /dev/dsp -f video4linux2 -i /dev/video0 /tmp/out.mpg

或者采用 ALSA 音频源 ( 单声道，卡的 id 是 1) 替代 OSS :

> 
	ffmpeg -f alsa -ac 1 -i hw:1 -f video4linux2 -i /dev/video0 /tmp/out.mpg

**注意**对于不同的视频采集卡，你必须正确激活视频源和通道，例如 Gerd Knorr 的 `xawtv`。你还需要设置正确的音频记录层次和混合模式。只有这样你才能采集到想要的视音频。

## X11 显示的抓取

可以通过 ffmpeg 直接抓取 X11 显示内容：

> 
	ffmpeg -f x11grab -video_size cif -framerate 25 -i :0.0+10,20 /tmp/out.mpg

`0.0` 是 X11 服务的显示屏幕号 (display.screen)，定义于 `DISPLAY` 环境变量。10 是水平偏移，20 是垂直偏移

### 视频和音频文件格式转换

任何支持的文件格式或者协议都可以作为 ffmpeg 输入。例如：

- 你可以使用YUV文件作为输入

> 	
	ffmpeg -i /tmp/test%d.Y /tmp/out.mpg
	
这里可能是这样一些文件
    
> 
	/tmp/test0.Y, /tmp/test0.U, /tmp/test1.V,
	/tmp/test1.Y, /tmp/test1.U, /tmp/test1.V, etc...

这里 Y 还有对应分辨率的 2 个关联文件 U 和 V。这是一种 raw 数据文件而没有文件头，它可以被所有的视频解码器生成。你必须利用`-s`对它指定一个尺寸而不是让 ffmpeg 去猜测。

- 你可以把 raw YUV420P 文件作为输入：

> 
    ffmpeg -i /tmp/test/yuv /tmp/out.avi

test.yuv 是一个包含 raw YUV 通道数据的文件。每个帧先是 Y 数据，然后是 U 和 V 数据。

- 也可以输出 YUV420P 类型的文件

> 
    ffmpeg -i mydivx.avi hugefile.yuv

- 可以设置一些输入文件和输出文件  

> 
	ffmpeg -i /tmp/a.wav -s 640x480 -i /tmp/a.yuv /tmp/a.mpg
	
	这将转换一个音频和raw的YUV视频到一个MPEG文件中

- 你也可以同时对音频或者视频进行转换

> 
	ffmpeg -i /tmp/a.wav -ar 22050 /tmp/a.mp2

这里把 a.wav 转换为 MPEG 音频，同时转换了采样率为 22050HZ

- 你也可以利用映射同时编码多个格式作为输入或者输出：

> 
	ffmpeg -i /tmp/a.wav -map 0:a -b:a 64k /tmp/a.mp2 -map 0:a -b:a 128k /tmp/b.mp2

这将同时把 a.wav 以 64k 码率输出到 a.mp2，以 128k 码率输出到 b.mp2。 "-map file:index"指定了对于每个输出是连接到那个输入流的。

- 还可以转换解码 VOBs：  

> 
    ffmpeg -i snatch_1.vob -f avi -c:v mpeg4 -b:v 800k -g 300 -bf 2 -c:a libmp3lame -b:a 128k snatch.avi

这是一个典型的 DVD 抓取例子。这里的输入是一个 VOB 文件，输出是 MPEG-4 编码视频以及 MP3 编码音频的 AVI 文件。**注意**在这个命令行里使用了 B-frames （ B 帧）是兼容 DivX5 的，GOP 设置为 300 则意味着有一个内帧是适合 29.97fps 的输入视频。此外，音频流采用 MP3 编码需要运行 LAME 支持，它需要通过在编译是设置 `--enable-libmp3lame`。这种转换设置在多语言 DVD 抓取转换出所需的语言音频时特别有用。

**注意**要了解支持那些格式，可以采用 `ffmpeg -formats`

- 可以从一个视频扩展生成图片（序列），或者从一些图片生成视频：

- 导出图片

> 
	ffmpeg -i foo.avi -r 1 -s WxH -f image2 foo-%03d.jpeg

这将每秒依据 foo.avi 生成一个图片命名为 foo-001.jpeg,foo-002.jpeg 以此类推, 图片尺寸是 WxH 定义的值。

如果你想只生成有限数量的视频帧，你可以进一步结合 `-vframes` 或者 `-t` 或者 `-ss` 选项实现。

- 从图片生成视频

> 
	ffmpeg -f image2 -framerate 12 -i foo-%03d.jpeg -s WxH foo.avi

这里的语法 `foo- % 03d.jpeg` 指明使用 3 位数字来补充完整文件名，不足 3 位以 0 补齐。这类似于 C 语言的 printf 函数中的格式，但只接受常规整数作为部分。

当导入一个图片序列时，`-i` 也支持 shell 的通配符模式 ( 内置的 )，这需要同时选择 image2 的特性选项 `-pattern_type glob`：例如下面就利用了所有匹配 `foo- *.jpeg` 的图片序列创建一个视频：

> 
	ffmpeg -f image2 -pattern_type glob -framerate 12 -i 'foo-*.jpeg' -s WxH foo.avi

-  你可以把很多相同类型的流一起放到一个输出中：

> 
	ffmpeg -i test1.avi -i test2.avi -map 1:1 -map 1:0 -map 0:1 -map 0:0 -c copy -y test12.nut

这里最后输出文件 test12.nut 包括了 4 个流，其中流的顺序完全根据前面 `-map` 的指定顺序。

- 强制为固定码率编码 (CBR) 输出视频：

> 
	ffmpeg -i myfile.avi -b 4000k -minrate 4000k -maxrate 4000k -bufsize 1835k out.m2v

- 使用 `lambda` 工具的 4 个选项 `lmin`，`lmax`，`mblmin` 以及 `mblmax` 使你能更简单的从 `q` 转换到 `QP2LAMBDA` :

> 
	ffmpeg -i src.ext -lmax 21*QP2LAMBDA dst.ext

## 技巧/提示

- 如果流有非常低的码率，使用低帧率和小的 GOP 尺寸。这对于 RealVideo 在 Linux 下面的播放显得不是特别快时特别有用，因为它可以跳过一些帧，例如：

	ffmpeg -g 3 -r 3 -t 10 -b:v 50k -s qcif -f rv10 /tmp/b.rm

- 选项参数 `q` 将打开一个显示编码品质的水平数。值 1 表示非常高的品质，值 31 表示最差品质。如果 q = 31 经常出现，则表明当前编码码率不足以高品质的压缩保存你的内容，你可能需要提高码率或降低帧率或减小帧尺寸。
- 如果你的计算机不够快，你可以额外设定来加速。你可以使用 `-me zero` 加速运动预测，使用 `-g 0` 则完全禁用运动预测（这对于 I 帧的 JPEG 压缩会有益处）
- 如果你需要非常低的音频码率，降低采样率（对于 MPEG 音频采样 22050HZ, 对于 AC-3 采用 22050HZ 或者 11025HZ ）
- 需要一个恒定的质量（但编码率可变），使用选项 `qscale n` 进行设定，`n` 取值为 1(最好)-31(最差)。
