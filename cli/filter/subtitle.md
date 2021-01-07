---
date: 2020-09-15T22:20:49+08:00  # 创建日期
author: "Rustle Karl"  # 作者

# 文章
title: "FFmpeg 添加字幕"  # 文章标题
url:  "posts/ffmpeg/cli/filter/subtitle"  # 设置网页链接，默认使用文件名
tags: [ "ffmpeg", "filter"]  # 自定义标签
series: [ "FFmpeg 从入门到放弃"]  # 文章主题/文章系列
categories: [ "学习笔记"]  # 文章分类

# 章节
weight: 20 # 文章在章节中的排序优先级，正序排序
chapter: false  # 将页面设置为章节

index: true  # 文章是否可以被索引
draft: false  # 草稿
---

## 字幕格式

### 外挂字幕与内嵌字幕

- 外挂字幕：是和视频文件分离的字幕文件，在播放的时候要导入字幕文件；
- 内嵌字幕：视频文件和字幕文件集成到一起，没有办法改变和去掉。

### 外挂字幕的三种格式

- srt 格式：体积小，用记事本可以打开编辑；
- sub+idx：图形字幕，只能用字幕转换软件，体积较大；
- ass 字幕：比较少，比 srt 多一些特效。

## 写入视频流

这种方法产生的字幕是内嵌字幕/硬字幕。

```shell
# 字幕文件
ffmpeg -i input.mkv -vf subtitles=subtitles.srt output.mkv

# 其他视频的字幕流
ffmpeg -i input.mkv -vf subtitles=input.mkv output.mp4

# 指定多个中的某个字幕流
ffmpeg -i input.mkv -vf subtitles=input.mkv:si=1 output.mp4
```

## 写入封装容器

这种方法产生的字幕是内封字幕/软字幕。字幕流和视频流、音频流同等地位，但并非所有的容器都支持字幕流。

```shell
# 将字幕封装到容器中
ffmpeg -i input.mkv -i subtitles.srt -c copy output.mkv

# 从容器中提取字幕流
ffmpeg -i input.mkv output.srt
```
