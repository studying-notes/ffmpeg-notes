# MP4 格式标准介绍

- MP4 文件由许多个 Box 与 FullBox 组成
- 每个 Box 由 Header 和 Data 两部分组成
- FullBox 是 Box 的扩展，其在 Box 结构的基础上，在 Header 中增加 8位 version 标志和 24 位的 fags 标志
- Header 包含了整个 Box 的长度的大小 `size` 和类型 `type`，当 `size` 等于 0 时，代表这个 Box 是文件的最后一个 Box。当 `size` 等于1时，说明 Box 长度需要更多的位来描述，在后面会定义一个 64 位的 `largesize` 用来描述 Box 的长度。当 `type` 为 `uid` 时，说明这个 Box 中的数据是用户自定义扩展类型
- Data 为 Box 的实际数据，可以是纯数据，也可以是更多的子 Box
- 当一个 Box 中 Data 是一系列的子 Box 时，这个 Box 又可以称为 Container Box

## moov 容器

moov 容器定义了一个 MP4 文件中的数据信息，类型是 moov，是一个容器Atom，其至少必须包含以下三种 Atom 中的一种：

- mvhd标签，Movie header atom，存放未压缩过的影片信息的头容器
- cmov标签，Compressed Movie Atom，压缩过的电影信息容器，此容器不常用
- rmra标签，Reference Movie atom，参考电影信息容器，此容器不常用

也可以包含其他容器信息，例如影片剪辑信息 Clipping atom（cip）、一个或几个 trakAtom（trak）、一个 Color Table atom（ctab）和一个 User Data Atom（udta）。

其中，mvhd 中定义了多媒体文件的 time scale、duration 以及 display characteristics。

而 trak 中定义了多媒体文件中的一个 track 的信息，track 是多媒体文件中可以独立操作的媒体单位，例如一个音频流就是一个 track、一个视频流就是一个 track。



