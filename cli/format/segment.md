# 视频文件切片

## FFmpeg 切片 segment 参数

![](imgs/segment1.png)
![](imgs/segment2.png)

## FFmpeg 切片 segment 示例

### 指定切片文件的格式

```shell
ffmpeg -i long.mp4 -c copy -f segment -segment_format mp4 long_seg%d.mp4
```

## 按照时间点剪切

```shell
ffmpeg -i long.mp4 -c copy -f segment -segment_format mp4 -segment_times 3,6,9 long_seg%d.mp4
```

### 指定切片索引列表

#### 生成 ffconcat 格式索引文件

```shell
ffmpeg -i long.mp4 -c copy -f segment -segment_format mp4 -segment_list_type ffconcat -segment_list long_output.lst long_seg%d.mp4
```

#### 生成 FLAT 格式索引文件

```shell
ffmpeg -i long.mp4 -c copy -f segment -segment_format mp4 -segment_list_type flat -segment_list long_output.txt long_seg%d.mp4
```

#### 生成 CSV 格式索引文件

```shell
ffmpeg -i long.mp4 -c copy -f segment -segment_format mp4 -segment_list_type csv -segment_list long_output.csv long_seg%d.mp4
```

#### 生成 M3U8 格式索引文件

```shell
ffmpeg -i long.mp4 -c copy -f segment -segment_format mp4 -segment_list_type m3u8 -segment_list long_output.m3u8 long_seg%d.mp4
```
