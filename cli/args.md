# FFmpeg 命令参数

## 显示功能/信息

| 参数 | 作用 |
| ---- | ---- |
| -formats | 显示可用格式 |
| -muxers | 显示可用的 muxers |
| -demuxers | 显示可用的多路分解器 |
| -devices | 显示可用设备 |
| -codecs | 显示可用的编解码器 |
| -decoders | 显示可用的解码器 |
| -encoders | 显示可用的编码器 |
| -bsfs | 显示可用的位流过滤器 |
| -protocols | 显示可用协议 |
| -filters | 显示可用的过滤器 |
| -pix_fmts | 显示可用的像素格式 |
| -layouts | 显示标准的频道布局 |
| -sample_fmts | 显示可用的音频样本格式 |
| -colors | 显示可用的颜色名称 |
| -sources device | 列出输入设备的来源 |
| -sinks device | 列出输出设备的接收器 |
| -hwaccels | 显示可用的硬件加速方法 |

## 全局选项

| 参数 | 作用 |
| ---- | ---- |
| -y   | 覆盖输出文件 |
| -n | 从不覆盖输出文件 |
| -hide_banner | 隐藏标语及细节，只显示媒体文件信息 |
| -max_alloc bytes | 设置单个分配块的最大大小 |
| -ignore_unknown | 忽略未知的流类型 |
| -filter_threads | 非复杂过滤器线程的数量 |
| -lavfi, -filter_complex  | 复杂滤镜 |
| -filter_complex_threads | -filter_complex 的线程数 |
| -stats | 在编码过程中打印进度报告 |
| -max_error_rate maximum error rate | 最大错误率的错误率 `0.0` 无错误 `1.0` 100％ 错误），高于此比率将返回错误而不是成功 |
| -bits_per_raw_sample number | 设置每个原始样本的位数 |
| -vol volume | 更改音频音量（256 =正常） |

## 文件选项

| 参数 | 作用 |
| ---- | ---- |
| -f fmt | 强制格式 |
| -c codec | 编解码器名称 |
| -codec codec | 编解码器名称 |
| -pre preset | 预设名称 |
| -map_metadata outfile[,metadata]:infile[,metadata] | 设置元数据信息 |
| -t duration | 记录或转码音频/视频的“持续时间”秒 |
| -to time_stop | 记录或转码停止时间 |
| -fs limit_size | 设置限制文件大小 字节 |
| -ss time_off | 设置开始时间偏移 |
| -sseof time_off | 设置相对于 EOF 的开始时间偏移 |
| -seek_timestamp | 使用 `-ss` 按时间戳启用/禁用查找 |
| -timestamp time | 设置录制时间戳 |
| -metadata string=string | 添加元数据 |
| -program title=string:st=number... | 添加具有指定流的程序 |
| -target type | 指定目标文件类型 |
| -apad | 音频垫 |
| -frames number | 设置要输出的帧数 |
| -filter filter_graph | 设置流 filtergraph |
| -filter_script filename | 从文件中读取流 filtergraph 描述 |
| -reinit_filter | 输入参数更改时重新初始化 filtergraph |
| -discard | 丢弃 |
| -disposition | 处置 |

## 视频选项

| 参数 | 作用 |
| ---- | ---- |
| -vframes number | 设置要输出的视频帧数 |
| -r rate | 设置帧速率（Hz值，分数或缩写） |
| -s size | 设置帧大小（WxH 或缩写） |
| -aspect aspect | 宽高比设置的宽高比（4：3、16：9或1.3333、1.7777） |
| -bits_per_raw_sample number | 设置每个原始样本的位数 |
| -vn | 禁用视频 |
| -vcodec codec | 强制视频编解码器 |
| -timecode hh:mm:ss[:;.]ff | 设置初始 TimeCode 值 |
| -pass n | 选择通行证编号 1-3 |
| -vf filter_graph | 设置视频过滤器 |
| -ab bitrate | 音频比特率 -b:a |
| -b bitrate | 视频比特率 -b:v |
| -dn | 禁用数据 |

## 视频选项

| 参数 | 作用 |
| ---- | ---- |
| -aframes number | 设置要输出的音频帧数 |
| -aq quality | 设置音频质量（特定于编解码器） |
| -ar rate | 设置音频采样率（以 Hz 为单位） |
| -ac channels | 设置音频频道数 |
| -an | 禁用音频 |
| -acodec codec | 强制音频编解码器 |
| -vol volume | 更改音频音量（256 =正常） |
| -af filter_graph | 设置音频过滤器 |

## 字幕选项

| 参数 | 作用 |
| ---- | ---- |
| -s size | 设置帧大小（WxH或缩写） |
| -sn | 禁用字幕 |
| -scodec codec | 强制字幕编解码器 |
| -stag fourcc/tag | 强制字幕 tag/fourcc |
| -fix_sub_duration | 修复字幕的持续时间 |
| -canvas_size size | 设置画布大小（WxH 或缩写） |
| -spre preset | 将字幕选项设置为指示的预设 |
