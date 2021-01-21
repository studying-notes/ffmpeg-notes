---
date: 2021-01-11T10:20:49+08:00  # 创建日期
author: "Rustle Karl"  # 作者

# 文章
title: "Ubuntu 手动编译 FFmpeg"  # 文章标题
url:  "posts/ffmpeg/compile/ubuntu"  # 设置网页链接，默认使用文件名
tags: [ "ffmpeg" ]  # 自定义标签
series: [ "FFmpeg 从入门到放弃"]  # 文章主题/文章系列
categories: [ "学习笔记"]  # 文章分类

# 章节
weight: 20 # 文章在章节中的排序优先级，正序排序
chapter: false  # 将页面设置为章节

index: true  # 文章是否可以被索引
draft: false  # 草稿
---

## 获取源码

```shell
git clone https://gitee.com/fujiawei/FFmpeg.git ffmpeg
```

## 带 OpenGL 转场特技

### 给 ffmpeg 打上补丁

```shell
cd ~
```

```shell
git clone https://gitee.com/fujiawei/ffmpeg-gl-transition.git
```

```shell
cd ffmpeg-gl-transition
git clone https://gitee.com/fujiawei/gl-transitions.git
cd ..
```

```shell
cd ffmpeg
```

```shell
git apply ~/ffmpeg-gl-transition/ffmpeg.diff
```

### 添加依赖

```shell
apt-get -y install gcc g++ make xorg-dev pkg-config libglew-dev libglfw3-dev nasm yasm libx264-dev libx265-dev libvpx-dev libglu1-mesa-dev libmp3lame-dev libopus-dev libfdk-aac-dev
```

```shell
sudo apt-get install -y libglew2.0 # ubuntu 18
```

```shell
sudo apt-get install -y libglew2.1 # ubuntu 20
```

### 部分功能（不推荐）

```shell
grep -v "define GL_TRANSITION_USING_EGL" ~/ffmpeg-gl-transition/vf_gltransition.c > ~/ffmpeg/libavfilter/vf_gltransition.c
```

```shell
cd ~/ffmpeg
```

```shell
./configure --enable-libx264 --enable-libx265 --enable-libvpx  --enable-libfdk-aac --enable-libmp3lame --enable-libopus --enable-nonfree --enable-gpl --enable-opengl --enable-filter=gltransition --extra-libs='-lGLEW -lglfw -ldl'
```

无法添加文字滤镜。。。

### 完整功能

```shell
./configure --prefix=/usr/local --enable-gpl --enable-nonfree --enable-libass --enable-libfdk-aac --enable-libfreetype --enable-libmp3lame --enable-libtheora --enable-libvorbis --enable-libvpx --enable-libx264 --enable-libx265 --enable-libopus --enable-libxvid --enable-opengl --enable-filter=gltransition --extra-libs='-lGLEW -lglfw'
```

```shell
make -j && make install
```

## 测试安装成功

```shell
./ffmpeg
```

编译结束后，执行 `./ffmpeg` 看到有 `--extra-libs='-lGLEW -lglfw -ldl'` 表示编译成功。

```shell
ffmpeg version N-100606-ga0acc44106 Copyright (c) 2000-2021 the FFmpeg developers
  built with gcc 9 (Ubuntu 9.3.0-17ubuntu1~20.04)
  configuration: --enable-libx264 --enable-libx265 --enable-libvpx --enable-libfdk-aac --enable-libmp3lame --enable-libopus --enable-nonfree --enable-gpl --enable-opengl --enable-filter=gltransition --extra-libs='-lGLEW -lglfw -ldl'
  WARNING: library configuration mismatch
  avutil      configuration: --enable-shared --enable-static --disable-asm
  avcodec     configuration: --enable-shared --enable-static --disable-asm
  avformat    configuration: --enable-shared --enable-static --disable-asm
  avdevice    configuration: --enable-shared --enable-static --disable-asm
  avfilter    configuration: --enable-shared --enable-static --disable-asm
  swscale     configuration: --enable-shared --enable-static --disable-asm
  swresample  configuration: --enable-shared --enable-static --disable-asm
  libavutil      56. 63.100 / 56. 63.100
  libavcodec     58.115.102 / 58.115.102
  libavformat    58. 65.101 / 58. 65.101
  libavdevice    58. 11.103 / 58. 11.103
  libavfilter     7. 95.100 /  7. 95.100
  libswscale      5.  8.100 /  5.  8.100
  libswresample   3.  8.100 /  3.  8.100
  libpostproc    55.  8.100 / 55.  8.100
Hyper fast Audio and Video encoder
usage: ffmpeg [options] [[infile options] -i infile]... {[outfile options] outfile}...

Use -h to get full help or, even better, run 'man ffmpeg'
```

### 无窗口情况

```shell
apt-get -y install xvfb
```

启动

```shell
Xvfb :1 -screen 0 1280x1024x16
```

```shell
nohup Xvfb :1 -screen 0 1280x1024x16 &
```

```shell
nohup Xvfb -ac :1 -screen 0 1280x1024x16 > /dev/null 2>&1 &
export DISPLAY=:1
```

```shell
echo "alias ffmpeg='env DISPLAY=:1 ffmpeg -hide_banner'" >> ~/.profile
```

在另一个窗口执行：

```shell
env DISPLAY=:1 ffmpeg -i media/0.mp4 -i media/1.mp4 -filter_complex gltransition -y out.mp4
```

`env DISPLAY=:1` 是设置运行时环境变量。
