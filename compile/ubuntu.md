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

```shell
mkdir -p /build && cd /build
```

```shell
echo -e "deb http://mirrors.aliyun.com/ubuntu/ focal main restricted universe multiverse\ndeb-src http://mirrors.aliyun.com/ubuntu/ focal main restricted universe multiverse\ndeb http://mirrors.aliyun.com/ubuntu/ focal-security main restricted universe multiverse\ndeb-src http://mirrors.aliyun.com/ubuntu/ focal-security main restricted universe multiverse\ndeb http://mirrors.aliyun.com/ubuntu/ focal-updates main restricted universe multiverse\ndeb-src http://mirrors.aliyun.com/ubuntu/ focal-updates main restricted universe multiverse\ndeb http://mirrors.aliyun.com/ubuntu/ focal-proposed main restricted universe multiverse\ndeb-src http://mirrors.aliyun.com/ubuntu/ focal-proposed main restricted universe multiverse\ndeb http://mirrors.aliyun.com/ubuntu/ focal-backports main restricted universe multiverse\ndeb-src http://mirrors.aliyun.com/ubuntu/ focal-backports main restricted universe multiverse\n" >/etc/apt/sources.list
```

```shell
apt-get -y update && apt-get -y upgrade
```

```shell
export TZ=Asia/Shanghai
```

> 非最简依赖

```shell
apt-get -y install git \
                            autoconf \
                            automake \
                            build-essential \
                            cmake \
                            g++ \
                            gcc \
                            git-core \
                            libass-dev \
                            libfdk-aac-dev \
                            libfreetype6-dev \
                            libglew-dev \
                            libglfw3-dev \
                            libglu1-mesa-dev \
                            libgnutls28-dev \
                            libmp3lame-dev \
                            libopus-dev \
                            libsdl2-dev \
                            libtheora-dev \
                            libtool \
                            libva-dev \
                            libvdpau-dev \
                            libvorbis-dev \
                            libvpx-dev \
                            libx264-dev \
                            libx265-dev \
                            libxcb-shm0-dev \
                            libxcb-xfixes0-dev \
                            libxcb1-dev \
                            libxvidcore-dev \
                            make \
                            nasm \
                            pkg-config \
                            texinfo \
                            wget \
                            xorg-dev \
                            yasm \
                            zlib1g-dev \
                            gperf \
                            libglew2.1 \
                            xvfb
```

------------------------------------------------------------------

## 获取源码

```shell
git clone https://gitee.com/fujiawei/FFmpeg.git ffmpeg
```

----------------------------------------------------------

## x264 编译

```shell
cd ffmpeg
```

```shell
./configure --enable-shared --enable-static --disable-asm
```

```shell
make && make install
```

----------------------------------------------------------

## 带 OpenGL 转场特技

### 给 ffmpeg 打上补丁

```shell
cd /build && git clone https://gitee.com/fujiawei/ffmpeg-gl-transition.git
```

```shell
cd ffmpeg-gl-transition
git clone https://gitee.com/fujiawei/gl-transitions.git
```

```shell
cd /build/ffmpeg
```

```shell
git apply /build/ffmpeg-gl-transition/ffmpeg.diff
```

```shell
sudo apt-get install -y libglew2.0 # ubuntu 18
```

```shell
sudo apt-get install -y libglew2.1 # ubuntu 20
```

```shell
grep -v "define GL_TRANSITION_USING_EGL" /build/ffmpeg-gl-transition/vf_gltransition.c > /build/ffmpeg/libavfilter/vf_gltransition.c
```

-------------------------------------------------------------------------------

### 部分功能（不推荐）

```shell
cd /build/ffmpeg
```

```shell
./configure --enable-libx264 --enable-libx265 --enable-libvpx  --enable-libfdk-aac --enable-libmp3lame --enable-libopus --enable-nonfree --enable-gpl --enable-opengl --enable-filter=gltransition --extra-libs='-lGLEW -lglfw -ldl'
```

> 无法添加文字滤镜。。。

-------------------------------------------------------------------------------

### 增加文字滤镜

```shell
git clone https://gitee.com/fujiawei/mirror.git /build
```

> http://www.linuxfromscratch.org/blfs/view/svn/multimedia/libass.html

```shell
cd /build && git clone https://gitee.com/fujiawei/libass.git
```

```shell
mv /build/mirror/freetype-2.10.4.tar.xz  /build/freetype-2.10.4.tar.xz
```

```shell
tar -xf freetype-2.10.4.tar.xz
```

```shell
cd freetype-2.10.4/
```

```shell
./configure --prefix=/usr --enable-freetype-config --disable-static
```

```shell
make && make install
```

```shell
cd /build && mv /build/mirror/fribidi-1.0.9.tar.xz /build/fribidi-1.0.9.tar.xz
```

```shell
tar -xf fribidi-1.0.9.tar.xz
```

```shell
cd fribidi-1.0.9/
```

```shell
./configure --prefix=/usr
```

```shell
make && make install
```

```shell
cd /build && mv /build/mirror/nasm-2.15.05.tar.xz /build/nasm-2.15.05.tar.xz
```

```shell
tar -xf nasm-2.15.05.tar.xz
```

```shell
cd nasm-2.15.05/
```

```shell
./configure --prefix=/usr
```

```shell
make && make install
```

```shell
cd /build && mv /build/mirror/fontconfig-2.13.1.tar.bz2 /build/fontconfig-2.13.1.tar.bz2
```

```shell
tar -xf fontconfig-2.13.1.tar.bz2
```

```shell
cd fontconfig-2.13.1/
```

```shell
rm -f src/fcobjshash.h
```

```shell
./configure --prefix=/usr --sysconfdir=/etc --localstatedir=/var --disable-docs --docdir=/usr/share/doc/fontconfig-2.13.1
```

```shell
make && make install
```

```shell
cd /build/libass
```

```shell
sh autogen.sh
```

```shell
./configure --prefix=/usr --disable-static
```

```shell
make && make install
```

```shell
export PKG_CONFIG_PATH=/usr/local/ass/lib/pkgconfig:$PKG_CONFIG_PATH
```

```shell
cd /build && git clone --depth=1 https://gitee.com/fujiawei/SVT-AV1
```

```shell
cd SVT-AV1 && cd Build
```

```shell
cmake .. -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
```

```shell
make install
```

```shell
cd /build && git clone https://gitee.com/fujiawei/x264.git
```

```shell
cd x264
```

```shell
./configure --prefix=/usr --enable-static --enable-shared
```

```shell
make && make install
```

```shell
cd /build/ffmpeg
```

```shell
./configure \
--enable-gnutls \
--enable-gpl \
--enable-libass \
--enable-libfdk-aac \
--enable-libfreetype \
--enable-libmp3lame \
--enable-libopus \
--enable-libtheora \
--enable-libvorbis \
--enable-libvpx \
--enable-libx264 \
--enable-libx265 \
--enable-libxvid \
--enable-nonfree \
--enable-opengl \
--enable-filter=gltransition \
--extra-libs='-lGLEW -lglfw -ldl'
```

```shell
make -j && make install
```

> libx265 看似安装成功了，但始终无法用这个编码器

## 测试安装成功

```shell
./ffmpeg
```

编译结束后，执行 `./ffmpeg` 看到有 `--extra-libs='-lGLEW -lglfw -ldl'` 表示编译成功。

```shell
ffmpeg version N-100606-ga0acc44106 Copyright (c) 2000-2021 the FFmpeg developers
  built with gcc 9 (Ubuntu 9.3.0-17ubuntu1/build20.04)
  configuration: --enable-libx264 --enable-libx265 --enable-libvpx --enable-libfdk-aac --enable-libmp3lame --enable-libopus --enable-nonfree --enable-gpl --enable-opengl --enable-filter=gltransition --extra-libs='-lGLEW -lglfw -ldl'
```

```shell
./ffmpeg -filters | grep draw
```

```shell
ffmpeg -codecs
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

### 推荐

```shell
echo "alias ffmpeg='env DISPLAY=:1 ffmpeg -hide_banner'" >> /build/.profile
```

然后，可在另一个窗口执行：

```shell
env DISPLAY=:1 ffmpeg -i media/0.mp4 -i media/1.mp4 -filter_complex gltransition -y out.mp4
```

`env DISPLAY=:1` 是设置运行时环境变量。
