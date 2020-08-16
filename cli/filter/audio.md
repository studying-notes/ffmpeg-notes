# 音频 Fliter 详解

- [音频 Fliter 详解](#音频-fliter-详解)
  - [声道定义](#声道定义)
  - [双声道合并单声道](#双声道合并单声道)
  - [双声道提取多单声道](#双声道提取多单声道)
  - [双声道转双音频流](#双声道转双音频流)
  - [单声道转双声道](#单声道转双声道)
  - [2 个音频源合并为双声道](#2-个音频源合并为双声道)
  - [多个音频合并为多声道](#多个音频合并为多声道)
  - [音频音量探测](#音频音量探测)
  - [绘制音频波形](#绘制音频波形)
    - [单声道波形图](#单声道波形图)
    - [5.1 声道的波形图](#51-声道的波形图)
  - [音频倍速处理](#音频倍速处理)
    - [半速处理](#半速处理)
    - [2 倍速处理](#2-倍速处理)

## 声道定义

声道 Sound Channel 是指声音在录制或播放时在不同空间位置采集或回放的相互独立的音频信号，所以声道数也就是声音录制时的音源数量或回放时相应的扬声器数量。

声卡所支持的声道数是衡量声卡档次的重要指标之一，从单声道到最新的环绕立体声。

## 双声道合并单声道

```
ffmpeg -i audio.aac -ac 1 audio_single.aac
```

## 双声道提取多单声道

```
ffmpeg -i audio.aac -map_channel 0.0.0 audio_left.aac -map_channel 0.0.1 audio_right.aac
```

## 双声道转双音频流

```
ffmpeg -i audio.aac -filter_complex channelsplit=channel_layout=stereo audio.mka
```

## 单声道转双声道

```
ffmpeg -i audio_right.aac -ac 2 audio_right_2.mka
```

## 2 个音频源合并为双声道

```
ffmpeg -i audio_left.aac -i audio_right.aac -filter_complex "[0:a][1:a]amerge=inputs=2[aout]" -map "[aout]" audio_amerge.mka
```

## 多个音频合并为多声道

```
ffmpeg -i left.aac -i left.aac -i left.aac -i left.aac -i left.aac -i left.aac -filter_complex "[0:a][1:a][2:a][3:a][4:a][5:a]amerge=inputs=6[aout]" -map "[aout]" audio_5.1.aac
```

## 音频音量探测

```
ffmpeg -i audio.aac -filter_complex volumedetect -c:v copy -f null /dev/null
```

## 绘制音频波形

### 单声道波形图

```
ffmpeg -i audio_left.aac -filter_complex "showwavespic=s=1920x1080" -frames:v 1 left_wave.png
```

### 5.1 声道的波形图

```
ffmpeg -i audio_5.1.aac -filter_complex "showwavespic=s=1920x1080:split_channels=1" -frames:v 1 audio_5.1_wave.png
```

## 音频倍速处理

### 半速处理

```
ffmpeg -i audio.aac -filter_complex "atempo=tempo=0.5" -acodec aac audiotempo0.5.aac
```

### 2 倍速处理

```
ffmpeg -i audio.aac -filter_complex "atempo=tempo=2" -acodec aac audiotempo2.aac
```
