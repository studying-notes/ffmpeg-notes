---
date: 2021-01-11T10:20:49+08:00  # 创建日期
author: "Rustle Karl"  # 作者

# 文章
title: "Windows 手动编译 FFmpeg"  # 文章标题
url:  "posts/ffmpeg/compile/windows"  # 设置网页链接，默认使用文件名
tags: [ "ffmpeg" ]  # 自定义标签
series: [ "FFmpeg 从入门到放弃"]  # 文章主题/文章系列
categories: [ "学习笔记"]  # 文章分类

# 章节
weight: 20 # 文章在章节中的排序优先级，正序排序
chapter: false  # 将页面设置为章节

index: true  # 文章是否可以被索引
draft: false  # 草稿
---

# Windows 手动编译 FFmpeg

mingw-w64

```
https://sourceforge.net/projects/mingw-w64/
```

yasm 汇编编译器

```
http://www.tortall.net/projects/yasm/releases/yasm-1.3.0-win64.exe
```

```shell
mv yasm-1.3.0-win64.exe yasm.exe
```

## MSYS && MINGW64

```shell
pacman -S mingw-w64-x86_64-toolchain
```

```shell
pacman -S --noconfirm make
```

```shell
pacman -S --noconfirm diffutils
```

```shell
pacman -S --noconfirm yasm
```

```shell
pacman -S --noconfirm pkg-config
```

### x264 编译测试

必须是 MINGW64 终端

```shell
./configure --enable-shared --enable-static --disable-asm
```

```shell
make && make install
```

以下失败，不折腾

### 给 ffmpeg 打上补丁

```shell
cd ..
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
git apply ../ffmpeg-gl-transition/ffmpeg.diff
```

### disable EGL

```shell
cd ..
grep -v "define GL_TRANSITION_USING_EGL" ffmpeg-gl-transition/vf_gltransition.c > ffmpeg/libavfilter/vf_gltransition.c
```

```shell
cd ffmpeg
```

```shell
./configure --enable-gpl --enable-opengl --enable-filter=gltransition --extra-libs='-lGLEW -lEGL'
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

## 交叉编译

```shell
apt install -y subversion ragel curl texinfo g++ bison flex cvs yasm automake libtool autoconf gcc cmake git make pkg-config zlib1g-dev unzip pax nasm gperf autogen bzip2 autoconf-archive p7zip-full meson clang
```

```shell
apt-get install -y gcc mingw-w64 mingw-w64-tools binutils-mingw-w64 binutils-mingw-w64-x86-64 binutils-mingw-w64-i686 mingw-w64-x86-64-dev mingw-w64-i686-dev
```

```shell
export CC=/usr/bin/x86_64-w64-mingw32-gcc-win32
export CXX=/usr/bin/x86_64-w64-mingw32-g++-win32
export CPP=/usr/bin/x86_64-w64-mingw32-cpp-win32
export CXXCPP=/usr/bin/x86_64-w64-mingw32-cpp-win32
export AR=/usr/bin/x86_64-w64-mingw32-gcc-ar-win32
export NM=/usr/bin/x86_64-w64-mingw32-gcc-nm-win32
export RANLIB=/usr/bin/x86_64-w64-mingw32-gcc-ranlib-win32
export DLLTOOL=/usr/bin/x86_64-w64-mingw32-dlltool
export DLLWRAP=/usr/bin/x86_64-w64-mingw32-dllwrap
export LD=/usr/bin/x86_64-w64-mingw32-ld
export OBJCOPY=/usr/bin/x86_64-w64-mingw32-objcopy
export OBJDUMP=/usr/bin/x86_64-w64-mingw32-objdump
export PKGCONFIG=/usr/bin/x86_64-w64-mingw32-pkg-config
```

### x264 编译

```shell
./configure --arch=x86_64 --target-os=mingw32 --cross-prefix=x86_64-w64-mingw32- --enable-shared --enable-static --disable-asm
```

```shell
make && make install
```

```shell
git apply ~/ffmpeg-gl-transition/ffmpeg.diff
```

```shell
grep -v "define GL_TRANSITION_USING_EGL" ~/ffmpeg-gl-transition/vf_gltransition.c > ~/ffmpeg/libavfilter/vf_gltransition.c
```

```shell
./configure --arch=x86 --target-os=mingw32 --cross-prefix=i686-w64-mingw32-  --enable-libx264 --enable-libx265 --enable-libvpx  --enable-libfdk-aac --enable-libmp3lame --enable-libopus --enable-nonfree --enable-gpl --enable-opengl --enable-filter=gltransition --extra-libs='-lGLEW -lglfw -ldl'
```

```shell
make
```
