---
date: 2021-03-08T22:56:00+08:00  # 创建日期
author: "Rustle Karl"  # 作者

# 文章
title: "FFmpeg 视频滤镜篇"  # 文章标题
# description: "文章描述"
url:  "posts/ffmpeg/blogs/video_filters"  # 设置网页永久链接
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

FFmpeg 自带了大概有 450 多个音视频媒体滤镜，基本上涵盖了视频处理的绝大部分功能。但是由于对这些滤镜不可能了如指掌，甚至大部分滤镜都没见过，就可能不知道如何实现一个需求，所以还是有必要尝试一遍 FFmpeg 的全部滤镜吧。所以就决定写一下这个系列，力求给出每个滤镜的示例及最终结果对比视频/图，但也有可能中途夭折。

该系列文章的 FFmpeg 命令都是由 FFmepg-Generator 生成的，不论是出于复用考量，还是方便书写，记原生命令太为难普通人了。不过该项目现在还在测试中，暂时未开源。

```python
from ffmpeg import input, vtools
from testdata import *
```

FFmpeg 的滤镜参数可以是位置参数，即不提供参数名，只提供参数值，这种情况下参数的顺序是不可以改变的；另一种的关键词参数，以键值对的形式指明参数，位置不限。

## 1. addroi

> https://ffmpeg.org/ffmpeg-filters.html#addroi

ROI（region of interest），感兴趣区域。机器视觉、图像处理中，从被处理的图像以方框、圆、椭圆、不规则多边形等方式勾勒出需要处理的区域，称为感兴趣区域，ROI。

该滤镜用于指定视频中若干个区域为感兴趣区域，但是视频帧信息不改变，只是在 metadata 中添加 roi 的信息，影响稍后的编码过程，通过多次应用该滤镜可以标记多个区域。

### 参数

- x 与帧左边的像素距离
- y 与帧上边的像素距离
- w 区域像素宽，iw 表示输入帧宽
- h 区域像素高，ih 表示输入帧高
- qoffset 标记区域编码质量偏移，介于 -1 到 1，负值表示更高质量，0 表示保持质量，正值表示更低质量，可以认为是不感兴趣
- clear 设置为 true 则移除全部已存在的区域标记

### 示例

#### 标记视频的中心区域

为了对比明显，设置了最差质量。

```python
_ = input(src).addroi(x="iw/4", y="ih/4", w="iw/2", h="ih/2", qoffset=1, clear=1).output(dst).run()
```

> 该代码可生成下面的命令并执行，最后给出运行时间。

```
ffmpeg -i testdata\media\0.mp4 -filter_complex "[0]addroi=clear=1:h=ih/2:qoffset=1:w=iw/2:x=iw/4:y=ih/4[tag0]" -map [tag0] testdata\media\v0_addroi.mp4 -y -hide_banner
[0.5302s]
```

##### 对比

原视频 221 KB，合成视频 202 KB。对比视频（默认左边原视频）：


```python
vtools.compare_2_videos(src, dst, src_dst_compare)
```

> 之后对比用的代码都是一样，所以就这里写一次。

```
ffmpeg -i testdata\media\0.mp4 -i testdata\media\v0_addroi.mp4 -filter_complex "[0]pad=w=2*iw[tag0];[tag0][1]overlay=x=w[tag1]" -map [tag1] testdata\media\v0_addroi_compare.mp4 -y -hide_banner
[1.0771s]
```

[视频对比链接]

## 2. alphaextract

> https://ffmpeg.org/ffmpeg-filters.html#alphaextract

从输入中提取 alpha 分量作为灰度视频。这对于 alphamerge 过滤器特别有用。

### 参数

无

### 示例

首先要确定输入的视频有 alpha 通道，不存在则报错。即必须是 rgba 格式的，不然会提取失败。rgba 格式是在普通的 rgb 格式基础上增长了一个 alpha 分量，该分量用于表示图像的透明度。

太遗憾了，我找不到这种符合格式的视频，最终尝试用 PNG 格式的图片代替视频测试：

```python
_ = input(src).alphaextract().output(dst).run()
```

```
ffmpeg -i testdata\i2.png -filter_complex "[0]alphaextract[tag0]" -map [tag0] testdata\i2_alphaextract.png -y -hide_banner
[0.1415s]
```

#### 对比

[视频对比链接]


## 3. alphamerge

> https://ffmpeg.org/ffmpeg-filters.html#alphamerge

用第二个输入视频的灰度值添加或替换第一个输入视频的 alpha 分量。 可以与 alphaextract 一起使用，以允许传输或存储具有 alpha 格式但不支持 alpha 通道的帧序列。

### 参数

无

### 示例

```python
_ = input(src).movie(filename=i2_alpha.as_posix()).alphamerge().output(dst).run()
```

```
ffmpeg -i testdata\i3.png -filter_complex "movie=filename=testdata/i2_alpha.png[tag0];[0][tag0]alphamerge[tag1]" -map [tag1] testdata\i3_alphamerge.png -y -hide_banner
```

等价写法：

```python
_ = input(src).alphamerge(input(i2_alpha)).output(dst).run()
```

```
ffmpeg -i testdata\i3.png -i testdata\i2_alpha.png -filter_complex "[0][1]alphamerge[tag0]" -map [tag0] testdata\i3_alphamerge.png -y -hide_banner
[0.1514s]
```

#### 对比

黑色部分实际上是透明的。

[视频对比链接]

## 4. amplify

> https://ffmpeg.org/ffmpeg-filters.html#amplify

放大当前帧和相邻帧相同位置的像素差异。

### 参数

- radius 设置取相邻帧的数量，范围 1 ~ 63，默认 2，比如设置为 3，即取前后各 3 帧加上当前帧，计算 7 帧的平均值
- factor 设置差异放大因子，范围 0 ~ 65535，默认 2
- threshold 设置差异上限，范围 0 ~ 65535，默认 2，任何大于等于该上限的像素差异都不会放大（差异已经够大，所以不再放大）。
- tolerance 设置差异下限，范围 0 ~ 65535，默认 0，任何小于该下限的像素差异都不会放大（差异过小）。
- low 设置更改源像素的下限。默认是 65535。取值范围是 0 ~ 65535。降低源像素值的最大可能值。
- high 设置更改源像素的上限。 默认值为 65535。允许的范围是 0 到 65535。增加源像素值的最大可能值。
- planes 设置进行处理的通道。 默认为全部。 允许范围是 0 到 15。

### 示例

```python
_ = input(src).amplify(radius=3, factor=10, threshold=50).output(dst).run()
```

```
ffmpeg -i testdata\media\0.mp4 -filter_complex "[0]amplify=factor=10:radius=3:threshold=50[tag0]" -map [tag0] testdata\media\v0_amplify.mp4 -y -hide_banner
[1.0090s]
```

#### 对比

[视频对比链接]

## 5. ass

> https://ffmpeg.org/ffmpeg-filters.html#ass

用于添加 ASS 格式字幕的滤镜。用法与 subtitles 滤镜相同。字幕文件由[Aegisub](https://aegi.vmoe.info/downloads/)制作。

### 参数

- shaping 设置渲染引擎。
  - auto 默认的 libass 引擎
  - simple font-agnostic 引擎
  - complex OpenType 引擎

其他与 subtitles 滤镜参数相同。

### 示例

```python
_ = input(src).ass(filename=media_v0_ass.as_posix()).output(dst).run()
```

```
ffmpeg -i testdata\media\0.mp4 -filter_complex "[0]ass=filename=testdata/media/0.ass[tag0]" -map [tag0] testdata\media\v0_ass.mp4 -y -hide_banner
[0.6707s]
```

#### 对比

[视频对比链接]

## 6. atadenoise

> https://ffmpeg.org/ffmpeg-filters.html#atadenoise

自适应时域平均降噪器 (Adaptive Temporal Averaging Denoiser)。

### 参数

- 0a 设置 1 通道阈值 A，范围 0 ~ 0.3，默认 0.02
- 0b 设置 1 通道阈值 B，范围 0 ~ 5，默认0.04
- 1a 同上类推
- 1b 同上类推
- 2a 同上类推
- 2b 同上类推
- s 设置用于平均的帧数，范围 5 到 129 的奇数，默认 9
- p 设置通道，默认全部
- a 设置算法过滤器将用于平均的变量。 默认为 p 并行。 也可以将其设置为 s 串行。并行可以比串行更快。 并行将在第一个变化大于阈值时提前中止，而串行将继续处理帧的另一侧（等于或小于阈值时）。
- 0s 同下类推
- 1s 同下类推
- 2s 为通道设置 sigma。 默认值为 32767。有效范围是 0 到 32767。控制由尺寸定义的半径中每个像素的权重。 默认值表示每个像素具有相同的权重。 设置为 0 可以有效地禁用过滤。

### 示例

```python
_ = input(src).atadenoise(s=25).output(dst).run()
```

```
ffmpeg -i testdata\media\0.mp4 -filter_complex "[0]atadenoise=s=25[tag0]" -map [tag0] testdata\media\v0_atadenoise.mp4 -y -hide_banner
[1.5373s]
```

#### 对比

肉眼看不出差别：

[视频对比链接]

## 7. avgblur

> https://ffmpeg.org/ffmpeg-filters.html#avgblur

平均模糊滤波器。

### 参数

- sizeX 水平半径
- sizeY 垂直半径
- planes 通道，默认全部

### 示例

```python
_ = input(src).avgblur(x=10, y=10).output(dst).run()
```

```
ffmpeg -i testdata\media\0.mp4 -filter_complex "[0]avgblur=sizeX=10:sizeY=10[tag0]" -map [tag0] testdata\media\v0_avgblur.mp4 -y -hide_banner
[0.7048s]
```

#### 对比

[视频对比链接]

## 8. bbox

> https://ffmpeg.org/ffmpeg-filters.html#bbox

计算输入帧亮度平面中非黑色像素的边界框。

该过滤器计算包含所有像素的边界框，这些像素的亮度值大于最小允许值。 描述边界框的参数打印在过滤器日志中。对于输出视频无影响。

### 参数

- min_val 最小亮度值，默认 16

### 示例

```python
_ = input(src).bbox(min_val=100).output(dst).run(capture_stderr=False,capture_stdout=False)
```

```
ffmpeg -i testdata\media\0.mp4 -filter_complex "[0]bbox=min_val=100[tag0]" -map [tag0] testdata\media\v0_bbox.mp4 -y -hide_banner
[0.9152s]
```

#### 对比

只是输出了描述边界框的参数，对于输出视频无影响。


## 9. bilateral

> https://ffmpeg.org/ffmpeg-filters.html#bilateral

双边过滤器，在保留边缘的同时进行空间平滑。

### 参数

- sigmaS 设置高斯函数的 sigma 以计算空间权重。 允许范围为 0 到 512。默认值为 0.1
- sigmaR 设置高斯函数的 sigma 以计算范围权重。允许范围为 0 到 1。默认值为 0.1
- planes 设置通道，默认第一通道

### 示例

```python
_ = input(src).bilateral(s=12, r=0.3).output(dst).run()
```

```
ffmpeg -i testdata\media\0.mp4 -filter_complex "[0]bilateral=sigmaR=0.3:sigmaS=12[tag0]" -map [tag0] testdata\media\v0_bilateral.mp4 -y -hide_banner
[1.6872s]
```

#### 对比

[视频对比链接]


## 10 bitplanenoise

> https://ffmpeg.org/ffmpeg-filters.html#bitplanenoise

显示和测量位通道噪声。

### 参数

- bitplane 指定分析通道，默认 1
- filter 从上面设置的位通道中滤除噪点像素，默认设置为禁用。

### 示例

```python
_ = input(src).bitplanenoise(filter=True).output(dst).run()
```

```
ffmpeg -i testdata\media\0.mp4 -filter_complex "[0]bitplanenoise=filter=True[tag0]" -map [tag0] testdata\media\v0_bitplanenoise.mp4 -y -hide_banner
[0.8679s]
```

#### 对比

[视频对比链接]

## 11. blackdetect

> https://ffmpeg.org/ffmpeg-filters.html#blackdetect

检测视频中全黑的段落。

### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 12. blackframe

> https://ffmpeg.org/ffmpeg-filters.html#blackframe


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 13. blend

> https://ffmpeg.org/ffmpeg-filters.html#blend


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 14. bm3d

> https://ffmpeg.org/ffmpeg-filters.html#bm3d


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 15. boxblur

> https://ffmpeg.org/ffmpeg-filters.html#boxblur


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 16. bwdif

> https://ffmpeg.org/ffmpeg-filters.html#bwdif


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 17. cas

> https://ffmpeg.org/ffmpeg-filters.html#cas


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 18. chromahold

> https://ffmpeg.org/ffmpeg-filters.html#chromahold


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 19. chromakey

> https://ffmpeg.org/ffmpeg-filters.html#chromakey


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 20. chromanr

> https://ffmpeg.org/ffmpeg-filters.html#chromanr


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 21. chromashift

> https://ffmpeg.org/ffmpeg-filters.html#chromashift


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 22. ciescope

> https://ffmpeg.org/ffmpeg-filters.html#ciescope


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 23. codecview

> https://ffmpeg.org/ffmpeg-filters.html#codecview


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 24. colorbalance

> https://ffmpeg.org/ffmpeg-filters.html#colorbalance


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 25. colorcontrast

> https://ffmpeg.org/ffmpeg-filters.html#colorcontrast


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 26. colorcorrect

> https://ffmpeg.org/ffmpeg-filters.html#colorcorrect


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 27. colorchannelmixer

> https://ffmpeg.org/ffmpeg-filters.html#colorchannelmixer


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 28. colorize

> https://ffmpeg.org/ffmpeg-filters.html#colorize


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 29. colorkey

> https://ffmpeg.org/ffmpeg-filters.html#colorkey


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 30. colorhold

> https://ffmpeg.org/ffmpeg-filters.html#colorhold


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 31. colorlevels

> https://ffmpeg.org/ffmpeg-filters.html#colorlevels


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 32. colormatrix

> https://ffmpeg.org/ffmpeg-filters.html#colormatrix


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 33. colorspace

> https://ffmpeg.org/ffmpeg-filters.html#colorspace


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 34. colortemperature

> https://ffmpeg.org/ffmpeg-filters.html#colortemperature


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 35. convolution

> https://ffmpeg.org/ffmpeg-filters.html#convolution


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 36. convolve

> https://ffmpeg.org/ffmpeg-filters.html#convolve


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 37. copy

> https://ffmpeg.org/ffmpeg-filters.html#copy


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 38. coreimage

> https://ffmpeg.org/ffmpeg-filters.html#coreimage


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 39. cover_rect

> https://ffmpeg.org/ffmpeg-filters.html#cover_rect


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 40. crop

> https://ffmpeg.org/ffmpeg-filters.html#crop


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 41. cropdetect

> https://ffmpeg.org/ffmpeg-filters.html#cropdetect


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 42. cue

> https://ffmpeg.org/ffmpeg-filters.html#cue


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 43. curves

> https://ffmpeg.org/ffmpeg-filters.html#curves


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 44. datascope

> https://ffmpeg.org/ffmpeg-filters.html#datascope


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 45. dblur

> https://ffmpeg.org/ffmpeg-filters.html#dblur


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 46. dctdnoiz

> https://ffmpeg.org/ffmpeg-filters.html#dctdnoiz


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 47. deband

> https://ffmpeg.org/ffmpeg-filters.html#deband


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 48. deblock

> https://ffmpeg.org/ffmpeg-filters.html#deblock


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 49. decimate

> https://ffmpeg.org/ffmpeg-filters.html#decimate


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 50. deconvolve

> https://ffmpeg.org/ffmpeg-filters.html#deconvolve


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 51. dedot

> https://ffmpeg.org/ffmpeg-filters.html#dedot


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 52. deflate

> https://ffmpeg.org/ffmpeg-filters.html#deflate


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 53. deflicker

> https://ffmpeg.org/ffmpeg-filters.html#deflicker


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 54. dejudder

> https://ffmpeg.org/ffmpeg-filters.html#dejudder


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 55. delogo

> https://ffmpeg.org/ffmpeg-filters.html#delogo


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 56. derain

> https://ffmpeg.org/ffmpeg-filters.html#derain


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 57. deshake

> https://ffmpeg.org/ffmpeg-filters.html#deshake


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 58. despill

> https://ffmpeg.org/ffmpeg-filters.html#despill


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 59. detelecine

> https://ffmpeg.org/ffmpeg-filters.html#detelecine


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 60. dilation

> https://ffmpeg.org/ffmpeg-filters.html#dilation


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 61. displace

> https://ffmpeg.org/ffmpeg-filters.html#displace


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 62. dnn_processing

> https://ffmpeg.org/ffmpeg-filters.html#dnn_processing


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 63. drawbox

> https://ffmpeg.org/ffmpeg-filters.html#drawbox


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 64. drawgraph

> https://ffmpeg.org/ffmpeg-filters.html#drawgraph


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 65. drawgrid

> https://ffmpeg.org/ffmpeg-filters.html#drawgrid


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 66. drawtext

> https://ffmpeg.org/ffmpeg-filters.html#drawtext


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 67. edgedetect

> https://ffmpeg.org/ffmpeg-filters.html#edgedetect


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 68. elbg

> https://ffmpeg.org/ffmpeg-filters.html#elbg


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 69. entropy

> https://ffmpeg.org/ffmpeg-filters.html#entropy


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 70. epx

> https://ffmpeg.org/ffmpeg-filters.html#epx


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 71. eq

> https://ffmpeg.org/ffmpeg-filters.html#eq


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 72. erosion

> https://ffmpeg.org/ffmpeg-filters.html#erosion


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 73. estdif

> https://ffmpeg.org/ffmpeg-filters.html#estdif


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 74. exposure

> https://ffmpeg.org/ffmpeg-filters.html#exposure


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 75. extractplanes

> https://ffmpeg.org/ffmpeg-filters.html#extractplanes


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 76. fade

> https://ffmpeg.org/ffmpeg-filters.html#fade


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 77. fftdnoiz

> https://ffmpeg.org/ffmpeg-filters.html#fftdnoiz


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 78. fftfilt

> https://ffmpeg.org/ffmpeg-filters.html#fftfilt


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 79. field

> https://ffmpeg.org/ffmpeg-filters.html#field


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 80. fieldhint

> https://ffmpeg.org/ffmpeg-filters.html#fieldhint


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 81. fieldmatch

> https://ffmpeg.org/ffmpeg-filters.html#fieldmatch


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 82. fieldorder

> https://ffmpeg.org/ffmpeg-filters.html#fieldorder


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 83. fifo

> https://ffmpeg.org/ffmpeg-filters.html#fifo


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 84. afifo

> https://ffmpeg.org/ffmpeg-filters.html#afifo


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 85. fillborders

> https://ffmpeg.org/ffmpeg-filters.html#fillborders


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 86. find_rect

> https://ffmpeg.org/ffmpeg-filters.html#find_rect


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 87. floodfill

> https://ffmpeg.org/ffmpeg-filters.html#floodfill


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 88. format

> https://ffmpeg.org/ffmpeg-filters.html#format


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 89. fps

> https://ffmpeg.org/ffmpeg-filters.html#fps


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 90. framepack

> https://ffmpeg.org/ffmpeg-filters.html#framepack


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 91. framerate

> https://ffmpeg.org/ffmpeg-filters.html#framerate


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 92. framestep

> https://ffmpeg.org/ffmpeg-filters.html#framestep


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 93. freezedetect

> https://ffmpeg.org/ffmpeg-filters.html#freezedetect


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 94. freezeframes

> https://ffmpeg.org/ffmpeg-filters.html#freezeframes


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 95. frei0r

> https://ffmpeg.org/ffmpeg-filters.html#frei0r


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 96. fspp

> https://ffmpeg.org/ffmpeg-filters.html#fspp


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 97. gblur

> https://ffmpeg.org/ffmpeg-filters.html#gblur


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 98. geq

> https://ffmpeg.org/ffmpeg-filters.html#geq


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 99. gradfun

> https://ffmpeg.org/ffmpeg-filters.html#gradfun


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 100. graphmonitor

> https://ffmpeg.org/ffmpeg-filters.html#graphmonitor


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 101. greyedge

> https://ffmpeg.org/ffmpeg-filters.html#greyedge


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 102. haldclut

> https://ffmpeg.org/ffmpeg-filters.html#haldclut


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 103. hflip

> https://ffmpeg.org/ffmpeg-filters.html#hflip


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 104. histeq

> https://ffmpeg.org/ffmpeg-filters.html#histeq


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 105. histogram

> https://ffmpeg.org/ffmpeg-filters.html#histogram


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 106. hqdn3d

> https://ffmpeg.org/ffmpeg-filters.html#hqdn3d


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 107. hwdownload

> https://ffmpeg.org/ffmpeg-filters.html#hwdownload


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 108. hwmap

> https://ffmpeg.org/ffmpeg-filters.html#hwmap


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 109. hwupload

> https://ffmpeg.org/ffmpeg-filters.html#hwupload


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 110. hwupload_cuda

> https://ffmpeg.org/ffmpeg-filters.html#hwupload_cuda


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 111. hqx

> https://ffmpeg.org/ffmpeg-filters.html#hqx


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 112. hstack

> https://ffmpeg.org/ffmpeg-filters.html#hstack


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 113. hue

> https://ffmpeg.org/ffmpeg-filters.html#hue


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 114. hysteresis

> https://ffmpeg.org/ffmpeg-filters.html#hysteresis


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 115. identity

> https://ffmpeg.org/ffmpeg-filters.html#identity


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 116. idet

> https://ffmpeg.org/ffmpeg-filters.html#idet


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 117. il

> https://ffmpeg.org/ffmpeg-filters.html#il


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 118. inflate

> https://ffmpeg.org/ffmpeg-filters.html#inflate


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 119. interlace

> https://ffmpeg.org/ffmpeg-filters.html#interlace


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 120. kerndeint

> https://ffmpeg.org/ffmpeg-filters.html#kerndeint


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 121. kirsch

> https://ffmpeg.org/ffmpeg-filters.html#kirsch


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 122. lagfun

> https://ffmpeg.org/ffmpeg-filters.html#lagfun


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 123. lenscorrection

> https://ffmpeg.org/ffmpeg-filters.html#lenscorrection


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 124. lensfun

> https://ffmpeg.org/ffmpeg-filters.html#lensfun


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 125. libvmaf

> https://ffmpeg.org/ffmpeg-filters.html#libvmaf


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 126. limiter

> https://ffmpeg.org/ffmpeg-filters.html#limiter


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 127. loop

> https://ffmpeg.org/ffmpeg-filters.html#loop


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 128. lut1d

> https://ffmpeg.org/ffmpeg-filters.html#lut1d


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 129. lut3d

> https://ffmpeg.org/ffmpeg-filters.html#lut3d


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 130. lumakey

> https://ffmpeg.org/ffmpeg-filters.html#lumakey


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 131. lut

> https://ffmpeg.org/ffmpeg-filters.html#lut


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 132. lutrgb

> https://ffmpeg.org/ffmpeg-filters.html#lutrgb


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 133. lutyuv

> https://ffmpeg.org/ffmpeg-filters.html#lutyuv


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 134. lut2

> https://ffmpeg.org/ffmpeg-filters.html#lut2


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 135. tlut2

> https://ffmpeg.org/ffmpeg-filters.html#tlut2


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 136. maskedclamp

> https://ffmpeg.org/ffmpeg-filters.html#maskedclamp


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 137. maskedmax

> https://ffmpeg.org/ffmpeg-filters.html#maskedmax


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 138. maskedmerge

> https://ffmpeg.org/ffmpeg-filters.html#maskedmerge


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 139. maskedmin

> https://ffmpeg.org/ffmpeg-filters.html#maskedmin


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 140. maskedthreshold

> https://ffmpeg.org/ffmpeg-filters.html#maskedthreshold


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 141. maskfun

> https://ffmpeg.org/ffmpeg-filters.html#maskfun


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 142. mcdeint

> https://ffmpeg.org/ffmpeg-filters.html#mcdeint


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 143. median

> https://ffmpeg.org/ffmpeg-filters.html#median


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 144. mergeplanes

> https://ffmpeg.org/ffmpeg-filters.html#mergeplanes


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 145. mestimate

> https://ffmpeg.org/ffmpeg-filters.html#mestimate


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 146. midequalizer

> https://ffmpeg.org/ffmpeg-filters.html#midequalizer


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 147. minterpolate

> https://ffmpeg.org/ffmpeg-filters.html#minterpolate


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 148. mix

> https://ffmpeg.org/ffmpeg-filters.html#mix


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 149. monochrome

> https://ffmpeg.org/ffmpeg-filters.html#monochrome


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 150. mpdecimate

> https://ffmpeg.org/ffmpeg-filters.html#mpdecimate


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 151. msad

> https://ffmpeg.org/ffmpeg-filters.html#msad


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 152. negate

> https://ffmpeg.org/ffmpeg-filters.html#negate


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 153. nlmeans

> https://ffmpeg.org/ffmpeg-filters.html#nlmeans


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 154. nnedi

> https://ffmpeg.org/ffmpeg-filters.html#nnedi


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 155. noformat

> https://ffmpeg.org/ffmpeg-filters.html#noformat


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 156. noise

> https://ffmpeg.org/ffmpeg-filters.html#noise


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 157. normalize

> https://ffmpeg.org/ffmpeg-filters.html#normalize


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 158. null

> https://ffmpeg.org/ffmpeg-filters.html#null


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 159. ocr

> https://ffmpeg.org/ffmpeg-filters.html#ocr


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 160. ocv

> https://ffmpeg.org/ffmpeg-filters.html#ocv


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 161. oscilloscope

> https://ffmpeg.org/ffmpeg-filters.html#oscilloscope


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 162. overlay

> https://ffmpeg.org/ffmpeg-filters.html#overlay


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 163. overlay_cuda

> https://ffmpeg.org/ffmpeg-filters.html#overlay_cuda


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 164. owdenoise

> https://ffmpeg.org/ffmpeg-filters.html#owdenoise


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 165. pad

> https://ffmpeg.org/ffmpeg-filters.html#pad


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 166. palettegen

> https://ffmpeg.org/ffmpeg-filters.html#palettegen


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 167. paletteuse

> https://ffmpeg.org/ffmpeg-filters.html#paletteuse


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 168. perspective

> https://ffmpeg.org/ffmpeg-filters.html#perspective


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 169. phase

> https://ffmpeg.org/ffmpeg-filters.html#phase


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 170. photosensitivity

> https://ffmpeg.org/ffmpeg-filters.html#photosensitivity


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 171. pixdesctest

> https://ffmpeg.org/ffmpeg-filters.html#pixdesctest


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 172. pixscope

> https://ffmpeg.org/ffmpeg-filters.html#pixscope


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 173. pp

> https://ffmpeg.org/ffmpeg-filters.html#pp


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 174. pp7

> https://ffmpeg.org/ffmpeg-filters.html#pp7


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 175. premultiply

> https://ffmpeg.org/ffmpeg-filters.html#premultiply


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 176. prewitt

> https://ffmpeg.org/ffmpeg-filters.html#prewitt


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 177. pseudocolor

> https://ffmpeg.org/ffmpeg-filters.html#pseudocolor


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 178. psnr

> https://ffmpeg.org/ffmpeg-filters.html#psnr


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 179. pullup

> https://ffmpeg.org/ffmpeg-filters.html#pullup


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 180. qp

> https://ffmpeg.org/ffmpeg-filters.html#qp


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 181. random

> https://ffmpeg.org/ffmpeg-filters.html#random


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 182. readeia608

> https://ffmpeg.org/ffmpeg-filters.html#readeia608


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 183. readvitc

> https://ffmpeg.org/ffmpeg-filters.html#readvitc


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 184. remap

> https://ffmpeg.org/ffmpeg-filters.html#remap


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 185. removegrain

> https://ffmpeg.org/ffmpeg-filters.html#removegrain


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 186. removelogo

> https://ffmpeg.org/ffmpeg-filters.html#removelogo


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 187. repeatfields

> https://ffmpeg.org/ffmpeg-filters.html#repeatfields


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 188. reverse

> https://ffmpeg.org/ffmpeg-filters.html#reverse


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 189. rgbashift

> https://ffmpeg.org/ffmpeg-filters.html#rgbashift


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 190. roberts

> https://ffmpeg.org/ffmpeg-filters.html#roberts


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 191. rotate

> https://ffmpeg.org/ffmpeg-filters.html#rotate


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 192. sab

> https://ffmpeg.org/ffmpeg-filters.html#sab


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 193. scale

> https://ffmpeg.org/ffmpeg-filters.html#scale


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 194. scale_npp

> https://ffmpeg.org/ffmpeg-filters.html#scale_npp


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 195. scale2ref

> https://ffmpeg.org/ffmpeg-filters.html#scale2ref


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 196. scroll

> https://ffmpeg.org/ffmpeg-filters.html#scroll


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 197. scdet

> https://ffmpeg.org/ffmpeg-filters.html#scdet


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 198. selectivecolor

> https://ffmpeg.org/ffmpeg-filters.html#selectivecolor


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 199. separatefields

> https://ffmpeg.org/ffmpeg-filters.html#separatefields


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 200. setdar

> https://ffmpeg.org/ffmpeg-filters.html#setdar


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 201. setsar

> https://ffmpeg.org/ffmpeg-filters.html#setsar


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 202. setfield

> https://ffmpeg.org/ffmpeg-filters.html#setfield


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 203. setparams

> https://ffmpeg.org/ffmpeg-filters.html#setparams


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 204. shear

> https://ffmpeg.org/ffmpeg-filters.html#shear


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 205. showinfo

> https://ffmpeg.org/ffmpeg-filters.html#showinfo


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 206. showpalette

> https://ffmpeg.org/ffmpeg-filters.html#showpalette


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 207. shuffleframes

> https://ffmpeg.org/ffmpeg-filters.html#shuffleframes


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 208. shufflepixels

> https://ffmpeg.org/ffmpeg-filters.html#shufflepixels


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 209. shuffleplanes

> https://ffmpeg.org/ffmpeg-filters.html#shuffleplanes


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 210. signalstats

> https://ffmpeg.org/ffmpeg-filters.html#signalstats


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 211. signature

> https://ffmpeg.org/ffmpeg-filters.html#signature


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 212. smartblur

> https://ffmpeg.org/ffmpeg-filters.html#smartblur


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 213. sobel

> https://ffmpeg.org/ffmpeg-filters.html#sobel


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 214. spp

> https://ffmpeg.org/ffmpeg-filters.html#spp


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 215. sr

> https://ffmpeg.org/ffmpeg-filters.html#sr


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 216. ssim

> https://ffmpeg.org/ffmpeg-filters.html#ssim


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 217. stereo3d

> https://ffmpeg.org/ffmpeg-filters.html#stereo3d


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 218. streamselect

> https://ffmpeg.org/ffmpeg-filters.html#streamselect


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 219. astreamselect

> https://ffmpeg.org/ffmpeg-filters.html#astreamselect


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 220. subtitles

> https://ffmpeg.org/ffmpeg-filters.html#subtitles


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 221. super2xsai

> https://ffmpeg.org/ffmpeg-filters.html#super2xsai


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 222. swaprect

> https://ffmpeg.org/ffmpeg-filters.html#swaprect


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 223. swapuv

> https://ffmpeg.org/ffmpeg-filters.html#swapuv


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 224. tblend

> https://ffmpeg.org/ffmpeg-filters.html#tblend


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 225. telecine

> https://ffmpeg.org/ffmpeg-filters.html#telecine


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 226. thistogram

> https://ffmpeg.org/ffmpeg-filters.html#thistogram


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 227. threshold

> https://ffmpeg.org/ffmpeg-filters.html#threshold


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 228. thumbnail

> https://ffmpeg.org/ffmpeg-filters.html#thumbnail


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 229. tile

> https://ffmpeg.org/ffmpeg-filters.html#tile


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 230. tinterlace

> https://ffmpeg.org/ffmpeg-filters.html#tinterlace


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 231. tmedian

> https://ffmpeg.org/ffmpeg-filters.html#tmedian


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 232. tmidequalizer

> https://ffmpeg.org/ffmpeg-filters.html#tmidequalizer


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 233. tmix

> https://ffmpeg.org/ffmpeg-filters.html#tmix


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 234. tonemap

> https://ffmpeg.org/ffmpeg-filters.html#tonemap


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 235. tpad

> https://ffmpeg.org/ffmpeg-filters.html#tpad


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 236. transpose

> https://ffmpeg.org/ffmpeg-filters.html#transpose


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 237. transpose_npp

> https://ffmpeg.org/ffmpeg-filters.html#transpose_npp


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 238. trim

> https://ffmpeg.org/ffmpeg-filters.html#trim


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 239. unpremultiply

> https://ffmpeg.org/ffmpeg-filters.html#unpremultiply


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 240. unsharp

> https://ffmpeg.org/ffmpeg-filters.html#unsharp


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 241. untile

> https://ffmpeg.org/ffmpeg-filters.html#untile


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 242. uspp

> https://ffmpeg.org/ffmpeg-filters.html#uspp


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 243. v360

> https://ffmpeg.org/ffmpeg-filters.html#v360


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 244. vaguedenoiser

> https://ffmpeg.org/ffmpeg-filters.html#vaguedenoiser


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 245. vectorscope

> https://ffmpeg.org/ffmpeg-filters.html#vectorscope


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 246. vidstabdetect

> https://ffmpeg.org/ffmpeg-filters.html#vidstabdetect


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 247. vidstabtransform

> https://ffmpeg.org/ffmpeg-filters.html#vidstabtransform


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 248. vflip

> https://ffmpeg.org/ffmpeg-filters.html#vflip


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 249. vfrdet

> https://ffmpeg.org/ffmpeg-filters.html#vfrdet


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 250. vibrance

> https://ffmpeg.org/ffmpeg-filters.html#vibrance


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 251. vif

> https://ffmpeg.org/ffmpeg-filters.html#vif


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 252. vignette

> https://ffmpeg.org/ffmpeg-filters.html#vignette


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 253. vmafmotion

> https://ffmpeg.org/ffmpeg-filters.html#vmafmotion


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 254. vstack

> https://ffmpeg.org/ffmpeg-filters.html#vstack


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 255. w3fdif

> https://ffmpeg.org/ffmpeg-filters.html#w3fdif


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 256. waveform

> https://ffmpeg.org/ffmpeg-filters.html#waveform


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 257. weave

> https://ffmpeg.org/ffmpeg-filters.html#weave


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 258. doubleweave

> https://ffmpeg.org/ffmpeg-filters.html#doubleweave


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 259. xbr

> https://ffmpeg.org/ffmpeg-filters.html#xbr


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 260. xfade

> https://ffmpeg.org/ffmpeg-filters.html#xfade


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 261. xmedian

> https://ffmpeg.org/ffmpeg-filters.html#xmedian


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 262. xstack

> https://ffmpeg.org/ffmpeg-filters.html#xstack


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 263. yadif

> https://ffmpeg.org/ffmpeg-filters.html#yadif


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 264. yadif_cuda

> https://ffmpeg.org/ffmpeg-filters.html#yadif_cuda


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 265. yaepblur

> https://ffmpeg.org/ffmpeg-filters.html#yaepblur


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 266. zoompan

> https://ffmpeg.org/ffmpeg-filters.html#zoompan


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 267. zscale

> https://ffmpeg.org/ffmpeg-filters.html#zscale


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 268. avgblur_opencl

> https://ffmpeg.org/ffmpeg-filters.html#avgblur_opencl


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 269. boxblur_opencl

> https://ffmpeg.org/ffmpeg-filters.html#boxblur_opencl


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 270. colorkey_opencl

> https://ffmpeg.org/ffmpeg-filters.html#colorkey_opencl


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 271. convolution_opencl

> https://ffmpeg.org/ffmpeg-filters.html#convolution_opencl


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 272. erosion_opencl

> https://ffmpeg.org/ffmpeg-filters.html#erosion_opencl


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 273. deshake_opencl

> https://ffmpeg.org/ffmpeg-filters.html#deshake_opencl


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 274. dilation_opencl

> https://ffmpeg.org/ffmpeg-filters.html#dilation_opencl


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 275. nlmeans_opencl

> https://ffmpeg.org/ffmpeg-filters.html#nlmeans_opencl


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 276. overlay_opencl

> https://ffmpeg.org/ffmpeg-filters.html#overlay_opencl


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 277. pad_opencl

> https://ffmpeg.org/ffmpeg-filters.html#pad_opencl


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 278. prewitt_opencl

> https://ffmpeg.org/ffmpeg-filters.html#prewitt_opencl


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 279. program_opencl

> https://ffmpeg.org/ffmpeg-filters.html#program_opencl


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 280. roberts_opencl

> https://ffmpeg.org/ffmpeg-filters.html#roberts_opencl


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 281. sobel_opencl

> https://ffmpeg.org/ffmpeg-filters.html#sobel_opencl


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 282. tonemap_opencl

> https://ffmpeg.org/ffmpeg-filters.html#tonemap_opencl


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 283. unsharp_opencl

> https://ffmpeg.org/ffmpeg-filters.html#unsharp_opencl


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 284. xfade_opencl

> https://ffmpeg.org/ffmpeg-filters.html#xfade_opencl


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 285. tonemap_vaapi

> https://ffmpeg.org/ffmpeg-filters.html#tonemap_vaapi


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 286. buffer

> https://ffmpeg.org/ffmpeg-filters.html#buffer


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 287. cellauto

> https://ffmpeg.org/ffmpeg-filters.html#cellauto


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 288. coreimagesrc

> https://ffmpeg.org/ffmpeg-filters.html#coreimagesrc


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 289. gradients

> https://ffmpeg.org/ffmpeg-filters.html#gradients


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 290. mandelbrot

> https://ffmpeg.org/ffmpeg-filters.html#mandelbrot


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 291. mptestsrc

> https://ffmpeg.org/ffmpeg-filters.html#mptestsrc


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 292. frei0r_src

> https://ffmpeg.org/ffmpeg-filters.html#frei0r_src


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 293. life

> https://ffmpeg.org/ffmpeg-filters.html#life


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 294. allrgb

> https://ffmpeg.org/ffmpeg-filters.html#allrgb


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 295. allyuv

> https://ffmpeg.org/ffmpeg-filters.html#allyuv


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 296. color

> https://ffmpeg.org/ffmpeg-filters.html#color


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 297. haldclutsrc

> https://ffmpeg.org/ffmpeg-filters.html#haldclutsrc


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 298. nullsrc

> https://ffmpeg.org/ffmpeg-filters.html#nullsrc


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 299. pal75bars

> https://ffmpeg.org/ffmpeg-filters.html#pal75bars


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 300. pal100bars

> https://ffmpeg.org/ffmpeg-filters.html#pal100bars


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 301. rgbtestsrc

> https://ffmpeg.org/ffmpeg-filters.html#rgbtestsrc


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 302. smptebars

> https://ffmpeg.org/ffmpeg-filters.html#smptebars


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 303. smptehdbars

> https://ffmpeg.org/ffmpeg-filters.html#smptehdbars


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 304. testsrc

> https://ffmpeg.org/ffmpeg-filters.html#testsrc


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 305. testsrc2

> https://ffmpeg.org/ffmpeg-filters.html#testsrc2


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 306. yuvtestsrc

> https://ffmpeg.org/ffmpeg-filters.html#yuvtestsrc


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 307. openclsrc

> https://ffmpeg.org/ffmpeg-filters.html#openclsrc


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 308. sierpinski

> https://ffmpeg.org/ffmpeg-filters.html#sierpinski


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 309. buffersink

> https://ffmpeg.org/ffmpeg-filters.html#buffersink


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 310. nullsink

> https://ffmpeg.org/ffmpeg-filters.html#nullsink


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 311. abitscope

> https://ffmpeg.org/ffmpeg-filters.html#abitscope


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 312. adrawgraph

> https://ffmpeg.org/ffmpeg-filters.html#adrawgraph


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 313. agraphmonitor

> https://ffmpeg.org/ffmpeg-filters.html#agraphmonitor


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 314. ahistogram

> https://ffmpeg.org/ffmpeg-filters.html#ahistogram


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 315. aphasemeter

> https://ffmpeg.org/ffmpeg-filters.html#aphasemeter


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 316. avectorscope

> https://ffmpeg.org/ffmpeg-filters.html#avectorscope


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 317. bench

> https://ffmpeg.org/ffmpeg-filters.html#bench


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 318. abench

> https://ffmpeg.org/ffmpeg-filters.html#abench


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 319. concat

> https://ffmpeg.org/ffmpeg-filters.html#concat


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 320. ebur128

> https://ffmpeg.org/ffmpeg-filters.html#ebur128


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 321. interleave

> https://ffmpeg.org/ffmpeg-filters.html#interleave


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 322. ainterleave

> https://ffmpeg.org/ffmpeg-filters.html#ainterleave


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 323. metadata

> https://ffmpeg.org/ffmpeg-filters.html#metadata


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 324. ametadata

> https://ffmpeg.org/ffmpeg-filters.html#ametadata


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 325. perms

> https://ffmpeg.org/ffmpeg-filters.html#perms


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 326. aperms

> https://ffmpeg.org/ffmpeg-filters.html#aperms


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 327. realtime

> https://ffmpeg.org/ffmpeg-filters.html#realtime


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 328. arealtime

> https://ffmpeg.org/ffmpeg-filters.html#arealtime


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 329. select

> https://ffmpeg.org/ffmpeg-filters.html#select


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 330. aselect

> https://ffmpeg.org/ffmpeg-filters.html#aselect


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 331. sendcmd

> https://ffmpeg.org/ffmpeg-filters.html#sendcmd


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 332. asendcmd

> https://ffmpeg.org/ffmpeg-filters.html#asendcmd


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 333. setpts

> https://ffmpeg.org/ffmpeg-filters.html#setpts


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 334. asetpts

> https://ffmpeg.org/ffmpeg-filters.html#asetpts


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 335. setrange

> https://ffmpeg.org/ffmpeg-filters.html#setrange


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 336. settb

> https://ffmpeg.org/ffmpeg-filters.html#settb


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 337. asettb

> https://ffmpeg.org/ffmpeg-filters.html#asettb


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 338. showcqt

> https://ffmpeg.org/ffmpeg-filters.html#showcqt


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 339. showfreqs

> https://ffmpeg.org/ffmpeg-filters.html#showfreqs


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 340. showspatial

> https://ffmpeg.org/ffmpeg-filters.html#showspatial


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 341. showspectrum

> https://ffmpeg.org/ffmpeg-filters.html#showspectrum


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 342. showspectrumpic

> https://ffmpeg.org/ffmpeg-filters.html#showspectrumpic


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 343. showvolume

> https://ffmpeg.org/ffmpeg-filters.html#showvolume


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 344. showwaves

> https://ffmpeg.org/ffmpeg-filters.html#showwaves


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 345. showwavespic

> https://ffmpeg.org/ffmpeg-filters.html#showwavespic


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 346. sidedata

> https://ffmpeg.org/ffmpeg-filters.html#sidedata


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 347. asidedata

> https://ffmpeg.org/ffmpeg-filters.html#asidedata


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 348. spectrumsynth

> https://ffmpeg.org/ffmpeg-filters.html#spectrumsynth


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 349. split

> https://ffmpeg.org/ffmpeg-filters.html#split


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 350. asplit

> https://ffmpeg.org/ffmpeg-filters.html#asplit


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 351. zmq

> https://ffmpeg.org/ffmpeg-filters.html#zmq


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 352. azmq

> https://ffmpeg.org/ffmpeg-filters.html#azmq


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 353. amovie

> https://ffmpeg.org/ffmpeg-filters.html#amovie


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]

## 354. movie

> https://ffmpeg.org/ffmpeg-filters.html#movie


### 参数


### 示例

```python

```

```

```

#### 对比

[视频对比链接]
