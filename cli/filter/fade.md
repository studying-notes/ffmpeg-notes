# 淡入淡出与交叉淡入淡出

- [淡入淡出与交叉淡入淡出](#淡入淡出与交叉淡入淡出)
  - [参数](#参数)
  - [参数设置示例](#参数设置示例)
  - [淡入淡出实例](#淡入淡出实例)
  - [交叉淡入淡出](#交叉淡入淡出)
    - [视频交叉淡入淡出](#视频交叉淡入淡出)
    - [音频交叉淡入淡出](#音频交叉淡入淡出)

## 参数

- `type, t` 指定类型是 `in` 代表淡入，`out` 代表淡出，默认为 `in`；
- `start_frame, s` 指定应用效果开始的帧序数，默认为 0；
- `nb_frames, n` 应用效果的最后一帧序数。对于淡入，在此帧后将以本身的视频输出，对于淡出，此帧后将以设定的颜色输出，默认 `25`；
- `alpha` 设置为 1 则只在透明通道实施效果，默认为 0；
- `start_time, st` 指定按秒的开始时间戳来应用效果，默认为 0；
- `duration, d` 按秒的效果持续时间。对于淡入，在此时后将以本身的视频输出，对于淡出，此时后将以设定的颜色输出。
- `color, c` 设置淡化后/淡入前的颜色，默认为 `black`。

## 参数设置示例

```shell
# 30 帧开始淡入
fade=in:0:30
fade=t=in:s=0:n=30

# 在 200 帧视频中从最后 45 帧淡出
fade=out:155:45 fade=type=out:start_frame=155:nb_frames=45

# 对 1000 帧的视频 25 帧淡入，最后 25 帧淡出
fade=in:0:25,fade=out:975:25

# 让前 5 帧为黄色，然后在 5-24 淡入
fade=in:5:20:color=yellow

# 仅在透明通道的第 25 帧开始淡入
fade=in:0:25:alpha=1

# 设置 5.5秒 的黑场，然后开始 0.5 秒的淡入
fade=t=in:st=5.5:d=0.5
```

## 淡入淡出实例

```shell
# 设置开始 3 秒的淡入和 10 秒后的淡出
ffmpeg -i input.mp4 -vf fade=in:d=3,fade=out:st=10:d=3 output.mp4 -y

# 设置淡入淡出同时截取视频
ffmpeg -i input.mp4 -ss 00:00:00 -t 00:00:12.95 -vf fade=in:d=3,fade=out:st=10:d=3 output.mp4 -y
```

## 交叉淡入淡出

### 视频交叉淡入淡出

```shell
ffmpeg -y -i part0.mp4 -i part1.mp4 -filter_complex "\
[0:v]format=pix_fmts=yuva420p,fade=out:st=5:d=6:alpha=1,setpts=PTS-STARTPTS[v0];\
[1:v]format=pix_fmts=yuva420p,fade=in:st=0:d=6:alpha=1,fade=out:st=5:d=6:alpha=1,setpts=PTS-STARTPTS+5/TB[v1];\
[v0][v1]overlay=format=yuv420[outv]" \
-vcodec libx264 -map [outv] merge.mp4
```

```shell
ffmpeg -y -i part0.mp4 -i part1.mp4 -i part2.mp4  -i part3.mp4  -i part4.mp4 -filter_complex "\
[0:v]format=pix_fmts=yuva420p,fade=t=out:st=9:d=2:alpha=1,setpts=PTS-STARTPTS[v0];\
[1:v]format=pix_fmts=yuva420p,fade=t=in:st=0:d=2:alpha=1,fade=t=out:st=9:d=2:alpha=1,setpts=PTS-STARTPTS+9/TB[v1];\
[2:v]format=pix_fmts=yuva420p,fade=t=in:st=0:d=2:alpha=1,fade=t=out:st=9:d=2:alpha=1,setpts=PTS-STARTPTS+18/TB[v2];\
[3:v]format=pix_fmts=yuva420p,fade=t=in:st=0:d=2:alpha=1,fade=t=out:st=9:d=2:alpha=1,setpts=PTS-STARTPTS+27/TB[v3];\
[4:v]format=pix_fmts=yuva420p,fade=t=in:st=0:d=2:alpha=1,fade=t=out:st=9:d=2:alpha=1,setpts=PTS-STARTPTS+36/TB[v4];\
[v0][v1]overlay[v01];\
[v01][v2]overlay[v012];\
[v012][v3]overlay[v0123];\
[v0123][v4]overlay=format=yuv420[outv]" \
-vcodec libx264 -map [outv] merge.mp4
```

### 音频交叉淡入淡出

```shell
ffmpeg -y -i part0.mp4 -i part1.mp4 -filter_complex "\
[0:v]format=pix_fmts=yuva420p,fade=t=out:st=5:d=6:alpha=1,setpts=PTS-STARTPTS[v0]; \
[1:v]format=pix_fmts=yuva420p,fade=t=in:st=0:d=6:alpha=1,fade=t=out:st=5:d=6:alpha=1,setpts=PTS-STARTPTS+5/TB[v1]; \
[v0][v1]overlay=format=yuv420[v]; \
[1][2]acrossfade=d=1[a];" \
-vcodec libx264 -map [outv] merge.mp4
```
