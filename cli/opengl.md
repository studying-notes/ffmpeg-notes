---
date: 2021-01-11T16:13:17+08:00  # 创建日期
author: "Rustle Karl"  # 作者

# 文章
title: "FFmpeg OpenGL 转场特效"  # 文章标题
url:  "posts/ffmpeg/cli/opengl"  # 设置网页链接，默认使用文件名
tags: [ "ffmpeg", "opengl" ]  # 自定义标签
series: [ "FFmpeg 从入门到放弃"]  # 文章主题/文章系列
categories: [ "学习笔记"]  # 文章分类

# 章节
weight: 20 # 排序优先级
chapter: false  # 设置为章节

index: true  # 是否可以被索引
toc: true  # 是否自动生成目录
draft: false  # 草稿
---

## 视频转场特效

一共有70多种。。。

### 两个视频合成

```shell
ffmpeg -i media/0.mp4 -i media/1.mp4 -filter_complex gltransition -y out.mp4
```

```shell
ffmpeg -i media/0.mp4 -i media/1.mp4 -filter_complex gltransition=offset=1:duration=1 -y out.mp4
```

- **offset** 指定转场效果在 1s 开始
- **duration** 转场效果持续时间 1s

### 三个以上视频合成

nohup Xvfb :1 -screen 0 1280x1024x16 &
env DISPLAY=:1 

统一命名方法：

```shell
ffmpeg -hide_banner \
  -i media/0.mp4 \
  -i media/1.mp4 \
  -i media/2.mp4 \
  -filter_complex " \
    [0:v]split[v00split1][v00split2]; \
    [1:v]split[v01split1][v01split2]; \
    [2:v]split[v02split1][v02split2]; \
    [v00split1]trim=0:3[v00trim1]; \
    [v00split2]trim=3:4[v00trim2]; \
    [v00trim2]setpts=PTS-STARTPTS[v00trim2setpts]; \
    [v01split1]trim=0:3[v01trim1]; \
    [v01split2]trim=3:4[v01trim2]; \
    [v01trim2]setpts=PTS-STARTPTS[v01trim2setpts]; \
    [v02split1]trim=0:3[v02trim1]; \
    [v02split2]trim=3:4[v02trim2]; \
    [v02trim2]setpts=PTS-STARTPTS[v02trim2setpts]; \
    [v00trim2setpts][v01trim1]gltransition=duration=1:source=transitions/crosswarp.glsl[vt0]; \
    [v01trim2setpts][v02trim1]gltransition=duration=1[vt1]; \
    [v00trim1][vt0][vt1][v02trim2setpts]concat=n=4[outv]" \
  -map "[outv]" \
  -c:v libx264 -profile:v baseline -preset slow -movflags faststart -pix_fmt yuv420p \
  -y out.mp4
```

## 图片合成带转场效果视频

### 指定转场时间和转场特效

```shell
ffmpeg -t 1 -loop 1 -i image/1.jpg -t 1 -loop 1 -i image/2.jpg -filter_complex "gltransition=offset=0.5:duration=0.5:source=transitions/crosswarp.glsl" -y output/crosswarp.mp4
```

> 合成视频的时长为第二个视频的时长

- **-t**
  图片转为视频的持续时间
- **-loop**
  图片循环，如果设置为1，则循环输入，预设值为0。
- **-i**
  输入图片
- **-t 1 -loop 1 -i input0.jpg**
  整体意思即为图片循环生成持续1秒的视频
- **-filter_complex**
  过滤器
- **gltransition**
  指定使用 ffmpeg-gl-transitions 的过滤器
- **offset**
  （浮点型，默认 0）指定转场效果在 0.5s 开始
- **duration**
  （浮点型，默认 1）转场效果持续时间
- **source**
  转场效果指定的文件的路径，注意一定要使用全路径，~ 路径识别不了，转场文件可以从 gl-transitions 获取，如果不指定 source，则转场效果为默认的淡入淡出效果。
- **-y**
  如果输出文件已存在则覆盖

## 多图片合成带音乐的、时长叠加的视频

```shell
ffmpeg \
-t 3 -loop 1 -i image/1.jpg \
-t 3 -loop 1 -i image/2.jpg \
-t 2 -loop 1 -i image/3.jpg \
-stream_loop -1 -i media/audio.mp3 -acodec aac \
-filter_complex "\
[0]split[v_sp_0_0][v_sp_0_1];[v_sp_0_0]trim=0:2[v_tr_0_0];[v_sp_0_1]trim=2:3[v_tr_0_1];[v_tr_0_1]setpts=PTS-STARTPTS[v_st_0];\
[1]split[v_sp_1_0][v_sp_1_1];[v_sp_1_0]trim=0:2[v_tr_1_0];[v_sp_1_1]trim=2:3[v_tr_1_1];[v_tr_1_1]setpts=PTS-STARTPTS[v_st_1];\
[v_st_0][v_tr_1_0]gltransition=duration=1:source=transitions/SimpleZoom.glsl[v0];\
[v_st_1][2]gltransition=duration=1:source=transitions/CrossZoom.glsl[v1];\
[v_tr_0_0][v0][v1]concat=n=3[v];\
afade=t=out:st=5:d=2" \
-map "[v]" \
-map "3:a" \
-t 7 \
-shortest \
-c:v libx264 \
-profile:v main \
-pix_fmt yuv420p \
-preset fast \
-y \
tripartite.mp4
```

- **-stream_loop**
  输入流循环的次数，0 表示无循环，-1 表示无限循环，即音乐循环播放。
- **-acodec aac**
  设置音频编解码为 acc 模式
- **[0]split[v_sp_0_0][v_sp_0_1];[v_sp_0_0]trim=0:2.0[v_tr_0_0];[v_sp_0_1]trim=2.0:3[v_tr_0_1];[v_tr_0_1]setpts=PTS-STARTPTS[v_st_0]**
  将第一个视频拆分为 0-2s 和 2s-3s 两个视频；帧是从每个输入视频按时间戳顺序获取的，因此，如果它们的初始时间戳不同，则最好将两个输入通过 setpts = PTS-STARTPTS 过滤器传递，以使它们以相同的0时间戳开始，即为了让 2s-3s 的视频和第二个视频合并作为转场视频，需要将 2s-3s 视频的初始时间戳设为 0。
- **[v_st_0][v_tr_1_0]gltransition=duration=1:source=/home/jl/ffmpeg-gl-transition/gl-transitions/transitions/SimpleZoom.glsl[v0]**
  将第一个视频拆分出来的 2s-3s 的视频与第二个视频的初始视频合并生成时长 1s 的转场视频。
- **[v_tr_0_0][v0][v1]concat=n=3[v]**
  合并视频为一个完整的输出视频
- **afade=t=out:st=5:d=2**
  设置从 5s 开始，持续 2s 的音乐淡出效果。转场时间会合并到下一个视频的初始时间中，所以合成视频的时长为 2+2+2=6s，视频在播放时最后 1-2s 通常就黑屏了，所以视频时长加 1s。
- **-map "[v]"**
  将合成的视频输入流 v 指定为输出文件的源
- **-map "3:a"**
  将第四个文件作为视频音频文件
- **-t 7**
  输出视频时长为 7s
- **shortest**
  最短的输入流结束时，完成编码。
- **-c:v libx264**
  输出视频编码格式
- **-profile:v main**
  指定视频画质，baseline：基本画质，extended：进阶画质，main：主流化之，high：高级画质。
- **-pix_fmt yuv420p**
  设置像素格式为 yuv420p
- **-preset fast**
  调节编码速度和质量的平衡，有ultrafast、superfast、veryfast、faster、fast、medium、slow、slower、veryslow、placebo这10个选项，编码速度从快到慢，视频质量越来越好。

### 合成带背景图的视频

如上背景图为 1080x1920 尺寸的，中间的透明区域为 1080x720 的，当我们需要将上面的图片合成的视频以如下背景图作为背景的时候，那么我们的视频尺寸就必须缩放为 1080x720 的视频，我们当然可以通过 -s 1080x720 来缩放上面生成的视频，但是会存在视频变形的情况，我们可以通过如下命令来等比例缩放图片从而一步到位生成带背景的视频。

```shell
ffmpeg -hide_banner \
-t 3 -loop 1 -i input0.jpg \
-t 3 -loop 1 -i input1.jpg \
-t 2 -loop 1 -i input2.jpg \
-i bg.png \
-stream_loop -1 -i music.mp3 -acodec aac \
-filter_complex "\
[0]scale=min(iw*720/ih\,1080):min(720\,ih*1080/iw),pad=1080:720:(1080-iw)/2:(720-ih)/2[p0];\
[1]scale=min(iw*720/ih\,1080):min(720\,ih*1080/iw),pad=1080:720:(1080-iw)/2:(720-ih)/2[p1];\
[2]scale=min(iw*720/ih\,1080):min(720\,ih*1080/iw),pad=1080:720:(1080-iw)/2:(720-ih)/2[p2];\
[p0]split[v_sp_0_0][v_sp_0_1];[v_sp_0_0]trim=0:2[v_tr_0_0];[v_sp_0_1]trim=2:3[v_tr_0_1];[v_tr_0_1]setpts=PTS-STARTPTS[v_st_0];\
[p1]split[v_sp_1_0][v_sp_1_1];[v_sp_1_0]trim=0:2[v_tr_1_0];[v_sp_1_1]trim=2:3[v_tr_1_1];[v_tr_1_1]setpts=PTS-STARTPTS[v_st_1];\
[v_st_0][v_tr_1_0]gltransition=duration=1:source=/home/jl/ffmpeg-gl-transition/gl-transitions/transitions/SimpleZoom.glsl[v0];\
[v_st_1][p2]gltransition=duration=1:source=/home/jl/ffmpeg-gl-transition/gl-transitions/transitions/CrossZoom.glsl[v1];\
[v_tr_0_0][v0][v1]concat=n=3[out];\
[3][out]overlay=0:600[v];\
afade=t=out:st=5:d=2" \
-map "[v]" \
-map "4:a" \
-t 7 \
-shortest \
-c:v libx264 \
-profile:v main \
-pix_fmt yuv420p \
-preset fast \
-y \
out.mp4
```

- **-i bg.png**
  指定背景图
- **[0]scale=min(iw\*720/ih\,1080):min(720\,ih\*1080/iw),pad=1080:720:(1080-iw)/2:(720-ih)/2[p0]**
  等比例缩放为 1080x720 的居中图片，`pad`指定尺寸不足的地方以黑色背景填充。
- **缩放扩展：scale=1080:720:force_original_aspect_ratio=decrease,pad=1080:720:(ow-iw)/2:(oh-ih)/2**
  `force_original_aspect_ratio`参数，`decrease`指定图片缩小，`increase`扩大。
- **[3][out]overlay=0:600[v]**
  将合成的视频覆盖到第四个输入文件（即背景图）上，x 轴位移 0，y 轴位移 600（(1920-720)/2=600）。

### 合成带高斯模糊背景图的视频

思路为先将图片合成高斯模糊背景视频，然后将生成的视频覆盖到背景视频上。原图为 1200x800，要想生成 3 的效果，就要将当前视频的图片缩放为 2880x1920，然后截取中间的 1080x1920 部分。下面命令前 3 个输入图片是作为高斯背景合成视频使用的。

```shell
 ffmpeg -hide_banner \
-t 2 -loop 1 -i input0.jpg \
-t 2 -loop 1 -i input1.jpg \
-t 2 -loop 1 -i input2.jpg \
-t 3 -loop 1 -i input0.jpg \
-t 3 -loop 1 -i input1.jpg \
-t 2 -loop 1 -i input2.jpg \
-stream_loop -1 -i music.mp3 -acodec aac \
-filter_complex "\
[0][1][2]concat=n=3[bout];\
[bout]scale=2880:1920[bv0];\
[bv0]crop=1080:1920:900:0[bv1];\
[bv1]avgblur=sizeX=90[bv2];\
[bv2]eq=contrast=1:brightness=-0.1[bv];\
[3]scale=min(iw*720/ih\,1080):min(720\,ih*1080/iw),pad=1080:720:(1080-iw)/2:(720-ih)/2[p0];\
[4]scale=min(iw*720/ih\,1080):min(720\,ih*1080/iw),pad=1080:720:(1080-iw)/2:(720-ih)/2[p1];\
[5]scale=min(iw*720/ih\,1080):min(720\,ih*1080/iw),pad=1080:720:(1080-iw)/2:(720-ih)/2[p2];\
[p0]split[v_sp_0_0][v_sp_0_1];[v_sp_0_0]trim=0:2[v_tr_0_0];[v_sp_0_1]trim=2:3[v_tr_0_1];[v_tr_0_1]setpts=PTS-STARTPTS[v_st_0];\
[p1]split[v_sp_1_0][v_sp_1_1];[v_sp_1_0]trim=0:2[v_tr_1_0];[v_sp_1_1]trim=2:3[v_tr_1_1];[v_tr_1_1]setpts=PTS-STARTPTS[v_st_1];\
[v_st_0][v_tr_1_0]gltransition=duration=1:source=/home/jl/ffmpeg-gl-transition/gl-transitions/transitions/SimpleZoom.glsl[v0];\
[v_st_1][p2]gltransition=duration=1:source=/home/jl/ffmpeg-gl-transition/gl-transitions/transitions/LinearBlur.glsl[v1];\
[v_tr_0_0][v0][v1]concat=n=3[out];\
[bv][out]overlay=0:600[v];\
afade=t=out:st=5:d=2" \
-map "[v]" \
-map "6:a" \
-t 7.0 \
-shortest \
-c:v libx264 \
-profile:v main \
-pix_fmt yuv420p \
-preset fast \
-y \
out.mp4
```

- **[bout]scale=2880:1920[bv0]**
  将前三个视频合成的视频缩放为 2880x1920
- **[bv0]crop=1080:1920:900:0[bv1]**
  2880x1920 的视频截取 x 轴位移 900，y 轴位移 0 的中间的 1080x1920 部分。
- **[bv1]avgblur=sizeX=90[bv2]**
  均值模糊，sizeX 的值越大越模糊，即产生高斯模糊效果，默认值 0。
- **[bv2]eq=contrast=1:brightness=-0.1[bv]**
  降低高斯模糊背景亮度，突出中间的视频。

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







