# FFmpeg 输出 AAC

在音视频流中，无论直播与点播，AAC 都是目前最常用的一种音频编码格式，例如 RTMP 直播、HLS 直播、RTSP 直播、FLV 直播、FLV 点播、MP4 点播等文件中都是常见的 AAC 音视频。

与 MP3 相比，AAC 是一种编码效率更高、编码音质更好的音频编码格式，常见的使用 AAC 编码后的文件存储格式为 m4a。

## FFmpeg 中的 AAC 编码器使用



```shell
ffmpeg -i high.flac -c:a aac -b:a 160k output.aac
```

```shell

```

```shell

```

```shell

```




