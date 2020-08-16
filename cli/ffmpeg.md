# FFmpeg 命令详解

- [FFmpeg 命令详解](#ffmpeg-命令详解)
  - [命令基本格式及参数](#命令基本格式及参数)
    - [FFmpeg 命令参数](#ffmpeg-命令参数)
  - [分解与复用](#分解与复用)
    - [复制流](#复制流)
    - [显示流信息](#显示流信息)
    - [分离视频流/音频流](#分离视频流音频流)
    - [文件格式转换](#文件格式转换)
      - [将一张 RGBA 格式表示的数据转换为 JPEG 格式的图片](#将一张-rgba-格式表示的数据转换为-jpeg-格式的图片)
      - [将一个 YUV 格式表示的数据转换为 JPEG 格式的图片](#将一个-yuv-格式表示的数据转换为-jpeg-格式的图片)
      - [编码格式控制](#编码格式控制)
      - [维持源文件的质量](#维持源文件的质量)
    - [音视频合成](#音视频合成)
      - [无声视频和音频合成](#无声视频和音频合成)
      - [替换有声视频中的音频](#替换有声视频中的音频)
      - [保留视频原声，添加背景音乐](#保留视频原声添加背景音乐)
  - [更改文件输入输出属性](#更改文件输入输出属性)
    - [设置视频的屏幕显式比例](#设置视频的屏幕显式比例)
    - [设置输出文件比特率](#设置输出文件比特率)
    - [强制输出文件的帧频](#强制输出文件的帧频)
    - [强制输入文件的帧频](#强制输入文件的帧频)
  - [处理原始数据](#处理原始数据)
    - [提取 YUV 数据](#提取-yuv-数据)
    - [YUV 转 H264](#yuv-转-h264)
    - [提取 PCM 数据](#提取-pcm-数据)
    - [PCM 转 WAV](#pcm-转-wav)
  - [滤镜](#滤镜)
    - [简单滤镜](#简单滤镜)
      - [添加水印](#添加水印)
      - [删除水印](#删除水印)
      - [更改视频分辨率](#更改视频分辨率)
      - [视频裁剪](#视频裁剪)
      - [压缩音视频文件](#压缩音视频文件)
        - [压缩视频](#压缩视频)
          - [改变视频的比特率和编码](#改变视频的比特率和编码)
          - [改变视频的帧率](#改变视频的帧率)
          - [改变视频的分辨率](#改变视频的分辨率)
      - [音视频变速](#音视频变速)
      - [调节音量](#调节音量)
        - [倍数调节](#倍数调节)
        - [分贝调节](#分贝调节)
    - [复杂滤镜](#复杂滤镜)
      - [倍速播放](#倍速播放)
      - [对称视频](#对称视频)
      - [画中画](#画中画)
      - [录制画中画](#录制画中画)
      - [多路视频拼接](#多路视频拼接)
      - [滤镜加水印](#滤镜加水印)
    - [特别的滤镜](#特别的滤镜)
      - [镜像](#镜像)
      - [复古](#复古)
      - [光晕](#光晕)
      - [变暗](#变暗)
      - [增加对比度](#增加对比度)
      - [降噪](#降噪)
      - [强对比度](#强对比度)
      - [变亮](#变亮)
      - [锐化](#锐化)
      - [低通滤波](#低通滤波)
      - [高通滤波](#高通滤波)
      - [边缘检测](#边缘检测)
      - [底片](#底片)
      - [彩色底片](#彩色底片)
  - [音视频的拼接与裁剪](#音视频的拼接与裁剪)
    - [裁剪](#裁剪)
      - [截取音频片段](#截取音频片段)
      - [截取视频片段](#截取视频片段)
      - [将视频分为多个部分](#将视频分为多个部分)
    - [视频拼接](#视频拼接)
      - [方法一](#方法一)
      - [方法二](#方法二)
    - [音频拼接](#音频拼接)
    - [音频混音](#音频混音)
      - [amerge](#amerge)
      - [amix](#amix)
  - [视频图像相互转换](#视频图像相互转换)
    - [视频转换为 JPEG](#视频转换为-jpeg)
    - [视频转换为 GIF](#视频转换为-gif)
    - [从多张图片创建视频](#从多张图片创建视频)
  - [直播相关](#直播相关)
    - [推流](#推流)
    - [拉流保存](#拉流保存)
    - [转流](#转流)
    - [实时推流](#实时推流)
  - [录屏操作](#录屏操作)
    - [列出设备](#列出设备)

## 命令基本格式及参数

```shell
ffmpeg [global_options] {[input_file_options] -i input_file} ...
						 {[output_file_options] output_file} ...
```

通过 `-i` 选项读取输任意数量的输入文件，可以是常规文件，管道，网络流，抓取设备等，并写入任意数量的输出文件。

原则上，每个输入/输出文件都可以包含任意数量的不同类型的视频流（视频/音频/字幕/附件/数据）。 流的数量和/或类型是由容器格式来限制。 选择从哪个输入进入到哪个输出将自动完成或使用 `-map` 选项。

要引用选项中的输入文件，必须使用它们的索引（从 0 开始）。 例如，第一个输入文件是 0，第二个输入文件是 1，等等。类似地，文件内的流被它们的索引引用。 例如， `2:3` 是指第三个输入文件中的第四个流。

避免混合输入和输出文件，首先指定所有输入文件，然后是所有的输出文件。也不能混用属于不同的文件的选项，所有**选项仅适用于下一个输入或输出文件**，之后选项将被重置。

### FFmpeg 命令参数

详细内容见独立[文档](args.md)。

## 分解与复用

### 复制流

流拷贝是通过将 `copy` 参数提供给 `-codec` 选项来选择流的模式。它使得 ffmpeg 省略了指定流的解码和编码步骤，所以它只能进行多路分解和多路复用。 这对于更改容器格式或修改容器级元数据很有用。 在这种情况下，上图将简化为：

```
 _______              ______________            ________
|       |            |              |          |        |
| input |  demuxer   | encoded data |  muxer   | output |
| file  | ---------> | packets      | -------> | file   |
|_______|            |______________|          |________|
```

由于没有解码或编码，速度非常快，没有质量损失。 但是，由于许多因素，在某些情况下可能无法正常工作。 应用过滤器显然也是不可能的，因为滤镜仅能作用在未压缩的数据上。

### 显示流信息

```shell
ffmpeg -i  video.mp4
```

```
# 视频流
Stream #0:0(und): Video: h264 (Main) (avc1 / 0x31637661), yuv420p, 960x516 [SAR 1:1 DAR 80:43], 238 kb/s, 30 fps, 30 tbr, 15360 tbn, 60 tbc (default)

# 音频流
Stream #0:1(eng): Audio: aac (LC) (mp4a / 0x6134706D), 44100 Hz, stereo, fltp, 192 kb/s (default)
```

### 分离视频流/音频流

```shell
# 分离视频流
ffmpeg -i  input.mp4 -vcodec copy -an output.mp4

# 分离音频流
ffmpeg -i  video.mp4 -acodec copy -vn audio.m4a

# AAC 格式
ffmpeg -i  video.mp4 -acodec aac -vn audio.aac
ffmpeg -i  video.mp4 -vn audio.aac
ffmpeg -i  video.mp4 audio.aac
```

- `-vn` : 不处理视频
- `-an` : 不处理音频
- `-ar` : 设定采样率，常用的值是 22050 Hz、44100 Hz、48000 Hz
- `-ab` : 表明音频比特率
- `-ac` : 设定声音的通道数目
- `-acodec` : 设定声音编解码器，未设定时则使用与输入流相同的编解码器；`copy` 指明只拷贝，不做编解码
- `-vcodec` : 设定视频编解码器，未设定时则使用与输入流相同的编解码器 
- `-f` : 指定输出文件的格式

### 文件格式转换

```shell
# 只拷贝，不做编解码，只是改了封装格式
ffmpeg -i out.mp4 -vcodec copy -acodec copy out.flv

# 音频格式转换
ffmpeg -i audio.m4a -acodec aac audio.aac

# 暴力转换
ffmpeg -i audio.m4a audio.mp3

ffmpeg -i 1.mp3 1.aac
ffmpeg -i 2.aac 2.m4a
```

#### 将一张 RGBA 格式表示的数据转换为 JPEG 格式的图片

```shell
ffmpeg -f rawvideo -pix_fmt rgba -s 480*480 -i texture.rgb -f image2 -vcodec mjpeg output.jpg
```

#### 将一个 YUV 格式表示的数据转换为 JPEG 格式的图片

```shell
ffmpeg -f rawvideo -pix_fmt yuv420p -s 480*480 -i texture.yuv -f image2 -vcodec mjpeg output.jpg
```

#### 编码格式控制

对于编码格式，一种方法是通过目标文件的扩展名来控制，另一种方法是通过 ``-c:v`` 参数来控制。

```shell
ffmpeg -i  input.mp4 -c:v libx265  input.avi
```

#### 维持源文件的质量

```shell
ffmpeg -i demo.flv -qscale 0 demo.mp4
```

- `-qscale` 值越低，输出视频的质量越高

### 音视频合成

#### 无声视频和音频合成

```shell
ffmpeg -y -i video_noaudio.mp4 -i audio.mp3  -shortest -vcodec copy -acodec copy av_merge.mp4
```

#### 替换有声视频中的音频

```shell
ffmpeg -y -i video.mp4 -i audio.mp3  -shortest -c:v copy -c:a copy -map 0:v:0 -map 1:a:0 av_merge.mp4
```

#### 保留视频原声，添加背景音乐

```shell
ffmpeg -i  input.mp3 -i  input.mp4 -threads 2 -filter_complex amix=inputs=2:duration=first:dropout_transition=0 merge.mp4 -y
```

> 音频和视频输入流的命令顺序对视频合成有影响，顺序交换可能造成跳帧，另外还有可能声音错位，因为视频中的 AAC 音频流长度不一定和视频长度相同，开头可能是空白，会导致缩进和背景音乐混音，造成声音错位。

## 更改文件输入输出属性

### 设置视频的屏幕显式比例

```shell
ffmpeg -i start.mp4 -aspect 16:9 out-start16_9.mp4
```

- -aspect : 设置视频文件的屏幕显式比例，常用的比例: 16:9, 4:3, 16:10, 2.21:1, 2.35:1, 5:4, 2.39:1

### 设置输出文件比特率

```shell
ffmpeg -i input.avi -b：V 64K -bufsize 64K output.avi
```

### 强制输出文件的帧频

```shell
ffmpeg -i input.avi -r 24 output.avi
```

### 强制输入文件的帧频

仅对原始格式有效，以 1 FPS 读入文件，以每秒 24 帧输出：

```shell
ffmpeg -r 1 -i input.m2v -r 24 output.avi
```

## 处理原始数据

### 提取 YUV 数据

```shell
$ ffmpeg -i input.mp4 -an -c:v rawvideo -pixel_format yuv420p out.yuv

$ du -h out.yuv
530M    out.yuv

# 播放文件
$ ffplay -video_size 1280x720 out.yuv
```

- `-c:v rawvideo` 指定将视频转成原始数据
- `-pixel_format yuv420p` 指定转换格式为 `yuv420p`

### YUV 转 H264

```shell
ffmpeg -f rawvideo -pix_fmt yuv420p -s 320x240 -r 30 -i out.yuv -c:v libx264 -f rawvideo out.h264
```

### 提取 PCM 数据

```shell
ffmpeg -i long.mp3 -vn -ar 44100 -ac 2 -f s16le long.pcm

# 播放文件
ffplay -ar 44100 -ac 2 -f s16le -i long.pcm
```

### PCM 转 WAV

```shell
ffmpeg -f s16be -ar 44100 -ac 2 -acodec pcm_s16be -i long.pcm long.wav
```

## 滤镜

在编码之前，ffmpeg 可以使用 `libavfilter` 库中的过滤器处理原始音频和视频帧。 几个链式过滤器形成一个过滤器图形。 ffmpeg 区分两种类型的过滤器图形：简单和复杂。

### 简单滤镜

简单的过滤器图是那些只有一个输入和输出，都是相同的类型。 在上面的图中，它们可以通过在解码和编码之间插入一个额外的步骤来表示：

```
 _________                        ______________
|         |                      |              |
| decoded |                      | encoded data |
| frames  |\                   _ | packets      |
|_________| \                  /||______________|
             \   __________   /
  simple     _\||          | /  encoder
  filtergraph   | filtered |/
                | frames   |
                |__________|

```

简单的 `filtergraphs` 配置了`per-stream-filter` 选项，分别为视频和音频使用 `-vf` 和 `-af` 别名。 一个简单的视频 `filtergraph` 可以看起来像这样的例子：

```
 _______        _____________        _______        ________
|       |      |             |      |       |      |        |
| input | ---> | deinterlace | ---> | scale | ---> | output |
|_______|      |_____________|      |_______|      |________|

```

某些滤镜会更改帧属性，但不会改变帧内容。

#### 添加水印

```shell
ffmpeg -i out.mp4  -vf "movie=logo.png,scale=64:48[watermask];[in][watermask] overlay=30:10 [out]" water.mp4
```

#### 删除水印

1. 通过 ffplay 找到要删除水印的位置

```shell
ffplay -i test.flv -vf delogo=x=806:y=20:w=70:h=80:show=1
```

1. 用 `delogo` 滤镜删除水印

```shell
ffmpeg -i test.flv -vf delogo=x=806:y=20:w=70:h=80 output.flv
```

#### 更改视频分辨率

```shell
ffmpeg -i out.mp4 -vf scale=iw/2:-1 scale.mp4
```

- -`vf scale` 指定使用简单过滤器 `scale`，`iw/2:-1` 中的 `iw` 指定按整型取视频的宽度。 `-1` 表示高度随宽度一起变化。


```shell
# 设置为 1920:1080 的分辨率
ffmpeg -i input.mp4 -filter:v scale=1920:1080 -c:a copy output.mp4

# 设置为 640:480 的分辨率
ffmpeg -i input.mp4 -filter:v scale=640:480 -c:a copy output.mp4
```

#### 视频裁剪

```shell
ffmpeg -i VR.mov  -vf crop=in_w-200:in_h-200 -c:v libx264 -c:a copy -video_size 1280x720 vr_new.mp4
```

crop 格式：

```
crop=out_w:out_h:x:y
```

- `out_w`: 输出的宽度，可以使用 `in_w` 表式输入视频的宽度
- `out_h`: 输出的高度，可以使用 `in_h` 表式输入视频的高度
- `x` : X 坐标，最左边为 0，默认为源视频的中间位置
- `y` : Y 坐标，最顶部为 0，默认为源视频的中间位置

> 如果 x 和 y 设置为 0，则从左上角开始裁剪，不写则从中心点裁剪。

#### 压缩音视频文件

##### 压缩视频

###### 改变视频的比特率和编码

```shell
ffmpeg -i input.webm -c:a copy -c:v vp9 -b:v 1M output.mkv

ffmpeg -i long.mp4 -c:a copy -b:v 1M long_compress.mp4 -y
```

- -b:v 用于指定视频的比特率

###### 改变视频的帧率

```shell
ffmpeg -i long.mp4 -c:a copy -s hd720 long_compress.mp4

ffmpeg -i long.mp4 -c:a copy -s 1280x720 long_compress.mp4
```

###### 改变视频的分辨率

同时压缩了音频流：

```shell
ffmpeg -i input.mp4 -vf scale=1280:-1 -c:v libx264 -preset veryslow -crf 24 -c:a aac -strict -2 -b:a 128k output.mp4
```

```shell
# 压缩音频
ffmpeg -i input.mp3 -ab 128 output.mp3
```

#### 音视频变速

```shell
# video 加速
ffmpeg -i start.mp4 -filter:v setpts=0.5*PTS slow-start.mp4
# audio 变速
ffmpeg -i input.mp3 -filter:a atempo=2.0 output.mp3
```

#### 调节音量

##### 倍数调节

```shell
# 调节为当前音量的一半
ffmpeg -i input.wav -filter:a "volume=0.5" output.wav

# 调节为当前音量的 1.5 倍
ffmpeg -i input.wav -filter:a "volume=1.5" output.wav

# 调节为静音
ffmpeg -i input.wav -filter:a "volume=0" output.wav
```

##### 分贝调节

```shell
# 增加 10dB
ffmpeg -i input.wav -filter:a "volume=10dB" output.wav

# 减少 5dB
ffmpeg -i input.wav -filter:a "volume=-5dB" output.wav
```

### 复杂滤镜

复杂的过滤器图是那些不能简单描述为应用于一个流的线性处理链的过滤器图。 例如，当图形有多个输入和/或输出，或者当输出流类型与输入不同时。可以用下图来表示：

```
 _________
|         |
| input 0 |\                    __________
|_________| \                  |          |
             \   _________    /| output 0 |
              \ |         |  / |__________|
 _________     \| complex | /
|         |     |         |/
| input 1 |---->| filter  |\
|_________|     |         | \   __________
               /| graph   |  \ |          |
              / |         |   \| output 1 |
 _________   /  |_________|    |__________|
|         | /
| input 2 |/
|_________|

```

复杂的过滤器图使用 `-filter_complex` 选项进行配置。此选项是全局性的，因为复杂的过滤器图形本质上不能与单个流或文件明确关联。

一个复杂的过滤器图的一个简单的例子是覆盖过滤器，它有两个视频输入和一个视频输出，包含一个视频叠加在另一个上面，它的音频对应是 `amix` 滤波器。

#### 倍速播放

```shell
ffmpeg -i out.mp4 -filter_complex "[0:v]setpts=0.5*PTS[v];[0:a]atempo=2.0[a]" -map "[v]" -map "[a]" speed2.0.mp4
```

- `[0:v]` 表示第一个文件的视频作为输入；`setpts=0.5*PTS` 表示每帧视频的 `pts` 时间戳都乘 `0.5` ，也就是差少一半；`[v]` 表示输出的别名。
- `map` 可用于处理复杂输出，如可以将指定的多路流输出到一个输出文件，也可以指定输出到多个文件；`[v]` 复杂滤镜输出的别名作为输出文件的一路流。上文示例的用法是将复杂滤镜输出的视频和音频输出到指定文件中。

#### 对称视频

```shell
ffmpeg  -i out.mp4 -filter_complex "[0:v]pad=w=2*iw[a];[0:v]hflip[b];[a][b]overlay=x=w" duicheng.mp4
```

- `hflip` 水平翻转
- `vflip` 垂直翻转

#### 画中画

```shell
ffmpeg -i out.mp4 -i out1.mp4 -filter_complex "[1:v]scale=w=176:h=144:force_original_aspect_ratio=decrease[ckout];[0:v][ckout]overlay=x=W-w-10:y=0[out]" -map "[out]" -movflags faststart new.mp4
```

#### 录制画中画

```shell
ffmpeg  -f avfoundation -i "1" -framerate 30 -f avfoundation -i "0:0" -r 30 -c:v libx264 -preset ultrafast -c:a libfdk_aac -profile:a aac_he_v2 -ar 44100 -ac 2 -filter_complex "[1:v]scale=w=176:h=144:force_original_aspect_ratio=decrease[a];[0:v][a]overlay=x=W-w-10:y=0[out]" -map "[out]" -movflags faststart -map 1:a b.mp4
```

#### 多路视频拼接

```shell
ffmpeg  -f avfoundation -i "1" -framerate 30 -f avfoundation   -i "0:0" -r 30 -c:v libx264 -preset ultrafast -c:a libfdk_aac -profile:a aac_he_v2 -ar 44100 -ac 2 -filter_complex "[0:v]scale=320:240[a];[a]pad=640:240[b];[b][1:v]overlay=320:0[out]" -map "[out]" -movflags faststart  -map 1:a  c.mp4
```

#### 滤镜加水印

```shell
ffmpeg -i killer.mp4 -filter_complex "movie=./logo/daka.png,scale=64:48[w];[0:v]curves=vintage[o];[o][w]overlay=30:10[out]" -map "[out]" -map 0:a test1.mp4
```

```shell
# 将图片文件放在当前文件夹
ffmpeg -i start.mp4 -i logo.png -filter_complex overlay out-image-start.mp4
```

### 特别的滤镜

#### 镜像

```shell
crop=iw/2:ih:0:0,split[left][tmp];[tmp]hflip[right]; 
```

#### 复古

```shell
curves=vintage;
```

#### 光晕

```shell
vignette=PI/4
```

#### 变暗

```shell
colorlevels=rimin=0.058:gimin=0.058:bimin=0.058
```

#### 增加对比度

```shell
fftfilt=dc_Y=0:weight_Y='exp(-4 * ((Y+X)/(W+H)))
```

#### 降噪

```shell
hqdn3d=luma_spatial=15.0
```

#### 强对比度

```shell
curves=strong_contrast
```

#### 变亮

```shell
curves=lighter
```

#### 锐化

```shell
fftfilt=dc_Y=0:weight_Y='1+squish(1-(Y+X)/100)'
```


#### 低通滤波

```shell
fftfilt=dc_Y=0:weight_Y='squish((Y+X)/100-1)'
```

#### 高通滤波

```shell
fftfilt=dc_Y=128:weight_Y='squish(1-(Y+X)/100)'
```


#### 边缘检测

```shell
edgedetect
```

#### 底片

```shell
negate
```

#### 彩色底片

```shell
curves = ‘none’‘color_negative’
```

## 音视频的拼接与裁剪

### 裁剪

#### 截取音频片段

```shell
ffmpeg -i  input.mp3 -ss 00:00:01 -t 00:00:30 -acodec copy 1_part.mp3
```

#### 截取视频片段

```shell
ffmpeg -i out.mp4 -ss 00:00:00 -t 10 out1.mp4

ffmpeg -i long.mp4 -y -ss 00:00:05 -t 00:00:05 part1.mp4
```

```shell
# 这种方法可能导致元数据时长和实际时长不一致
ffmpeg -y -ss 00:00:00 -t 00:00:05 -i long.mp4 -vcodec copy -acodec copy part0.mp4
```

- -ss 开始时间
- -t 持续时间

#### 将视频分为多个部分

```shell
# 将一个视频分成两个
ffmpeg -i start.mp4 -t 00:00:12 -c copy part01.mp4 -ss 00:00:12 -codec copy part02.mp4
```

### 视频拼接

#### 方法一

1. 创建一个文本文件，写入待拼接的视频名称

`inputs.txt`

```
file '1.flv'
file '2.flv'
file '3.flv'
```

2. 执行拼接命令

```shell
ffmpeg -f concat -i inputs.txt -c copy output.flv
```

#### 方法二

```shell
ffmpeg -i  input.mp4 -vcodec copy -acodec copy -vbsf h264_mp4toannexb  input.ts
ffmpeg -i 2.mp4 -vcodec copy -acodec copy -vbsf h264_mp4toannexb 2.ts

ffmpeg -i "concat: input.ts|2.ts" -acodec copy -vcodec copy -absf aac_adtstoasc merge.mp4
```

### 音频拼接

```shell
ffmpeg -i 1.mp3 -i 2.mp3 -filter_complex '[0:0] [1:0] concat=n=2:v=0:a=1 [a]' -map [a] 3.mp3
```

### 音频混音

#### amerge

```shell
ffmpeg -i test.mp4 -i test.mp3 -filter_complex "[0:a] [1:a]amerge=inputs=2[aout]" -map "[aout]" -ac 2 mix_amerge.aac
```

#### amix

```shell
ffmpeg -y -i 1.mp3 -i 2.mp3 -i 3.mp3 -filter_complex 'amix=inputs=3:duration=longest:dropout_transition=2' mix.mp3

ffmpeg -y -i 1.mp3 -i 2.m4a -filter_complex 'amix=inputs=2:duration=longest:dropout_transition=2' 1m2.mp3
```

- inputs: 输入的数量，默认为 2
- duration: 决定流何时结束
  - longest: 适配最长输入的持续时间
  - shortest: 适配最短输入的持续时间
  - first: 适配第一个输入的持续时间
- dropout_transition: 输入流结束时容量重整化的转换时间， 默认值为 2 秒。

## 视频图像相互转换

### 视频转换为 JPEG

```shell
ffmpeg –i test.avi –r 1 –f image2 image-%3d.jpg
```

- `-r` 提取图像的频率

### 视频转换为 GIF

```shell
ffmpeg -i out.mp4 -ss 00:00:00 -t 10 out.gif
```

### 从多张图片创建视频

```shell
ffmpeg  -f image2 -i image-%3d.jpeg images.mp4

# 每秒切换一张图片
ffmpeg -framerate 1 -i image-%02d.jpg -c:v libx264 -r 30 -pix_fmt yuv420p output.mp4

# 每帧切换一张图片
ffmpeg -framerate 30 -i image-%02d.jpg -c:v libx264 -pix_fmt yuv420p output.mp4
```

## 直播相关

### 推流

```shell
ffmpeg -re -i out.mp4 -c copy -f flv rtmp://server/live/stream
```

### 拉流保存

```shell
ffmpeg -i rtmp://server/live/stream -c copy dump.flv
```

### 转流

```shell
ffmpeg -i rtmp://server/live/original -c:a copy -c:v copy -f flv rtmp://server/live/h264
```

### 实时推流

```shell
ffmpeg -framerate 15 -f avfoundation -i "1" -s 1280x720 -c:v libx264  -f  flv rtmp://localhost:1935/live/room
```

## 录屏操作

### 列出设备

```shell
$ ffmpeg -devices
Devices:
 D. = Demuxing supported
 .E = Muxing supported
 --
 DE alsa            ALSA audio output
  E caca            caca (color ASCII art) output device
 DE fbdev           Linux framebuffer
 D  iec61883        libiec61883 (new DV1394) A/V input device
 D  jack            JACK Audio Connection Kit
 D  kmsgrab         KMS screen capture
 D  lavfi           Libavfilter virtual input device
 D  libcdio
 D  libdc1394       dc1394 v.2 A/V grab
 D  openal          OpenAL audio capture device
  E opengl          OpenGL output
 DE oss             OSS (Open Sound System) playback
 DE pulse           Pulse audio output
  E sdl,sdl2        SDL2 output device
 DE sndio           sndio audio playback
 DE video4linux2,v4l2 Video4Linux2 output device
 D  x11grab         X11 screen capture, using XCB
  E xv              XV (XVideo) output device
```

不太懂，之后再补，暂略。
