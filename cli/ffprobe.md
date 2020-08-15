# FFprobe 常用命令

> 查看媒体文件格式的工具

- [FFprobe 常用命令](#ffprobe-常用命令)
  - [查看音频文件](#查看音频文件)
  - [查看视频文件](#查看视频文件)
  - [格式化输出](#格式化输出)
  - [以 JSON 格式输出](#以-json-格式输出)
  - [显示帧信息](#显示帧信息)
  - [查看包信息](#查看包信息)

## 查看音频文件

```shell
$ ffprobe audio.aac

Input #0, aac, from 'audio.aac':
  Duration: 00:01:51.72, bitrate: 139 kb/s
    Stream #0:0: Audio: aac (LC), 44100 Hz, stereo, fltp, 139 kb/s
```

## 查看视频文件

```shell
$ ffprobe video.mp4

Input #0, mov,mp4,m4a,3gp,3g2,mj2, from 'video.mp4':
  Metadata:
    major_brand     : isom
    minor_version   : 512
    compatible_brands: isomiso2avc1mp41
    title           : 밤편지(夜信)
    artist          : IU
    encoder         : Lavf58.45.100
    comment         : Do
  Duration: 00:00:13.02, start: 0.000000, bitrate: 1465 kb/s
    Stream #0:0(und): Video: h264 (High) (avc1 / 0x31637661), yuv420p, 1920x1080 [SAR 1:1 DAR 16:9], 1328 kb/s, 23.98 fps, 23.98 tbr, 24k tbn, 47.95 tbc (default)
    Metadata:
      handler_name    : VideoHandler
    Stream #0:1(und): Audio: aac (LC) (mp4a / 0x6134706D), 44100 Hz, stereo, fltp, 130 kb/s (default)
    Metadata:
      handler_name    : SoundHandler
```

## 格式化输出

```shell
ffprobe -show_format video.mp4
```

## 以 JSON 格式输出

```shell
ffprobe -print_format json -show_streams video.mp4

ffprobe -print_format json -show_streams video.mp4 > meta.json
```

## 显示帧信息

```shell
ffprobe -show_frames video.mp4
```

## 查看包信息

```shell
ffprobe -show_packets video.mp4
```
