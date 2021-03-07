# FFmpeg 输出 MP3

## MP3 编码参数介绍

```shell
$ ffmpeg -h encoder=libmp3lame

Encoder libmp3lame [libmp3lame MP3 (MPEG audio layer 3)]:
    General capabilities: delay small
    Threading capabilities: none
    Supported sample rates: 44100 48000 32000 22050 24000 16000 11025 12000 8000     
    Supported sample formats: s32p fltp s16p
    Supported channel layouts: mono stereo
libmp3lame encoder AVOptions:
  -reservoir         <boolean>    E...A..... use bit reservoir (default true)        
  -joint_stereo      <boolean>    E...A..... use joint stereo (default true)
  -abr               <boolean>    E...A..... use ABR (default false)
```

![](../imgs/mp3.png)

## MP3 的编码质量设置

```shell
ffmpeg -i audio.aac -acodec libmp3lame output.mp3
```

![](../imgs/mp3_2.png)

```shell
ffmpeg -i high.flac -acodec libmp3lame -q:8 output.mp3
```


```shell

```

```shell

```

```shell

```

```shell

```


