---
date: 2021-02-22T10:56:09+08:00  # 创建日期
author: "Rustle Karl"  # 作者

# 文章
title: "各种系统安装官方编译的 FFmpeg"  # 文章标题
url:  "posts/ffmpeg/compile/install"  # 设置网页链接，默认使用文件名
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

## 推荐 Docker

```shell
docker pull linuxserver/ffmpeg:latest
```

```shell
docker run -it --entrypoint="" --rm --name ff linuxserver/ffmpeg:latest bash
```


```shell

```





## CentOS 7

```shell
sudo yum install epel-release
```

```shell
sudo yum localinstall --nogpgcheck https://download1.rpmfusion.org/free/el/rpmfusion-free-release-7.noarch.rpm
```

```shell
sudo yum install ffmpeg ffmpeg-devel
```

```shell
ffmpeg -version
```

```shell

```


```shell

```


```shell

```



