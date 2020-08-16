# 添加水印

- [添加水印](#添加水印)
  - [文字水印](#文字水印)
    - [中文水印](#中文水印)
    - [实时时间水印](#实时时间水印)
    - [间隔性出现消失水印](#间隔性出现消失水印)
  - [图片水印](#图片水印)
    - [读取输入文件的流指定为水印](#读取输入文件的流指定为水印)
    - [Movie 指定水印文件路径](#movie-指定水印文件路径)

## 文字水印

在视频中增加文字水印需要准备的条件比较多，需要有文字字库处理的相关文件，在编译 FFmpeg 时需要支持 Free Type、Font Config、Iconv，系统中需要有相关的字库，在 FFmpeg 中增加纯字母水印可以使用 drawtext 滤镜进行支持。

![](../../imgs/drawtext.png)

### 中文水印

```
ffmpeg -y -i video.mp4 -vf "drawtext=fontsize=100:fontfile=font.ttf:text='你好':fontcolor=green:box=1:boxcolor=yellow:x=20:y=20" video_logo_font.mp4
```

- `-vf` `-filter:v` 的别名，为视频流添加滤镜处理
- `drawtext` 添加文字水印
- `fontsize` 指定字体大小
- `fontfile` 指定字体库
- `text` 指定文字内容
- `fontcolor` 指定文字颜色
- `box=1` 表示为水印添加背景色
- `boxcolor` 指定背景色
- `x=20:y=20` 指定水印偏移的位置

### 实时时间水印

```
ffmpeg -y -i video.mp4 -vf "drawtext=fontsize=100:text='%{localtime\:%Y-%m-%d %H-%M-%S}':fontcolor=green:box=1:boxcolor=yellow:x=20:y=20" video_logo_time.mp4
```

### 间隔性出现消失水印

```
ffmpeg -y -i video.mp4 -vf "drawtext=fontsize=100:fontfile=font.ttf:text='水印':fontcolor=green:box=1:boxcolor=yellow:x=20:y=20:enable=lt(mod(t\,2)\,1)" video_logo_intermittent.mp4
```

- `mod(t\,2)`  计算 `t%2`
- `lt(mod(t\,2)\,1)` 如果 `mod(t\,2)<1`，返回 1，否则返回 0
- `enable=lt(mod(t\,2)\,1)` 每隔 1s 显示一次水印，`enable=lt(mod(t\,3)\,1)` 每隔 3s.

## 图片水印

### 读取输入文件的流指定为水印

```
ffmpeg -i video.mp4 -i logo.jpg -filter_complex "[1:v]scale=100:100[logo];[0:v][logo]overlay=x=0:y=0" video_logo.mp4
```

- `[1:v]scale=100:100[logo]` 将第 2 个输入文件的视频流/图片缩放成 `100x100` 大小，并设置标签 `[logo]`
- `[0:v][logo]overlay=x=0:y=0` 将第 1 个输入文件的视频流用 `[logo]` 覆盖 `(0,0)` 位置，即左上角位置

### Movie 指定水印文件路径

![](../../imgs/movie.png)

```
ffmpeg -y -i video.mp4 -vf "movie=logo.jpg,scale=100x100[logo];[in][logo]overlay=100:100[out]" video_logo.mp4
```

- `scale=100x100` 调整水印图片的大小
