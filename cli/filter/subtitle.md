# 添加字幕

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
