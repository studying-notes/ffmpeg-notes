# 详解 avformat 库

libavformat 是 FFmpeg 中处理音频、视频以及字幕封装和解封装的通用框架，内置了很多处理多媒体文件的 Muxer 和 Demuxer，它支持如 AVInputFormat 的输入容器和 AVOutputFormat 的输出容器，同时也支持基于网络的一些流媒体协议，如 HTTP、RTSP。

## 音视频流封装

![](../../imgs/mux.png)


