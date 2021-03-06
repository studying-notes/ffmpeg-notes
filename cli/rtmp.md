---
date: 2021-01-11T10:36:41+08:00  # 创建日期
author: "Rustle Karl"  # 作者

# 文章
title: "FFmpeg 推流"  # 文章标题
url:  "posts/ffmpeg/cli/rtmp"  # 设置网页链接，默认使用文件名
tags: [ "ffmpeg" ]  # 自定义标签
series: [ "FFmpeg 从入门到放弃"]  # 文章主题/文章系列
categories: [ "学习笔记"]  # 文章分类

# 章节
weight: 20 # 排序优先级
chapter: false  # 设置为章节

index: true  # 是否可以被索引
toc: true  # 是否自动生成目录
draft: false  # 草稿
---

## 检测源流信息

```shell
ffmpeg -i udp://localhost:10240
```

```shell
ffplay -f mpegts udp://localhost:10240
```

## 通过 Nginx 服务

```shell
ffmpeg -re -stream_loop -1 -i testdata/v1.mp4 -f flv rtmp://localhost:11935/live -y -hide_banner
```

```shell
ffplay rtmp://localhost:11935/live
```


## 推流

#### 命令参数解释

- -re:realtime 推的速度，按当前帧率推
- -map 0:0 第一个流的视频
- -map 0:1 第一个流的音频
- -map 1:0 第二个流的视频
- -map 1:1 第二个流的音频
- program_num=1:title=videoA:st=0:st=1 流 0,1 合成一个视频 videoA
- program_num=3:title=videoB:st=2:st=3 流 2,3 合成一个视频 videoB
- -stream_loop 循环推流
- -c copy 将视频中压缩的数据复制
- -c:v libh264 将视频流强制转换为 h264 编码

### 推一路流

```shell
ffmpeg -re -i testdata/v1.mp4 -map 0:0 -map 0:1 -program program_num=1:title=videoA:st=0:st=1  -f mpegts udp://localhost:10240
```

```shell
ffmpeg -re -i testdata/v1.mp4 -f mpegts -program program_num=1:title=videoA:st=0:st=1 udp:/localhost:10240 -y -hide_banner
```

### 循环推一路流，不做编码

```shell
ffmpeg -re -stream_loop 1 -i testdata/v1.mp4 -codec copy -f mpegts udp:/localhost:10240 -y -hide_banner
```

### 把 A.mp4 和 B.mp4 打包进同一个 ts 流，并推到本机的 10240 端口

```shell
ffmpeg -re -i testdata/rtmp/v1.mp4  -re -i testdata/rtmp/v2.mp4  -map 0:0 -map 0:1 -map 1:0 -map 1:1  -program program_num=1:title=videoA:st=0:st=1 -program program_num=3:title=videoB:st=2:st=3 -c copy -f mpegts udp://localhost:10240
```

```shell
ffmpeg -re -i testdata/v1.mp4  -re -i testdata/v2.mp4  -map 0:0 -map 0:1 -map 1:0 -map 1:1  -program program_num=1:title=videoA:st=0:st=1 -program program_num=3:title=videoB:st=2:st=3 -c copy -f mpegts udp://localhost:10240
```

### 推三路流

```shell
ffmpeg -re -i shenzhen.ts  -re -i guangdong.ts -re -i girl.mp4  -map 0:0 -map 0:1 -map 1:0 -map 1:1 -map 0:0 -map 0:1 -program program_num=1:title=videoA:st=0:st=1 -program program_num=3:title=videoB:st=2:st=3 -program program_num=5:title=videoXXX:st=4:st=5 -f mpegts udp://localhost:10003
```


### 查看流中的 program 信息

```shell
ffprobe -show_programs -print_format json -timeout 5000000 -i udp://localhost:10003
```

这个命令有返回值，0 表示执行成功，1 表示失败。

## 对视频流抽帧

```shell
ffmpeg -i 源流地址 -vframes 1 -vf scale=90:51 图片存放路径
```

## 重复推流

```shell
ffmpeg -re -stream_loop -1 -i aaa.xx -c copy -f mpegts udp://localhost:10003
```

```shell
./ffmpeg -re  -i -stream_loop -1 startslient.mp3  -c:a aac -f flv rtmp://123.56.17.184/live/znn
```

## 将流保存成文件

```shell
ffmpeg -i 流地址 -c copy 文件名
```

例如:

```shell
ffmpeg -i udp://localhost:10003 -c copy test.ts
```

## 检测源流中是否有静音

默认音频中如果有2秒是静音，就认为是有静音的

```shell
ffmpeg -i silence.mp3 -af silencedetect=noise=0.0001 -f null -
```

持续让 ffmpeg 检测音频 5 秒

```shell
ffmpeg -i silence.mp3 -af silencedetect=noise=0.0001 -t 0:0:5 -f null -
```

## 切分视频片段

### 切分纯视频片段

```shell
ffmpeg -i test.flv -vsync 0 -filter_complex "[0:v]select='between(t-start_t,8.250,10.000)'[xv];[xv]setpts='PTS-STARTPTS'[vout];[vout]scale=1920:1080,pad=1920:1080:0:0:black[vout2]" -map "[vout2]" -c:v libx264 test.ts
```

### 切分纯音频片段

```shell
ffmpeg -i test.mp3 -filter_complex "[0:a]aselect='between(t,0.000,26.250)'[xa];[xa]asetpts='NB_CONSUMED_SAMPLES/SAMPLE_RATE*1/TB'[aout] " -map "[aout]" out.mp3
```

### 切分带音频的视频片段

```shell
ffmpeg -i test.flv -filter_complex "[0:v]select='between(t-start_t,5.250,10.000)'[xv];[xv]setpts='PTS-STARTPTS'[vout];[vout]scale=[vout2]" -r 25 -map "[vout2]" -c:v libx264 -x264-params sps-id=6 out.ts
```

#### 命令参数解释

- -vsync 0：打印 ffmpeg 详细日志的开关
- black：添加黑边
- libx264：编码

## 合并视频片段

```shell
ffmpeg -f concat -safe 0 -i test.txt -c copy out.mp4
```

- test.txt：一个文件列表，里面存放着要合并的视频片段名
- -f concat -safe 0：打开文件时会用到

## 合并视频的同时添加图片和文字水印

```shell
ffmpeg -f concat -safe 0 -i test.txt -t 27.040 -loop 1 -i 1080P%05d.png -filter_complex "[0:v][1:v]overlay=x=0:y=885:enable='between(t,0,30)'[out];[out]drawtext=fontfile=simhei.ttf:text='name':x=414:y=928:fontsize=60:fontcolor=white:enable='between(t,0,30)'[out];[out]drawtext=fontfile=simhei.ttf:text='title':x=630:y=933:fontsize=52:fontcolor=white:enable='between(t,0,30)'" -y -c:v libx264 out.mp4
```

- -loop 1，让png序列循环

```shell

```

## 抓屏方法

在 Linux 下可以使用 x11grab 抓屏，在 MacOS 下可以使用 avfoundation 抓屏

### MacOS

#### 仅录制桌面

```shell
ffmpeg -f avfoundation -i "1" -vcodec libx264 -preset ultrafast -acodec libfaac -f flv rtmp://localhost/live
```

#### 桌面 + 麦克风

```shell
ffmpeg -f avfoundation -i "1:0" -vcodec libx264 -preset ultrafast -acodec libmp3lame -ar 44100 -ac 1 -f flv rtmp://localhost:1935/rtmplive/home 
```

#### 桌面 + 麦克风 + 摄像头拍摄

```shell
ffmpeg -f avfoundation -framerate 30 -i "1:0" \-f avfoundation -framerate 30 -video_size 640x480 -i "0" \-c:v libx264 -preset ultrafast \-filter_complex 'overlay=main_w-overlay_w-10:main_h-overlay_h-10' -acodec libmp3lame -ar 44100 -ac 1  -f flv rtmp://localhost:1935/rtmplive/home 
```

```shell

```



### Windows

#### gdigrab

基于 GDI 录屏，FFmpeg 直接从 Windows 的 GDI 中拷贝图形，然后再合成视频。

优点：省事，不需要依赖其他库

缺点：内存拷贝性能不是很好，没有硬件加速

gdigrab 是 FFmpeg 专门用于抓取 Windows 桌面的设备。非常适合用于屏幕录制。它通过不同的输入 URL 支持两种方式的抓取：

- “desktop”：抓取整张桌面。或者抓取桌面中的一个特定的区域。
- “title={窗口名称}”：抓取屏幕中特定的一个窗口。

gdigrab 另外还支持一些参数，用于设定抓屏的位置：

- offset_x：抓屏起始点横坐标。
- offset_y：抓屏起始点纵坐标。
- video_size：抓屏的大小。
- framerate：抓屏的帧率。

```shell
ffmpeg -f gdigrab -framerate 6 -offset_x 10 -offset_y 20 -video_size vga -i "desktop" -vcodec libx264 -preset ultrafast -acodec libfaac -f flv rtmp://localhost:11935/live
```

```shell
ffmpeg -framerate 30 -offset_x 0 -offset_y 0 -video_size vga -i "desktop" -acodec libfaac -f flv -preset ultrafast -vcodec libx264 rtmp://localhost:11935/live -y -hide_banner
```

#### directshow

基于directshow方案，FFmpeg从directshow驱动接口中读取。

优点：性能较好，directshow可以控制采样帧率，硬件加速。

缺点：需要安装directshow驱动，暂时找不到。。。

## 摄像头推流

```shell
ffmpeg -f dshow -i video="USB2.0 PC CAMERA" -vcodec libx264 -preset:v ultrafast -tune:v zerolatency -f flv rtmp://127.0.0.1:1935/live/123
```

## 麦克风推流

```shell
ffmpeg  -f dshow -i audio="麦克风 (2- USB2.0 MIC)" -vcodec libx264 -preset:v ultrafast -tune:v zerolatency -f flv rtmp://127.0.0.1:1935/live/123
```

## 摄像头&麦克风推流

```shell
ffmpeg -f dshow -i video="USB2.0 PC CAMERA" -f dshow -i audio="麦克风 (2- USB2.0 MIC)" -vcodec libx264 -preset:v ultrafast -tune:v zerolatency -f flv rtmp://127.0.0.1:1935/live/123
```


```shell
ffmpeg -f dshow -i video="USB2.0 PC CAMERA":audio="麦克风 (2- USB2.0 MIC)" -vcodec libx264  -r 25  -preset:v ultrafast -tune:v zerolatency -f flv rtmp://127.0.0.1:1935/live/123
```

```shell

```

```shell

```


```shell

```

```shell

```

```shell

```


```shell

```

```shell

```

```shell

```
