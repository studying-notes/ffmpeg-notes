---
date: 2020-09-15T22:20:49+08:00  # 创建日期
author: "Rustle Karl"  # 作者

# 文章
title: "FFmpeg 视频 Fliter 详解"  # 文章标题
url:  "posts/ffmpeg/cli/filter/video"  # 设置网页链接，默认使用文件名
tags: [ "ffmpeg", "filter" ]  # 自定义标签
series: [ "FFmpeg 从入门到放弃"]  # 文章主题/文章系列
categories: [ "学习笔记"]  # 文章分类

# 章节
weight: 20 # 文章在章节中的排序优先级，正序排序
chapter: false  # 将页面设置为章节

index: true  # 文章是否可以被索引
draft: false  # 草稿
---

- [](#)
- [常用 Filter 及其用途](#常用-filter-及其用途)
  - [scale 视频/图像的缩放](#scale-视频图像的缩放)
  - [crop 视频/图像的裁剪](#crop-视频图像的裁剪)
  - [trim 截取视频的片段](#trim-截取视频的片段)
  - [rotate 以任意角度旋转视频](#rotate-以任意角度旋转视频)
- [扣图合并](#扣图合并)
  - [chromakey](#chromakey)
  - [colorkey](#colorkey)
- [视频截图](#视频截图)
- [定时截图](#定时截图)
- [截取关键帧图片](#截取关键帧图片)
- [视觉处理](#视觉处理)
  - [增加亮度](#增加亮度)
  - [增加对比度](#增加对比度)

- [添加字幕](subtitle.md) 
- [添加水印](watermark.md)
- [Overlay Fliter 详解](overlay.md) - 视频/图像的叠加
- [淡入淡出与交叉淡入淡出](fade.md)

## vf 与 filter_complex 的区别

当我们通过 ffmpeg 使用简单的滤镜的时候，可以通过 `-vf` 与 `-af` 来实现滤镜效果。

当我们在处理复杂的滤镜场景的是，就需要使用 `-filter_complex` 参数来实现复杂的特效场景，其中 `-lavfi` 与 `-filter_complex` 效果是一样的。


## 常用 Filter 及其用途

### scale 视频/图像的缩放

```shell
scale=960:540
```

保持原始的宽高比：

```shell
scale=960:-1
```

###  crop 视频/图像的裁剪

从 (12, 34) 位置开始，将该点作为基准点，向右下进行裁剪，裁剪出 100x100 大小的视频：

```shell
crop=100:100:12:34
```

```shell
crop=w=100:h=100:x=12:y=34
```

### trim 截取视频的片段

只保留第二分钟：

```shell
trim=60:120
```

保留第一秒

```shell
trim=duration=1
```

### rotate 以任意角度旋转视频

## 扣图合并

colorkey 和 chromakey 用法参数基本一样

![](https://i.loli.net/2021/01/13/DwRJdNIoakr9y3l.png)

### chromakey

将绿色背景中的人物抠出来贴到另一个视频中：

```shell
ffmpeg -i input.mp4 -i input_green.mp4 -filter_complex "[1:v]chromakey=green:0.1:0.2[ckout];[0:v][ckout]overlay[out]" -map [out] output.mp4
```

### colorkey

```shell
ffmpeg -i input.png -vf colorkey=green out.png
```

```shell
ffmpeg -i background.png -i video.mp4 -filter_complex "[1:v]colorkey=0x3BBD1E:0.3:0.2[ckout];[0:v][ckout]overlay[out]" -map "[out]" output.flv
```

## 视频截图

```shell
ffmpeg -i video.mp4 -ss 00:00:04 -vframes 1 video.png
```

- `-ss` 定位时间
- `-vframes `限制输出的视频帧数

## 定时截图

```shell
ffmpeg -y -i video.mp4 -vf fps=1/2 video%d.png
```

- `fps` 帧率
- `fps=1/2` 每 2s 截图 1 张
- `fps=1` 每 1s 截图 1 张
- `fps=2` 每 1s 截图 2 张

## 截取关键帧图片

```shell
ffmpeg -y -i video.mp4 -vf "select='eq(pict_type,PICT_TYPE_I)'" -vsync vfr thumb%d.png
```

## 视觉处理

### 增加亮度

```shell
-vf eq=brightness=0.25
```

取值范围是从 -1.0 到 1.0，默认值是 0。

### 增加对比度

```shell
-vf eq=contrast=1.5
```

取值范围是从 -2.0 到 2.0，默认值是 1.0。
