---
date: 2021-03-06T23:25:48+08:00  # 创建日期
author: "Rustle Karl"  # 作者

# 文章
title: "ffmpeg 的 RTMP 推流"  # 文章标题
# description: "文章描述"
url:  "posts/ffmpeg/abc/rtmp_hls"  # 设置网页永久链接
tags: [ "ffmpeg" ]  # 自定义标签
series: [ "FFmpeg 从入门到放弃"]  # 文章主题/文章系列
categories: [ "学习笔记"]  # 文章分类

# 章节
weight: 20 # 排序优先级
chapter: false  # 设置为章节

index: true  # 是否可以被索引
toc: true  # 是否自动生成目录
draft: false  # 草稿
---

### **什么是推流？**

> 推流，指的是把采集阶段封包好的内容传输到服务器的过程。其实就是将现场的视频信号传到网络的过程。

用大白话讲，**推流就是把本地音视频数据通过网络上传到云端/后台服务器**，所谓**“采集阶段封包好”**，笔者认为是未解码的 H264 的 NALU。

推流的整个阶段如下图：

![img](https://i.loli.net/2021/03/06/3AQ8BLzxKRTSMpq.jpg)

上图从“推流端”到“源站”（同上文所说到的“服务器”），再到 CDN 分发节点，最后到“播放端”，整个过程的视音频数据，都是压缩的数据流。也就是说，对视频数据来说，就是 H264 码流。**解码工作是在播放端进行的。**

推流的工作可想而知，最多的应用就是直播；而在大多数视频门户网站，笔者目前也很疑惑，是否存在步骤 1，从效率上来说，视频数据直接存放在“源站”，通过CDN根据客户端请求下发，应该就可以了。至于具体做法，有待考证，笔者在这里姑且记一笔吧。

了解了什么是推流之后，下一个问题自然就出来了：**应该怎么推流呢？**这其实是一个复杂的过程，而且还需要了解”源站“，也就是推流服务器。本文仅涉及本地音视频数据通过网络上传到云端/后台服务器的**本地音视频上传**阶段，所以，先来了解**本地的数据是怎么上传的**，第一步就是了解推流的协议！

笔者了解到的几个目前常用的推流协议有：RTMP，HLS，webRTC，HTTP-FLV。

### **RTMP**

RTMP 是 Real Time Messaging Protocol （实时消息传输协议）的首字母缩写。是 Adobe 公司开发的一个基于 TCP 的应用层协议，也就是说，RTMP 是和 HTTP/HTTPS 一样，是应用层的一个协议族。**RTMP 在 TCP 通道上一般传输的是 flv 格式流**。

请注意，**RTMP 是网络传输协议，而 flv 则是视频的封装格式。** flv 封装格式设计出来的目的是为了用于网络传输使用的，因此 RTMP+FLV 可以说是”黄金搭档“。

RTMP 协议包括：基本协议及 RTMPT/RTMPS/RTMPE 等多种变种。RTMP 协议家族有以下几个点挺有趣：

1. RTMP 工作在 TCP 之上，默认使用端口 1935，这个是基本形态；
2. RTMPE 在 RTMP 的基础上增加了加密功能；
3. RTMPT 封装在 HTTP 请求之上，可穿透防火墙；
4. RTMPS 类似 RTMPT，增加了 TLS/SSL 的安全功能；
5. RTMFP 使用 UDP 进行传输的 RTMP ；

RTMP 就是专门针对多媒体数据流的实时通信设计出来的一种网络数据传输协议，主要用来在 Flash/AIR 平台和支持 RTMP 协议的流媒体 / 交互服务器之间进行音视频和数据通信。现在 Adobe 公司已经不支持了，不过目前，该协议还在广泛使用。

### **II. ffmpeg 的 rtmp 推流**

下面将介绍 2 种 ffmpeg 推流的方式：命令行（cmd）和代码（code）。

### **命令行（CMD）推流**

ffmpeg cmd 的参数实在太多，因此这里只介绍基础的、以及笔者了解的跟读者们做一个分享。

首先，来看一个将本地视频文件推流到服务器的最基础的命令：

```text
ffmpeg -i ${input_video} -f flv rtmp://${server}/live/${streamName}
```

- `-i`：表示输入视频文件，后跟视频文件路径/URL。
- `-f`：强制ffmpeg采用某种格式，后跟对应的格式。

上文有提到，RTMP一般用flv流数据，因此多设置`-f flv`。

接着，另一个基本需求，就是在推流的时候希望不要加上音频，这个也好实现：

```text
ffmpeg -i ${input_video} -vcodec copy -an -f flv rtmp://${server}/live/${streamName}
```

- `-vcodec`：指定视频解码器，`v`是视频`video`，`codec`就是解码器，后跟解码器名称，`copy`表示不作解码；
- `-acodec`：指定音频解码器，同理，`a`是`audio`，后跟解码器名称。`an`代表`acodec none`就是去掉音频的意思。

关于`a/v`的写法很多，除了上面介绍的，还有`-c:v copy -c:a copy`等。

再有其他的需求，读者可自行Google。

### **代码（code）推流**

ffmpeg的c++代码推流，网上也是一搜一大堆。笔者推荐雷神的[最简单的基于FFmpeg的推流器（以推送RTMP为例）](https://link.zhihu.com/?target=https%3A//blog.csdn.net/leixiaohua1020/article/details/39803457)，能够满足基础的推流需求。但是笔者在实际应用场景时遇到过几个case，最后总结得到一份相对鲁棒可用的code segment：

```cpp
AVFormatContext *mp_ifmt_ctx = nullptr;
AVFormatContext *mp_ofmt_ctx = nullptr;
uint64_t start_timestamp; // 获取得到第一帧的时间
// ...

int pushStreaming(AVPacket *pkt, int frm_cnt) {
    // 做筛选：因为实际源视频文件可能包括多个音视频码流，这里只选取一路视频流m_vs_index和一路音频流m_as_index。
    if (pkt->stream_index == m_vs_index || pkt->stream_index == m_as_index) {
        // 没有pts的视频数据，如未解码的H.264裸流，需要重新计算其pts。
        if (pkt->pts == AV_NOPTS_VALUE) {
            AVRational time_base = mp_ifmt_ctx->streams[m_vs_index]->time_base;
            // Duration between 2 frames (us)
            int64_t calc_duration = (double)AV_TIME_BASE / 
                    av_q2d(mp_ifmt_ctx->streams[m_vs_index]->r_frame_rate);
            // Reset Parameters
            pkt->pts = (double)(frm_cnt * calc_duration) / 
                    (double)(av_q2d(time_base) * AV_TIME_BASE);
            pkt->dts = pkt->pts;
            pkt->duration = (double)calc_duration /
                            (double)(av_q2d(time_base) * AV_TIME_BASE);
        }
        
        // 笔者在这里省去了delay操作，读者可根据需求增加。该操作通过控制推流的速率来减轻推流服务器的压力。
        // if (pkt->stream_index == m_vs_index) {
        //     AVRational time_base = mp_ifmt_ctx->streams[m_vs_index]->time_base; 
        //     AVRational time_base_q = {1, AV_TIME_BASE}; 
        //     int64_t pts_time =  av_rescale_q(pkt->dts, time_base, time_base_q); 
        //     int64_t now_time = av_gettime() - start_timestamp;
        //     if (pts_time > now_time) {
        //         av_usleep((unsigned int)(pts_time - now_time));
        //     }
        // }

        //计算延时后，重新指定时间戳
        AVRational istream_base = mp_ifmt_ctx->streams[pkt->stream_index]->time_base;
        AVRational ostream_base = mp_ofmt_ctx->streams[pkt->stream_index]->time_base;
        pkt->pts = av_rescale_q_rnd(pkt->pts, istream_base, ostream_base,
                (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
        pkt->dts = av_rescale_q_rnd(pkt->dts, istream_base, ostream_base,
                (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
        pkt->pts = pkt->pts < 0 ? 0 : pkt->pts;
        pkt->dts = pkt->dts < 0 ? 0 : pkt->dts;
        pkt->duration = (int)av_rescale_q(pkt->duration, istream_base, ostream_base);
        pkt->pos = -1;
        if (pkt->pts < pkt->dts) {
            return 1;
        }
        // 向推流服务器推送流数据
        int ret = av_interleaved_write_frame(mp_ofmt_ctx, pkt);
        if (ret < 0) {
            return ret;
        }
    }
    return 0;
}
```

### **III. 推流遇到的坑**

笔者遇到过 2 个推流失败的 case：

1. 推一个 RTSP 摄像头的流数据时，一旦打开该摄像头的音频轨道就会在 `av_interleaved_write_frame()` 函数处出错。（返回码不记得了）
2. 使用英飞拓某几款摄像头推流时，总是失败，返回码显示 -33。

以上 2 个问题都成功定位问题所在：

1. 操作人员在打开音频轨道时总是同时打开 2 个音频轨道，只要选择关闭其中 1 个音频，马上就可以推流了；
2. 这几款摄像头同时包括多个视频码流，似乎他们会通过 RTSP 同时把多个码流同时传过来；最后在代码中强行过滤视音频码流，只保留一路视频 + 一路音频就可以了。

原因归结为一个：**flv 格式至多只能包括一个视频流和一个音频流**。