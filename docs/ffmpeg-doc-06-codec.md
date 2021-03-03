---
date: 2021-03-03T13:51:52+08:00  # 创建日期
author: "Rustle Karl"  # 作者

# 文章
title: "Ffmpeg Doc 06 Codec"  # 文章标题
# description: "文章描述"
url:  "posts/ffmpeg/docs/ch06_codec"  # 设置网页永久链接
tags: [ "ffmpeg"]  # 标签
series: [ "FFmpeg 从入门到放弃"]  # 文章主题/文章系列
categories: [ "学习笔记"]  # 文章分类

# 章节
weight: 20 # 排序优先级
chapter: false  # 设置为章节

index: true  # 是否可以被索引
toc: true  # 是否自动生成目录
draft: false  # 草稿
---
## 10 编码选项
libavcodec提供一些通用的全局选项设置，可在所有的编码器和解码器起效。另外每个编解码器可以支持所谓的私有化设置，以满足特定的编解码要求。

有时，一个全局选项会影响到特定的编解码器，而对其它编解码产生不良影响或者会不被识别，所以你需要了解这些影响编解码选项的具体意义，了解那些只对特定编码或者解码有效的选项。

这些选项大多可以`-option value`的格式在ffmpeg工具中指定，其中`-option`是选项名，`value`是要设置的选项参数值，个别是利用`AVCodecContext`选项进行额外配置，还有极个别的使用定义在`libavutil/opt.h`中的API在程序过程中配置使用。

下面是这些选项的列表(括号中表示选项有效的状态，可能是decoding-解码时,encoding—编码时，audio-音频，video-视频，subtitles-字幕 以及特定的编码名称，如果mpeg4):

b integer (encoding,audio,video)

    设置码率，单位为bits/s。默认200K。
ab integer (encoding,audio)

    设置音频码率，单位bits/s。默认128K。
bt integer (encoding,video)

    设置视频码率偏离公差（video bitrate tolerance），单位bits/s。对于1次编码模式中码率控制公差指愿意偏离目标（码率）的平均码率值，故不能由此确定最小/最大码率，即不能假定最小/最大码率是目标码率+-码率偏离公差。太低的偏离公差影响编码后的质量。
flags flags (decoding/encoding,audio,video,subtitles)

    设置常见的标志

    可能值有：

    ‘mv4’

        使用4路监控宏块运动矢量 (mpeg4). 
    ‘qpel’

        使用1/4像素补偿 
    ‘loop’

        使用循环过滤 
    ‘qscale’

        使用固定放缩qscale 
    ‘gmc’

        使用gmc. 
    ‘mv0’

        一直假定mb中mv=<0,0>。
    ‘input_preserved’
    ‘pass1’

        在第1次编码中使用内部控制段模式。 
    ‘pass2’

        在第2次编码中使用内部控制段
    ‘gray’

        只有decode/encode灰度 
    ‘emu_edge’

        不描绘边缘 
    ‘psnr’

        在设置了错误变量时仍然继续编码
    ‘truncated’
    ‘naq’

        规范的自适应量化 
    ‘ildct’

        使用交错DCT. 
    ‘low_delay’

        强制减少延迟
    ‘global_header’

        在每个关键帧的扩展数据中添加全局头 
    ‘bitexact’

        只写的关于平台、编译创建以及时间无关（platform-, build- 和time-independent） 的数据 (除了(I)DCT)。这确保了文件和数据校验和平台之间的可重复性和匹配，它主要用于回归测试。 
    ‘aic’

        H263高级帧内编码/mpeg4打开ac预测 
    ‘cbp’

        已失效/过期，使用mpegvideo提供的私有选项 
    ‘qprd’

        已失效/过期，使用mpegvideo提供的私有选项  
    ‘ilme’

        交错运动估计。
    ‘cgop’

        关闭gop 

me_method integer (encoding,video)

    设置运动估计方法

    可能值有:

    ‘zero’

        0运动估计，即不进行估计 (最快) 
    ‘full’

        full运动估计 (最慢) 
    ‘epzs’

        EPZS运动估计 (默认) 
    ‘esa’

        esa运动估计 (full的别名) 
    ‘tesa’

        tesa运动估计 
    ‘dia’

        dia运动估计(epzs的别名) 
    ‘log’

        log运动估计 
    ‘phods’

        phods运动估计
    ‘x1’

        X1运动估计 
    ‘hex’

        hex运动估计 
    ‘umh’

        umh运动估计
    ‘iter’

        iter运动估计 

extradata_size integer

    设置扩展数据尺寸
time_base rational number

    设置编码的时间基础计量

    它是时间的基本单位(秒)的帧时间戳表示。对于固定FPS内容，这个值应该是`1/frame_rate`，每次时间戳都增加1个单位的时间量。
g integer (encoding,video)

    设置一组图片的数量，默认是12
ar integer (decoding/encoding,audio)

    设置音频采样率（单位Hz)
ac integer (decoding/encoding,audio)

    设置音频通道数
cutoff integer (encoding,audio)

    设置截止带宽
frame_size integer (encoding,audio)

    设置音频帧尺寸

    除了最后一帧音频数据，否则每个音频数据帧都包含`frame_size`设定大小的数据。当编码中`CODEC_CAP_VARIABLE_FRAME_SIZE`设置了，则这个值可能是0，在这种情况下帧的大小是没有限制的。它在一些解码器中显示固定帧大小。
frame_number integer

    设置帧数量
delay integer
qcomp float (encoding,video)

    设置视频量化压缩规模(VBR)。这里用在控制方程的常数。默认rc_eq推荐范围是: 0.0-1.0.
qblur float (encoding,video)

    设置视频量化尺度模糊(VBR).
qmin integer (encoding,video)

    设置最小视频量化尺度(VBR)。 取值范围是-1-69，默认为2
qmax integer (encoding,video)

    设置最大视频量化尺度(VBR)。 取值范围是-1-1024，默认为31
qdiff integer (encoding,video)

    设置量化级之间最大的差异(VBR).
bf integer (encoding,video)

    设置最大B帧间隔

    必须是-1 -16间的数。0表示禁止B帧，如果为-1表示依据编码器进行指定，默认值是0
b_qfactor float (encoding,video)

    设置P帧和B帧之间的qp因子
rc_strategy integer (encoding,video)

    设置码率控制方法
b_strategy integer (encoding,video)

    设置I/P/B帧选择策略.
ps integer (encoding,video)

    设置RTP播放加载数据量（缓冲），单位是字节（bytes）
mv_bits integer  
header_bits integer  
i_tex_bits integer  
p_tex_bits integer  
i_count integer  
p_count integer  
skip_count integer  
misc_bits integer  
frame_bits integer  
codec_tag integer  
bug flags (decoding,video)

    解决不能自动检测/识别编码的错误（bug）

    可能值:

    ‘autodetect’
    ‘old_msmpeg4’

        一些旧lavc处理的msmpeg4v3文件(不能自动检测) 
    ‘xvid_ilace’

        Xvid交错错误 (如果强制为fourcc==XVIX则可自动检测) 
    ‘ump4’

        (fourcc==UMP4可自动检测) 
    ‘no_padding’

        填充错误(自动检测) 
    ‘amv’
    ‘ac_vlc’

        非法vlc错误 (对每个fourcc自动检测) 
    ‘qpel_chroma’
    ‘std_qpel’

        老标准的qpel (对每个fourcc/version自动检测) 
    ‘qpel_chroma2’
    ‘direct_blocksize’

        direct-qpel-blocksize错误 (对每个fourcc/version自动检测) 
    ‘edge’

        edge填充bug (对每个fourcc/version自动检测) 
    ‘hpel_chroma’
    ‘dc_clip’
    ‘ms’

        微软破解解码器上的各种缺陷 
    ‘trunc’

        截断帧

lelim integer (encoding,video)

   设置亮度单系数消除阈值（负值也考虑直流系数）。
celim integer (encoding,video)

    设置消除色度单系数阈值（负值也考虑直流系数）
strict integer (decoding/encoding,audio,video)

    指定如何遵守标准（严格程度）

    可能的值:

    ‘very’

        严格模式，遵守过时的版本规格或者依软件需求的版本规格进行处理
    ‘strict’

        严格模式，无论如何都严格按照规格处理 
    ‘normal’
    ‘unofficial’

        允许非官方扩展
    ‘experimental’

        允许非标准的实验性质解码/编码器（未完成/未测试/不工作）**注意**实验性质的解码工具可能带来安全风险，不要用这类解码器解码不可信输入 

b_qoffset float (encoding,video)

    在P帧和B帧间设置QP（帧间偏移）
err_detect flags (decoding,audio,video)

    设置错误检测标志

    可能值:

    ‘crccheck’

        嵌入CRC验证 
    ‘bitstream’

        比特流规范偏差检测 
    ‘buffer’

        不当码流长度检测 
    ‘explode’

        在出现错误时中止解码 
    ‘ignore_err’

        忽略解码错误继续解码。则对于分析视频内容是十分有用的，这时希望无论如何解码都继续工作。
    ‘careful’

        
        考虑环境支持，一个正确的编码器不能被错误停止

has_b_frames integer
block_align integer
mpeg_quant integer (encoding,video)

    使用MPEG量化代替H.263。
qsquish float (encoding,video)

   保持Qmin和Qmax之间量化器。(0 = clip, 1 = use 还可以利用函数进行微调)。
rc_qmod_amp float (encoding,video)

    设置实验/量化 
rc_qmod_freq integer (encoding,video)

    设置实验/量化调制
rc_override_count integer
rc_eq string (encoding,video)

    速率控制方程组。除了内部标准定义外，可以有以下选择：bits2qp(bits), qp2bits(qp)。还可以利用下面介绍的常数: iTex、pTex、tex ，mv fCode iCount mcVar var isl isB avgQP qComp avgIITex avgPITx avgPPTex avgBPText。
maxrate integer (encoding,audio,video)

    设置最大比特率容差（单位 比特/秒）。要求的缓冲大小被设置。
minrate integer (encoding,audio,video)

    设置最小比特率容差（单位 比特/秒）。通常用于CBR编码，否则无意义
bufsize integer (encoding,audio,video)

    设置控制缓冲区大小（单位bits）
rc_buf_aggressivity float (encoding,video)

    目前无效
i_qfactor float (encoding,video)

    设置P帧和I帧间的QP因子
i_qoffset float (encoding,video)

    设置P帧和I帧间的QP偏移
rc_init_cplx float (encoding,video)

    设置1次编码的初始复杂性
dct integer (encoding,video)

    设置DCT（数字转换）算法

    可能值:

    ‘auto’

        自动选择一个优化质量算法(默认值) 
    ‘fastint’

        偏重速度 
    ‘int’

        精准整数 
    ‘mmx’
    ‘altivec’
    ‘faan’

        浮点AAN DCT 

lumi_mask float (encoding,video)

    压缩高亮
tcplx_mask float (encoding,video)

    设置临时/时间复杂遮蔽/蒙版
scplx_mask float (encoding,video)

    设置空间复杂遮蔽/蒙版
p_mask float (encoding,video)

    设置组间遮蔽
dark_mask float (encoding,video)

    压缩暗区
idct integer (decoding/encoding,video)

    选择实施的IDCT

    可能值:

    ‘auto’
    ‘int’
    ‘simple’
    ‘simplemmx’
    ‘simpleauto’

        自动应用一个兼容的IDCT
    ‘arm’
    ‘altivec’
    ‘sh4’
    ‘simplearm’
    ‘simplearmv5te’
    ‘simplearmv6’
    ‘simpleneon’
    ‘simplealpha’
    ‘ipp’
    ‘xvidmmx’
    ‘faani’

        浮点AAN IDCT 

slice_count integer  
ec flags (decoding,video)

    设置错误隐藏策略

    可能值:

    ‘guess_mvs’

        运动矢量迭代 (MV)搜索(慢/slow) 
    ‘deblock’

        对损坏的MBs使用强壮的去块滤波
    ‘favor_inter’

        有利用从前帧预测而不是当前帧预测 

bits_per_coded_sample integer  
pred integer (encoding,video)

    设置预测方法

    可能值:

    ‘left’
    ‘plane’
    ‘median’

aspect rational number (encoding,video)

    设置样本纵横比
debug flags (decoding/encoding,audio,video,subtitles)

    输出特定调试信息.

    可能值:

    ‘pict’

        图片相关信息 
    ‘rc’

        码率控制 
    ‘bitstream’
    ‘mb_type’

        宏块 (MB)类型 
    ‘qp’

        每个块的量化参数(QP) 
    ‘mv’

        运动矢量 
    ‘dct_coeff’
    ‘skip’
    ‘startcode’
    ‘pts’
    ‘er’

        错误识别 
    ‘mmco’

        内存管理控制操作(H.264) 
    ‘bugs’
    ‘vis_qp’

        量化参数可视化 (QP),即低的QP显示为绿色 
    ‘vis_mb_type’

        块类型可视化 
    ‘buffers’

        图像缓冲区分配 
    ‘thread_ops’

        线程操作 
    ‘nomc’

        跳跃运动补偿 

vismv integer (decoding,video)

    运动矢量可视化 (MVs).

    这个选项是过时的，参考`codecview`滤镜.

    可能值:

    ‘pf’

        p帧前测MVs 
    ‘bf’

        B帧前测MVs 
    ‘bb’

        B帧后测MVs 

cmp integer (encoding,video)

    设置完整图元me压缩功能

    可能值:

    ‘sad’

        绝对差异总和, 最快(默认) 
    ‘sse’

        误差平方和 
    ‘satd’

        绝对Hadamard转换差异总和
    ‘dct’

        绝对DCT转换差异总和 
    ‘psnr’

        量化误差平方和 (avoid, 低品质) 
    ‘bit’

        对特定块需要的比特数 
    ‘rd’

        动态失真优化, 最慢
    ‘zero’

        0 
    ‘vsad’

        绝对垂直差异总和 
    ‘vsse’

        垂直差异平方和 
    ‘nsse’

        噪声保护的差的平方和
    ‘w53’

        5/3 小波（变换）, 只用于下雪场景 
    ‘w97’

        9/7 小波（变换）, 只用于下雪场景 
    ‘dctmax’
    ‘chroma’

subcmp integer (encoding,video)

    设置局部图元me压缩功能

    可能值:

    ‘sad’

        绝对差异总和, 最快(默认) 
    ‘sse’

        误差平方和 
    ‘satd’

        绝对Hadamard转换差异总和
    ‘dct’

        绝对DCT转换差异总和 
    ‘psnr’

        量化误差平方和 (avoid, 低品质) 
    ‘bit’

        对特定块需要的比特数 
    ‘rd’

        动态失真优化, 最慢
    ‘zero’

        0 
    ‘vsad’

        绝对垂直差异总和 
    ‘vsse’

        垂直差异平方和 
    ‘nsse’

        噪声保护的差的平方和
    ‘w53’

        5/3 小波（变换）, 只用于下雪场景 
    ‘w97’

        9/7 小波（变换）, 只用于下雪场景 
    ‘dctmax’
    ‘chroma’

mbcmp integer (encoding,video)

    设置宏块压缩功能

    可能值:

    ‘sad’

        绝对差异总和, 最快(默认) 
    ‘sse’

        误差平方和 
    ‘satd’

        绝对Hadamard转换差异总和
    ‘dct’

        绝对DCT转换差异总和 
    ‘psnr’

        量化误差平方和 (avoid, 低品质) 
    ‘bit’

        对特定块需要的比特数 
    ‘rd’

        动态失真优化, 最慢
    ‘zero’

        0 
    ‘vsad’

        绝对垂直差异总和 
    ‘vsse’

        垂直差异平方和 
    ‘nsse’

        噪声保护的差的平方和
    ‘w53’

        5/3 小波（变换）, 只用于下雪场景 
    ‘w97’

        9/7 小波（变换）, 只用于下雪场景 
    ‘dctmax’
    ‘chroma’

ildctcmp integer (encoding,video)

    设置隔行dct压缩功能

    可能值:

    ‘sad’

        绝对差异总和, 最快(默认) 
    ‘sse’

        误差平方和 
    ‘satd’

        绝对Hadamard转换差异总和
    ‘dct’

        绝对DCT转换差异总和 
    ‘psnr’

        量化误差平方和 (avoid, 低品质) 
    ‘bit’

        对特定块需要的比特数 
    ‘rd’

        动态失真优化, 最慢
    ‘zero’

        0 
    ‘vsad’

        绝对垂直差异总和 
    ‘vsse’

        垂直差异平方和 
    ‘nsse’

        噪声保护的差的平方和
    ‘w53’

        5/3 小波（变换）, 只用于下雪场景 
    ‘w97’

        9/7 小波（变换）, 只用于下雪场景 
    ‘dctmax’
    ‘chroma’

dia_size integer (encoding,video)

    设置运动估计区域类型和尺寸
last_pred integer (encoding,video)

    设置从前帧预测运动量
preme integer (encoding,video)

    设置预运动估计
precmp integer (encoding,video)

    设置预运动估计压缩功能

    可能值:

    ‘sad’

        绝对差异总和, 最快(默认) 
    ‘sse’

        误差平方和 
    ‘satd’

        绝对Hadamard转换差异总和
    ‘dct’

        绝对DCT转换差异总和 
    ‘psnr’

        量化误差平方和 (avoid, 低品质) 
    ‘bit’

        对特定块需要的比特数 
    ‘rd’

        动态失真优化, 最慢
    ‘zero’

        0 
    ‘vsad’

        绝对垂直差异总和 
    ‘vsse’

        垂直差异平方和 
    ‘nsse’

        噪声保护的差的平方和
    ‘w53’

        5/3 小波（变换）, 只用于下雪场景 
    ‘w97’

        9/7 小波（变换）, 只用于下雪场景 
    ‘dctmax’
    ‘chroma’

pre_dia_size integer (encoding,video)

    设置运动估计预测的区域类型和尺寸
subq integer (encoding,video)

    设置子图元运动估计质量
dtg_active_format integer
me_range integer (encoding,video)

    设置运动矢量极限范围 (DivX是1023).
ibias integer (encoding,video)

    设置组内量化偏差
pbias integer (encoding,video)

    设置集间量化偏差
color_table_id integer
global_quality integer (encoding,audio,video)
coder integer (encoding,video)

    可能值:

    ‘vlc’

        可变长编码 / huffman编码 
    ‘ac’

        算术编码 
    ‘raw’

        raw (不进行编码) 
    ‘rle’

        游程长度编码 
    ‘deflate’

        紧缩编码 

context integer (encoding,video)

    设置上下文模型
slice_flags integer
xvmc_acceleration integer
mbd integer (encoding,video)

    设置宏块选择算法 (高质量模式).

    可能值:

    ‘simple’

        使用mbcmp，宏块比较 (默认) 
    ‘bits’

        减少数据量
    ‘rd’

        失真率优化 

stream_codec_tag integer
sc_threshold integer (encoding,video)

    设置场景变化阀值
lmin integer (encoding,video)

    设置最小拉格朗日(lagrange)因子(VBR).
lmax integer (encoding,video)

    设置最大拉格朗日(lagrange)因子 (VBR).
nr integer (encoding,video)

    设置降噪
rc_init_occupancy integer (encoding,video)

    设置解码开始前需加载到RC缓冲区的数据量
flags2 flags (decoding/encoding,audio,video)

    可能值:

    ‘fast’

        允许不符合规范的加速技巧。
    ‘sgop’

        失效, 使用mpegvideo私有选项 
    ‘noout’

        跳过比特流编码 
    ‘ignorecrop’

        忽略sps传来的遮蔽信息 
    ‘local_header’

        在全局头而不是每个关键帧放置扩展数据 
    ‘chunks’

        帧数据可被分割成多个块
    ‘showall’

        显示第一个关键帧前的所有帧（一般用于跳跃定位后的播放，默认是从最近关键帧开始显示，因为之前的帧不一定能够正确构建） 
    ‘skiprd’

        失效,使用了mpegvideo私有选项. 
    ‘export_mvs’

        支持它的编码中运动矢量导出给帧间数据(见`AV_FRAME_DATA_MOTION_VECTORS`) 。参考` doc/examples/export_mvs.c` 

error integer (encoding,video)
qns integer (encoding,video)

    失效, 使用了mpegvideo私有选项
threads integer (decoding/encoding,video)

    可能值:

    ‘auto’

        检测使用一个合适的线程数 

me_threshold integer (encoding,video)

    设置运动估计的阀值
mb_threshold integer (encoding,video)

    设置宏块阀值
dc integer (encoding,video)

    设置intra_dc_precision.
nssew integer (encoding,video)

    设置nsse权重.
skip_top integer (decoding,video)

    跳过顶部设置多个宏块行
skip_bottom integer (decoding,video)

    跳过底部设置多个宏块行
profile integer (encoding,audio,video)

    可能值:

    ‘unknown’
    ‘aac_main’
    ‘aac_low’
    ‘aac_ssr’
    ‘aac_ltp’
    ‘aac_he’
    ‘aac_he_v2’
    ‘aac_ld’
    ‘aac_eld’
    ‘mpeg2_aac_low’
    ‘mpeg2_aac_he’
    ‘mpeg4_sp’
    ‘mpeg4_core’
    ‘mpeg4_main’
    ‘mpeg4_asp’
    ‘dts’
    ‘dts_es’
    ‘dts_96_24’
    ‘dts_hd_hra’
    ‘dts_hd_ma’

level integer (encoding,audio,video)

    可能值:

    ‘unknown’

lowres integer (decoding,audio,video)

    在 1= 1/2, 2=1/4, 3=1/8 解码.
skip_threshold integer (encoding,video)

    设置跳帧阀值
skip_factor integer (encoding,video)

    设置跳帧因子
skip_exp integer (encoding,video)

    设置跳帧指数。 负值和正值除了归一化原因以外表现相同。正值存在的原因主要是兼容性，所以并不常见
skipcmp integer (encoding,video)

    设置跳帧压缩功能

    可能值:

    ‘sad’

        绝对差异总和, 最快(默认) 
    ‘sse’

        误差平方和 
    ‘satd’

        绝对Hadamard转换差异总和
    ‘dct’

        绝对DCT转换差异总和 
    ‘psnr’

        量化误差平方和 (avoid, 低品质) 
    ‘bit’

        对特定块需要的比特数 
    ‘rd’

        动态失真优化, 最慢
    ‘zero’

        0 
    ‘vsad’

        绝对垂直差异总和 
    ‘vsse’

        垂直差异平方和 
    ‘nsse’

        噪声保护的差的平方和
    ‘w53’

        5/3 小波（变换）, 只用于下雪场景 
    ‘w97’

        9/7 小波（变换）, 只用于下雪场景 
    ‘dctmax’
    ‘chroma’

border_mask float (encoding,video)

    增加接近边界宏块量化。
mblmin integer (encoding,video)

    设置最小的宏块的拉格朗日(lagrange)因子(VBR).
mblmax integer (encoding,video)

    设置最大的宏块的拉格朗日(lagrange)因子 (VBR).
mepc integer (encoding,video)

    设置运动估计比特率损失补偿(1.0 = 256).
skip_loop_filter integer (decoding,video)
skip_idct integer (decoding,video)
skip_frame integer (decoding,video)

    让解码器丢弃处理由选项值指定的帧类型

    skip_loop_filter 跳过循环帧   
	skip_idct 跳过IDCT/量化(dequantization）帧   
	skip_frame 跳过解码

    可能值:

    ‘none’

        不抛弃帧
    ‘default’

        抛弃无用帧，例如尺寸为0的帧
    ‘noref’

        抛弃所有非参考帧
    ‘bidir’

        抛弃所有双向(预测)帧
    ‘nokey’

        除了关键帧外都抛弃
    ‘all’

        抛弃所有帧 

    默认值就是‘default’.
bidir_refine integer (encoding,video)

    细化两个运动矢量用于双向宏块
brd_scale integer (encoding,video)

    对动态B帧判定是否下变换
keyint_min integer (encoding,video)

    设置IDR帧集的最小间隔
refs integer (encoding,video)

    为运动补偿设置参考帧 compensation.
chromaoffset integer (encoding,video)

    设置色度中qp对亮度的抵消
trellis integer (encoding,audio,video)

    设置比率失真优化
sc_factor integer (encoding,video)

    设置一个值（一个补偿因子）乘以`qscale`添加到每一帧的`scene_change_score`
mv0_threshold integer (encoding,video)
b_sensitivity integer (encoding,video)

    调整`b_frame_strategy`敏感性为1.
compression_level integer (encoding,audio,video)
min_prediction_order integer (encoding,audio)
max_prediction_order integer (encoding,audio)
timecode_frame_start integer (encoding,video)

    设置GOP时间码帧开始数,非丢帧格式
request_channels integer (decoding,audio)

    设置所需数字音频轨道/通道
bits_per_raw_sample integer
channel_layout integer (decoding/encoding,audio)

    可能值: 
request_channel_layout integer (decoding,audio)

    可能值: 
rc_max_vbv_use float (encoding,video)  
rc_min_vbv_use float (encoding,video)  
ticks_per_frame integer (decoding/encoding,audio,video)  
color_primaries integer (decoding/encoding,video)  
color_trc integer (decoding/encoding,video)  
colorspace integer (decoding/encoding,video)  
color_range integer (decoding/encoding,video)  
chroma_sample_location integer (decoding/encoding,video)  
log_level_offset integer  

    设置日志层次
slices integer (encoding,video)

    设置划片数，用于并行编码
thread_type flags (decoding/encoding,video)

    选择多线程方式

    使用‘frame’会导致每个线程解码延迟，所以如果客户端不提供未来帧状况就不应该使用

    可能值:

    ‘slice’

        每次解码不超过一个帧的多块数据

        划片多线程只用于视频划片编码工作
    ‘frame’

        一次解码多个帧 

    默认值是 ‘slice+frame’.
audio_service_type integer (encoding,audio)

    设置音频服务类型。

    可能值:

    ‘ma’

        主要音频服务 
    ‘ef’

        特效 
    ‘vi’

        视障 
    ‘hi’

        听障 
    ‘di’

        对话 
    ‘co’

        评论 
    ‘em’

        紧急情况 
    ‘vo’

        画外音 
    ‘ka’

        卡拉OK 

request_sample_fmt sample_fmt (decoding,audio)

    设置音频解码偏好。默认是none
pkt_timebase rational number
sub_charenc encoding (decoding,subtitles)

    设置输入的字幕字符编码
field_order field_order (video)

    设置/覆盖场序。可能值是：

    ‘progressive’

        逐行 
    ‘tt’

        隔行，顶场优先编码/显示
    ‘bb’

        隔行，底场优先编码/显示
    ‘tb’

        隔行，顶场优先编码，低场优先显示
    ‘bt’

        隔行，底场优先编码，低场优先显示

skip_alpha integer (decoding,video)

    设置为1来禁止处理透明度。不同的值可以类似一个‘灰色(gray)’蒙在画面上。默认值是0。
codec_whitelist list (input)

    "," 分隔的允许的解码器列表。 默认是都允许
dump_separator string (input)

    指定用于在命令行分隔参数、选项域的字符串。例如可以设置一个回车换行作为分隔:

    ffprobe -dump_separator "
                              "  -i ~/videos/matrixbench_mpeg2.mpg

## 11 解码器
解码器是让FFmpeg能对多媒体流进行解码的配置元素。

默认在编译FFmpeg时所有（内置）有效的解码器都会自动支持。如果解码器需要特别扩展库，则需要手动通过`--enable-lib`选项来进行支持。可以在配置编译项目中通过`--list-decoders`了解所有有效解码器（包括需要扩展库的）。

也可以通过在配置中采用`--disable-decoders`选项单独禁用某个解码器。`--enable-decoder=DECODER / --disable-decoder=DECODER`分别是启用/禁用`DECODER`解码器。

在`ff**`工具中（ffmpeg/ffplayer 等等）选项`-decoders` 可以显示当前有效的解码器。

## 12 视频解码器
介绍当前可用的一些视频解码器

### rawvideo ###
用于RAW视频解码。即解码rawvideo流。

#### rawvideo解码选项 ####

top top_field_first

    指定输入视频的呈现字段类型

    -1

        步进视频 (默认) 
    0

        下场优先（底部优先） 
    1

        上场优先（顶部优先）


## 13 音频解码器
介绍一些有效的音频解码器

### ac3 ###
AC-3 音频解码器，该解码器实现在ATSC A/52:2010 和 ETSI TS 102 366部分，以及RealAudio 3（又名DNET）中。

#### ac3解码器选项 ####
- `-drc_scale value`

    动态范围因子。该因子适合应用于从AC-3流中获取的动态值范围。这个值是指数值。有3个显著效果的典型值(范围)：

    drc_scale == 0

        DRC禁用，会产生原始全范围音频
    0 < drc_scale <= 1

        DRC启用，在一部分DRC中应用，音频表现为全范围与全压缩之间。 
    drc_scale > 1

        DRC启用。适用于drc_scale不对称场景，表现为响亮的全压缩，轻柔的声音增强。 

### flac ###
FLAC音频解码器，它由Xiph实现了对FLAC的完整规格

#### FLAC解码器选项 ####
- `-use_buggy_lpc`
	
	FLAC编码器用于产生高`lpc`值的数据包流（例如在默认设置下），本选项允许正确解码采用老版本`buggy lpc`逻辑编码的FLAC音频。

### ffwavesynth ###
内部波表合成装置。

该解码器根据预定义的序列产生音频波。其使用纯粹内部不公开的数据格式表征特定的序列（集）

### libcelt ###
libcelt解码器再封装

libcelt允许libavcodec解码Xiph CELT超低延迟音频编码。在需要libcelt头和库存在配置（能进一步搜索到库的配置）。在创建ffmpeg工具集时需要显式采用`--enable-libcelt`以打开支持

### libgsm ###
libgsm解码器再封装

libgsm允许libavcodec解码GSM全速率语音。需要libgsm头和库存在配置（能进一步搜索到库的配置）。需要在创建ffmpeg时显式设置`--enable-libgsm`以启用

该解码器支持普通GSM和微软修订

### libilbc ###
libilbc解码器再封装

libilbc允许libavcodec解码网络低码率编码（iLBC）音频。需要libilbc头和库存在配置。需要在创建ffmpeg时显式设置`--enable-libilbc`以启用

#### libilbc选项 ####
下面的选项被libilbc封装支持
- `enhance`
	
	设置为1，则解码时使音频增强，默认是0（禁用）

### libopencore-amrnb ###
libopencore-amrnb解码器再封装

libopencore-amrnb允许libavcodec解码 自适应多速率窄带（Adaptive Multi-Rate Narrowband）音频。需要libopencore-amrnb头和库存在配置才能使用。需要在创建ffmpeg时显式设置`--enable-libopencore-amrnb`以启用

现在FFmpeg已经直接支持解码AMR-NB，所以不需要该库了。

### libopencore-amrwb ###
libopencore-amrwb解码器再封装

libopencore-amrwb允许libavcodec解码 自适应多速率宽带（Adaptive Multi-Rate Wideband）音频。需要libopencore-amrwb头和库存在配置才能使用。需要在创建ffmpeg时显式设置`--enable-libopencore-amrwb`以启用

现在FFmpeg已经直接支持解码AMR-WB，所以不需要该库了。

### libopus ###
libopus解码器再封装

libopus允许libavcodec解码Opus互动音频。需要libopus头和库存在配置才能使用。需要在创建ffmpeg时显式设置`--enable-libopus`以启用。

现在FFmpeg已经直接支持解码Opus，所以不需要该库了。

## 14 字幕解码器

### dvdsub ###
解码用于dvd的bitmap类型字幕解码。该类型也用于vobsub文件和一些Matroska文件。

#### dvdsub解码选项 ####
- `palette`

    指定位图的全局调色板。当存储在VobSub中时，调色板可以依据索引表示颜色。在Matroska文件中，调色板以同于VobSub的格式存储在扩展数据区。在DVD中，存储在IFO文件中，因此在只读取VOB文件时不可用则。

    选项的格式是24位（bit）数，用16进制表示字符串（没有前导的0x）例如 0d00ee, ee450d, 101010, eaeaea, 0ce60b, ec14ed, ebff0b, 0d617a, 7b7b7b, d1d1d1, 7b2a0e, 0d950c, 0f007b, cf0dec, cfa80c, 7c127b.

- `ifo_palette`

    指定IFO文件中得到全局调色板（实验性质）=

- `forced_subs_only`

    只在解码强制字幕。有些字幕在同一轨道中保留了强制字幕和非强制字幕，选用这个选项将只解码强制字幕，即强制字幕标志设为1的字幕。默认是0 

### libzvbi-teletext ###
libzvbi允许libavcodec解码DVB的teletext页面和DVB的teletext字幕。需要libzvbi头和库存在配置。在编译是需配置` --enable-libzvbi`以启用

#### libzvbi-teletext选项 ####
- `txt_page`

    列出需要解码的teletext页编号。你可以用`*`表示所有页，没有匹配的页都被丢弃。默认是`*` 
- `txt_chop_top`

    取消顶部teletext行。默认是 1. 
- `txt_format`

    指定解码字幕的格式。这个teletext解码器可解码位图或简单文本，如果页面中有特定的图形和颜色则可能需要设置为"bitmap"，因为简单文本不能包含这些内容。否则可以选用"text"以简化处理。默认是bitmap
- `txt_left`

    位图X偏移量, 默认是 0. 
- `txt_top`

    位图Y偏移量, 默认是 0. 
- `txt_chop_spaces`

    在生成的文本中保留前导和尾随的空格去除空行。该选项常用于字幕需要显示为多行（双行）使得字幕放置的更好看。 默认是1. 
- `txt_duration`

    teletex页面或者字幕显示的时间，单位是ms，默认值是3000，即30秒
- `txt_transparent`

    让生成的teletext位图透明，默认是0，表示有一个不透明（黑）的背景色。 

## 15 编码器
编码器是ffmpeg用来编码多媒体流的配置单元。

当编译生成ffmpeg时，所有内置编码器默认被支持。可以通过手动设置`--enable-lib`选项以支持外部（扩展）库。可以在配置选项中利用` --list-encoders`了解所有可能的编码器

可以利用`--disable-encoders`禁用所有编码器，也可以单独的利用 `--enable-encoder=ENCODER / --disable-encoder=ENCODER`启用/禁用个别的编码器。

在ffmpeg工具集中利用选项` -encoders`可以了解支持的编码器。
## 16 音频编码器
介绍当前可用的音频编码器

### aac ###
AAC（Advanced Audio Coding ）编码器

当前原生（内置）编码器还处于实验阶段，而且只能支持AAC-LC（低复杂度AAC）。要使用这个编码器，必须选择 ‘experimental’或者'lower'

因为当前还处于实验期，所以很多意外可能发生。如果需要一个更稳定的AAC编码器，参考`libvo-aacenc`,然而它也有一些负面报告。

#### aac选项 ####

b

    设置码率，单位是bits/s，是自动恒定比特率(CBR)模式的码率
q

    设置为可变比特率（VBR）模式。此选项仅用于ffmpeg命令行工具。库接口是`global_quality`
stereo_mode

    设置立体声编码模式，可能值有:

    ‘auto’

        在编码时自动判断
    ‘ms_off’

        禁止中端的（即2.0声道，而不是2.1声道）编码，这时默认值
    ‘ms_force’

        强制中端编码，即强制2.1声道 encoding. 

aac_coder

    设置AAC编码方法，可能值:

    ‘faac’

        FAAC-启发方法.

        这个方法是显示方法的简化实现版，它为频带能量比设置阀值，降低所有量化步骤找到合适的量化失真阀值，对低于频带阀值的频带进行编码

        这种方法的质量稍微好于下面介绍的两回路搜索法，但很慢
    ‘anmr’

        基于网格的ANMR（平均噪音Average noise to mask ratio)掩比方案

        理论上它效果最好，但最慢
    ‘twoloop’

        双环搜索(Two loop searching)法

        该方法首先根据波段阀值量化并试图通过添或调整个别量化点减去一个特征值得到一个最佳组合

        这种方法和FAAC方法质量相当，是默认值
    ‘fast’

        固定量化法

        该方法设置所有带定量化，这是最快的方法，但质量最差

### ac3和ac3修订版 ###
AC-3音频编码器

这一编码器定义在 ATSC A/52:2010 和ETSI TS 102 366,以及RealAudio 3 (通过dnet)

AC3编码器使用浮点运算，而ac3_fixed编码器仅用定点整数的数学运算。这并不意味着一个人总是更快，只是一个或另一个可能更适合一个特定的系统。浮点编码通常会产生一个给定的比特率，更好的音频质量。ac3_fixed编码器没有任何输出格式的默认编码，所以它必须显式使用选项`-acodec ac3_fixed`指定。

#### AC-3元数据 ####
AC-3元数据选项用于设置音频参数的描述，它们大多数情况下不影响音频编码本身。这些选项不直接影响比特流或影响解码播放，只是提供信息。几个选项会增加音频数据比特输出流，从而影响输出质量。这将在下面的选项列表中注记出来：

下面文档介绍了几个公开文件文档：

- [A/52:2010 Digital Audio Compression (AC-3) (E-AC-3) Standard](http://www.atsc.org/cms/standards/a_52-2010.pdf)
- [A/54 Guide to the Use of the ATSC Digital Television Standard](http://www.atsc.org/cms/standards/a_54a_with_corr_1.pdf)
- [Dolby Metadata Guide](http://www.dolby.com/uploadedFiles/zz-_Shared_Assets/English_PDFs/Professional/18_Metadata.Guide.pdf)
- [Dolby Digital Professional Encoding Guidelines](http://www.dolby.com/uploadedFiles/zz-_Shared_Assets/English_PDFs/Professional/46_DDEncodingGuidelines.pdf) 

##### AC-3元数据控制选项 #####

- `-per_frame_metadata boolean`

    允许每个框架的元数据。指定编码器应该检测每帧变化的元数据

    0

        初始化的元数据用于每帧（不再管变化，默认) 
    1

        每帧都要检测元数据改变 

##### AC-3中置混合水平 #####
- `-center_mixlev level`

	AC-3中置混合水平。该值决定编码时根据立体声产生中置音量的标准。它只会写入存在中置通道的输出中。该值为规模因子，有3个有效值:

    0.707

        应用-3dB增益 
    0.595

        应用-4.5dB增益(默认值) 
    0.500

        应用-6dB增益

- `-surround_mixlev level`

    环绕混合水平。适用于环绕声道增益。它只会写入存在环绕声通道的输出中。该值为规模因子，有3个有效值:

    0.707

        应用-3dB增益 
    0.500

        应用-6dB增益(默认值) 
    0.000

        静默环绕声道(即没有环绕) 

##### AC-3音频制作信息 #####
音频制作信息描述了可选的混合环境信息，应用中要么都没有，要么同时有两个（即下面两个需要同时设置/或不设置）


- `-mixing_level number`

    混合水平. 指定在环境中混合的峰值声压级（SPL-Specifies peak sound pressure level）。 有效值是80 - 111或者-1（表示未知）或不指定。 默认值为-1，但如果`room_type`不为默认值，则`mixing_level`不能为-1.
- `-room_type type`

    空间类型。介绍了混音环境。是按大房间还是按小房间。如果没有指定`mixing_level`则写入默认值

    0
    notindicated

        没有指定 (默认) 
    1
    large

        大房间 
    2
    small

        小房间 

##### 其他AC-3元数据选项 #####



- `-copyright boolean`

    版权指示。

    0
    off

        不包含 版权(默认iansbaq) 
    1
    on

        版权信息 

- `-dialnorm value`

    对话常态化。表明对于低于平均值的音量保持原样（0dBFS）。 这个参数决定了匹配源的目标音量。值过小会导致相对于源没有变化。有效值为整数，范围为-31至-1，-31是默认值。
-dsur_mode mode

    杜比环绕模式。指定是否使用杜比环绕立体声信号。只对音频流是立体声的输出有效。使用了这个选项并不意味着实际处理会产生杜比环绕。

    0
    notindicated

        未指定 (默认) 
    1
    off

        不采用 
    2
    on

        采用杜比环绕解码 

-original boolean

    原始流指示器。指音频是原始源而不是副本。

    0
    off

        非原始源 
    1
    on

        原始源 (默认) 

#### 其他扩展比特流信息 ####
这些扩展比特流选项都被定义在A/52:2010标准的附录D中。它分为2个部分（组）。如果组中任意一个参数被指定，则组中所有的值将以默认值写入到流中。如果`mixing levels`被设置，则对支持备用比特流语法（ Alternate Bit Stream Syntax）的解码器将采用这个值以替代`center_mixlev`和`surround_mixlev`选项定义。

##### 其他扩展比特流信息 第一部分 #####
- `-dmix_mode mode`

    优化立体声缩混模式。允许在Lt/Rt (杜比环绕)或者Lo/Ro (常规立体声) 作为优化立体声缩混模式

    0
    notindicated

       	未指定(默认) 
    1
    ltrt

        Lt/Rt 缩混优化 
    2
    loro

        Lo/Ro 缩混优化 

- `-ltrt_cmixlev level`

    Lt/Rt模式下中置混合层次。在Lt/Rt模式下解码器输出中置通道的增益

    1.414

        使用+3dB增益 
    1.189

        使用 +1.5dB增益
    1.000

        使用 0dB  
    0.841

        使用 -1.5dB  
    0.707

        使用 -3.0dB  
    0.595

        使用 -4.5dB 增益 (默认值) 
    0.500

        使用 -6.0dB  
    0.000

        禁用中置通道 


- `-ltrt_surmixlev level`

    Lt/Rt模式下环绕增益。在Lt/Rt模式下解码器输出环绕通道的增益

    0.841

        使用 -1.5dB  
    0.707

        使用 -3.0dB  
    0.595

        使用 -4.5dB 增益 (默认值) 
    0.500

        使用 -6.0dB  
    0.000

        禁用环绕通道  


- `-loro_cmixlev level`

    Lo/Ro模式下中置混合层次。在Lo/Ro模式下解码器输出中置通道的增益.

    1.414

        使用+3dB增益 
    1.189

        使用 +1.5dB增益
    1.000

        使用 0dB  
    0.841

        使用 -1.5dB  
    0.707

        使用 -3.0dB  
    0.595

        使用 -4.5dB  
    0.500

        使用 -6.0dB  增益(默认值) 
    0.000

        禁用中置通道 


- `-loro_surmixlev level`

    Lo/Ro模式下中置混合层次。在Lo/Ro模式下解码器输出环绕通道的增益.

    0.841

        使用 -1.5dB  
    0.707

        使用 -3.0dB  
    0.595

        使用 -4.5dB  
    0.500

        使用 -6.0dB  增益(默认值) 
    0.000

        禁用环绕通道  

##### 其他扩展比特流信息 第二部分 #####
- `-dsurex_mode mode`

    Dolby环绕EX模式. 标识是否使用Dolby环绕EX模式（7.1矩阵转5.1）.使用了此选项并不意味着编码器将实际应用Dolby环绕EX模式

    0
    notindicated

        未标识 (default) 
    1
    on

        Dolby环绕EX模式关闭 
    2
    off

        Dolby环绕EX模式打开 

- `-dheadphone_mode mode`

    杜比耳机模式。标识编码为杜比耳机（多通道矩阵合成为2个声道）使用这个选项并不意味着实际应用了杜比耳机模式。

    0
    notindicated

        未标识 (default)
    1
    on

        Dolby 耳机模式关闭
    2
    off

        Dolby 耳机模式打开



- `-ad_conv_type type`

    A/D（模数转换） 转换类型。标识音频需要HDCD A/D 转换。

    0
    standard

        标准 A/D转换 (默认) 
    1
    hdcd

        HDCD A/D 转换 

#### 其他AC-3编码器选项 ####

- `-stereo_rematrixing boolean`

    Stereo 再混（Rematrixing）。通过Enables/Disables 来对应立体声输入。它是可选功能，通过选择左/右而当作立体声输出，从而提高输出效果。默认是启用的。因为该编码器会增加程序现象，所以只建议用于测试。

#### 浮点AC-3编码特有选项 ####
这些选项只在浮点AC-3编码时有效，整形AC-3时是不起作用的。

- `-channel_coupling boolean`

    Enables/Disables通道的耦合。这是一个可选的音频选项，它从多个通道中获取高频带信息整合输出到一个通道中。这允许更多的比特位用于较低频率音频的同时保持足够的信息重建高频部分。这个选项对浮点AC-3来说主要用于测试或者提高编码速度。

    -1
    auto

        由编码器选择 (默认) 
    0
    off

        禁用通道耦合 
    1
    on

        允许通道耦合

- `-cpl_start_band number`

    耦合开始带。设置通道耦合的开始带，从1-15可选。如果设置的值大于通道数，则处理为需耦合最后通道减一。如果auto（自动）被设置，则开始带将根据码率、通道布局、采样率有编码器自动计算。如果通道耦合设置为禁用，则本选项失效。

    -1
    auto

        由编码器 选择(默认) 

### flac ###
FLAC(自由低损失音频编码——Free Lossless Audio Codec)编码器
#### flac选项 ####
下面是FFmpeg中flac编码可用选项
- `compression_level`

    设置压缩级别，如果没有显式设置即采用默认值

- `frame_size`

    设置各个通道的帧大小

- `lpc_coeff_precision`

    设置LPC系数精度，有效值从1到15, 15是默认值

- `lpc_type`

    设置第一阶段LPC算法

    ‘none’

        不采用LPC
    ‘fixed’

        整数LPC
    ‘levinson’
    ‘cholesky’

- `lpc_passes`

    用Cholesky分解LPC的次数

- `min_partition_order`

    最小分区顺序

- `max_partition_order`

    最大分区顺序

- `prediction_order_method`

    ‘estimation’
    ‘2level’
    ‘4level’
    ‘8level’
    ‘search’

        强力搜索
    ‘log’

- `ch_mode`

    通道模式

    ‘auto’

        自动模式，对每帧自动匹配通道 
    ‘indep’

        通道独立编码 
    ‘left_side’
    ‘right_side’
    ‘mid_side’

- `exact_rice_parameters`

    是精确还是近似.如果设置为1表示精确，会减慢编码速度以提高压缩率。

- `multi_dim_quant`

    多维量化。如果设置为1，那么第二阶段LPC应用第一阶段结果进行算法调整。这很慢，但可以提高压缩率

### libfaac编码 ###
libfaac 是AAC（Advanced Audio Coding）编码器的再封装

要使用它需要libfaac头文件和库存在配置。你还需要在编译ffmpeg时通过`--enable-libfaac` `--enable-nonfree`进行配置。

该编码器高质量版本参考[aacenc]

对更多信息，参考libfaac项目介绍[http://www.audiocoding.com/faac.html/](http://www.audiocoding.com/faac.html/)

#### libfaac相关选项 ####
下面是ffmpeg工具编码时的可用选项

下面的选项适用于libfaac封装，`faac-XXXX`等效选项列在括号中


- b (-b)

    设置ABR（平均码率），单位bits/s。如果码率没有特别指定，会自动匹配所选特性（属性配置）。 faac比特率单位是 kilobits/s.

    **注意**libfaac不支持CBR(Constant Bit Rate——固定码率)，只支持ABR (Average Bit Rate——平均码率).

    如果VBR模式设置为允许，则本选项被忽略
- ar (-R)

    设置音频采样率，单位Hz
- ac (-c)

    设置音频通道数
- cutoff (-C)

    设置截至频率。如果没有设置或者设置为0，则自动根据库计算。默认为0
- profile

    设置音频特性（属性配置）文件

    下面的音频特性文件有效:

    ‘aac_main’

        主要的AAC (Main)
    ‘aac_low’

        低复杂度AAC (LC)
    ‘aac_ssr’

        可扩展采样 (SSR)
    ‘aac_ltp’

        长期预测(LTP——Long Term Prediction ) 

    如果没有指定则表示为‘aac_low’.
- flags +qscale

    设置VBR(动态码率Variable Bit Rate)下的品质
- global_quality

    设置VBR下的品质，其为一个数字或者lambda表达式

    仅仅在VBR模式，且 flags +qscale 被设置为有效才起作用。它将被`FF_QP2LAMBDA`转换成QP值，并应用于libfaac。QP值的范围为[10-500]，越大品质越好
- q (-q)

    允许VBR模式，但设置为负数值，则值作为双精度浮点数

	值应用于libface。值的可能范围是[10-500]，数字越大QP值越高。

	选项只用于ffmpeg命令行，或者通过`global_quality`属性描述问中rs。 

#### libfaac例子 ####
- 使用`ffmpeg`把一个音频转换为ABR 128kbps AAC编码格式流放置在M4A（MP4音频）文件中
    
    ffmpeg -i input.wav -codec:a libfaac -b:a 128k -output.m4a

- 使用`ffmpeg`把一个音频转换为VBR AAC编码（采用LTP AAC）格式流放置在M4A（MP4音频）文件中

	ffmpeg -i input.wav -c:a libfaac -profile:a aac_ltp -q:a 100 output.m4a

### libfdk aac ###libfdk-aac
libfdk-aac的再封装

该库只用于Fraunhofer  FDK AAC 编码格式

要使用，必须有libfdk-aac有头和预配的库，并在编译ffmpeg时用配置选项`--enalbe-libfdk-aac`启用。这个库可能不兼容于GPL，如果你要使用GPL，你必须`--enable-gpl --enable-nonfree --enable-libfdk-aac`

这个编码器被认为品质高于 内置的[AACenc] 和[libfaac]

VBR编码可以通过 `vbr` 或者`flags + qscale`选项启用，它们是实验性质的，只适合于某些参数组合。

libfdk-aac 0.1.3或者更高版本支持7.1声道

更多信息请参考fdk-aac项目[http://sourceforge.net/p/opencore-amr/fdk-aac/](http://sourceforge.net/p/opencore-amr/fdk-aac/)

#### libfdk-aac选项 ####
下面是可用的一些选项：


- b

    设置码率，如果未指定，则根据属性特性自动匹配

    如果工作于VBR模式，本选项被忽略、
- ar

    设置采样率 (单位Hz).
- channels

    设置通道数
- flags +qscale

    可以调整品质，VBR (Variable Bit Rate)模式。**注意** 当vbr为正表示VBR模式被隐含启用
- cutoff

    设置截止频率，如果没有设置或者为0，表示自动根据库计算默认为0
- profile

    设置音频属性预设文件，可以有：

    ‘aac_low’

        低复杂度AAC(LC)
    ‘aac_he’

        高效率AAC (HE-AAC)
    ‘aac_he_v2’

        高效率AAC版本2 (HE-AACv2)
    ‘aac_ld’

        低延迟AAC(LD)
    ‘aac_eld’

        增强低延迟AAC (ELD) 

    如果没有特别指定则为‘aac_low’. 

下面是libfdk_aac私有选项

- afterburner

    设置为1表示允许助力，否则为0，它可以增强品质，但要求更多处理能力。

    默认为1
- eld_sbr

    1表示允许对ELD采样SBR (Spectral Band Replication-频带复制)，否则为 0.

    默认为 0
- signaling

    设置SBR/PS 指令方式

    接受下面的值：

    ‘default’

        选择含蓄信号模式 (默认明确为 hierarchical, 如果全局头设置为禁止，则隐式表达)
    ‘implicit’

        隐式向后兼容指令信号
    ‘explicit_sbr’

        明确为SBR, 隐式PS信号
    ‘explicit_hierarchical’

        明确为hierarchical信号 

    默认为‘default’.
- latm

    设为1表示输出LATM/LOAS封装数据，否则为0

    默认为0
- header_period

    设置StreamMuxConfig 和 PCE 重复周期 (在帧上)， 把LATM/LOAS包含着配置发送缓冲中

    必须为16bit的非负整数

    默认为 0.
- vbr

    设置VBR模式，从1最低品质（但仍足够好）, 5是最高品质。如果值为0表示禁用VBR，而是采用CBR（固定码率）

    当前只有 ‘aac_low’属性预设支持VBR

    VBR模式1-5代表的平均码率:

    ‘1’

        32 kbps/channel 
    ‘2’

        40 kbps/channel 
    ‘3’

        48-56 kbps/channel 
    ‘4’

        64 kbps/channel 
    ‘5’

        about 80-96 kbps/channel 

    默认0. 

#### libfdk_aac 例子 ####
- 转换为VBR AAC M4A

    ffmpeg -i input.wav -codec:a libfdk_aac -vbr 3 output.m4a
- 转换为CBR 64k AAC，使用高效率AAC属性预设
    
    ffmpeg -i input.wav -c:a libfdk_aac -profile:a aac_he -b:a 64k output.m4a

### libmp3lame ###
LAME (Lame Ain’t an MP3 Encoder) MP3 编码器封装

需要在编译时配置 libmp3lame 头和库，并且显式设置-`-enable-libmp3lame`

参考`libshine` 这个整数修正MP3编码器（虽然质量较低）

#### libmp3lame选项 ####
下面是支持的选项（lame-XXX等效选项列在括号中）:


- b (-b)

    设置码率CBR/ABR，单位为bits/s，LAME的码率为kilobits/s\
- q (-V)

    设置VBR下的品质。它只用于ffmpeg命令行工具，对于库接口，使用`global_quality`
- compression_level (-q)

    设置算法品质。通过0-9的参数，表示不同的品质，0最高但最慢，9最低但最快
- reservoir

    为1（默认）表示允许bit池，否则为0. LAME 也是默认允许但可以被`--nores`覆盖
- joint_stereo (-m j)

    1表示允许在（每帧）中编码L/R立体声或者mid/side立体声。默认为1 Default value is 1.
- abr (--abr)

    为1表示允许ABR，lame `--abr`设置为一共的码率，这里只是表示采用ABR，码率还是由`b`设置

### libopencore-amrnb ###
开放核心自适应多速率窄带(OpenCORE Adaptive Multi-RateNarrowband)编码器

需要相应的头和库进行编译，并利用 ` --enable-libopencore-amrnb --enable-version3`以允许编译配置

是单声道编码器，常用于8k采样率，可以通过设置`strict` 到 `unofficial`或者更低来选用更低采样率

#### libopencore-amrnb选项 ####

- b

    设置码率，只有下面的码率被支持，设置为其他值将自动用最近的替代

    4750
    5150
    5900
    6700
    7400
    7950
    10200
    12200

- dtx

    设置为1表示允许连续传输 (产生少量噪音)，默认为0

### libshine ###
Shine整形Mp3编码的封装

Shine是一种整形MP3编码器，它在没有FPU（浮点协处理）的平台上可以更快更好，例如一些armel CPU或者一些电话或者平板上。但是不要期望获得更好的品质（与LAME或者其他产品级编码器比较）。同时，根据项目主页，该编码器可能并不提供给免费bug修正，代码是很久以前写的，已经有5年以上没有更新了。

只支持立体声和单声道，而且是CBR模式。

项目在（最后更新2007年）[http://sourceforge.net/projects/libshine-fxp](http://sourceforge.net/projects/libshine-fxp)，我们的支持更新放置在github上的Savonet/Liquidsoap中，地址是[https://github.com/savonet/shine](https://github.com/savonet/shine)

需要头和库支持，并需要配置`--enable-libshine`打开编译

参考[libmp3lame]

#### libshine选项 ####
这个库封装支持如下选项，其对应的` shineenc-XXXXX`形式等效选项列在括号中
- b (-b)
	
	设置CBR码率，单位bits/s ,`shineenc -b` 单位是kilobits/s


### libtwolame ###
双Lame Mp2 编码器封装

编译需要头和库，并且显式打开`--enable-libtwolame`

#### libtwolame选项 ####
下面是支持的选项，等效的`libtwolame-XXX`选项列在括号中


- b (-b)

    设置CBR码率单位bits/s，twolame会扩展为以kilobits/s为单位。默认128k
- q (-V)

    对VBR设置品质等级，从-50 至50，常见范围为-10-10.越高品质越好。只适用于ffmpeg命令行，接口需要使用`global_quality`.
- mode (--mode)

    设置结果音频模式，允许如下参数:

    ‘auto’

        基于输入自动适配模式，选项默认值. 
    ‘stereo’

        立体声 
    ‘joint_stereo’

        Joint立体声 
    ‘dual_channel’

        双声道 
    ‘mono’

        单声道 

- psymodel (--psyc-mode)

    为1设置为psychoacoustic（心理声学）模式，接受-1到4的参数，越大效果越好，默认为3
- energy_levels (--energy)

    为1设置能量扩展模式，否则为0（默认） (disabled).
- error_protection (--protect)

    为1设置CRC错误保护，否则为0（默认）
- copyright (--copyright)

    为1设置MPEG音频复制标志，否则为0（默认）
- original (--original)

    为1设置MPEG音频原音标志，否则为0（默认）


### libvo-aacenc ###
VisualOn AAC编码器

编译时需要头和库文件，以及利用配置选项`--enable-libvo-aacenc --enable-version3`打开

它类似于[原生FFmpeg AAC],可以处理多个源

#### libvo-aacenc选项 ####
VisualOn AAC编码器只支持AAC-LC和最多2个声道，而且是CBR
- b

	码率，单位秒

### libvo-amrwbenc ###
VisualOn 自适应多速率宽带编码器

编译时需要头和库文件，以及利用配置选项`--enable-libvo-amrwbenc --enable-version3`打开

只支持单声道，通常为16000Hz采样，可以通过设置`strict` 和 `unofficial`来覆盖为更低采样

#### libvo-amrwbenc选项 ####

- b

    设置码率，单位bits/s，只允许下列参数，否则自动选取最接近的有效参数

    ‘6600’
    ‘8850’
    ‘12650’
    ‘14250’
    ‘15850’
    ‘18250’
    ‘19850’
    ‘23050’
    ‘23850’

- dtx

    为1允许连续传输 (产生少量噪音)，默认为0


### libopus ###
libopus （Opus交互音频编码） 的封装

编译时需要头和库文件，以及利用配置选项`--enable-libopus `打开

#### libopus ####
更多选项可以通过opus-tools的 `opusenc`查询，下面仅仅是一些封装中支持的选项（对应的`opusenc-XXXX`选项列在括号中）：


- b (bitrate)

    设置码率，单位 bits/s, opusenc 中单位为kilobits/s.
- vbr (vbr, hard-cbr, and cvbr)

    设置VBR模式，下面为有效参数，其等效于opusenc中对应参数:

    ‘off (hard-cbr)’

        使用CBR码率控制
    ‘on (vbr)’

        使用合适的动态码率（默认）
    ‘constrained (cvbr)’

        使用约束变比特率编码

- compression_level (comp)

    设置集编码算法复杂度. 有效参数是0-10整数，0最快，但质量最差，10最慢，质量最好，默认为10
- frame_duration (framesize)

    设置最大帧尺寸，或者帧对应毫秒时间。有效参数为: 2.5, 5, 10, 20, 40, 60,越小的帧延迟越低，但会降低编码率控制质量，尺寸大于20ms在低码率时有较有趣表现，默认20ms
- packet_loss (expect-loss)

    设置预期分组丢失率，默认为0
- application (N.A.)

    设置预期应用类型，下面为有效参数：

    ‘voip’

        有利于提高语音清晰度
    ‘audio’

        有利于音频输入，默认值
    ‘lowdelay’

        有利于低延迟模式

- cutoff (N.A.)

    设置截止屏幕，单位Hz。参数必须是: 4000, 6000, 8000, 12000, 或者 20000（分别对应媒体带宽窄带、常规、宽带、超宽带和全频），默认为0，表示禁用cutoff。

### libvorbis ###
libvorbis编码器封装

编译要求头文件和库，还需要专门用`--enable-libvorbis`以允许使用

#### libvorbis选项 ####
下面的选项支持libvorbis封装。等效的`oggenc-XXX`选项部分列在括号中。

为了更多的了解`libvorbis`和`oggenc`选项，请参考[http://xiph.org/vorbis/](http://xiph.org/vorbis/)和[http://wiki.xiph.org/Vorbis-tools](http://wiki.xiph.org/Vorbis-tools)，以及`oggenc(1)`手册


- b (-b)

    设置ABR模式码率，单位bits/s。oggenc-b单位是kilobits/s.
- q (-q)

    设置VBR的品质。选项参数是浮点数，范围-1.0至10.0，越大越好，默认‘3.0’.

    该选项只用于ffmpeg命令行工具，要在库中使用则需要`global_quality`
- cutoff (--advanced-encode-option lowpass_frequency=N)

    设置截止频率，单位Hz，如果为0表示禁用截止频率。`oggenc`等效选项单位是kHz。默认值为‘0’ (表示不设置截止频率)。
- minrate (-m)

    设置最小码率，单位bits/s. oggenc -m 的单位是kilobits/s.选项只在ABR模式起效
- maxrate (-M)

    设置最大码率，单位bits/s. oggenc -M 的单位是kilobits/s. 选项只在ABR模式起效
- iblock (--advanced-encode-option impulse_noisetune=N)

    设置负偏压（底噪偏置），选项参数是浮点数，范围-15.0-0.0。指示编码器花费更多资源用于瞬态，这样可以使得获得更好的瞬态响应。

### libwavpack ###
wavpack的通过libwavpack的封装

当前只支持无损32位整数样本模式

编译要求头文件和库，还需要专门用`--enable-libwavpack`以允许使用

**注意**libavcoder原生编码器已支持wavpack编码，而不用使用这个扩展编码器了。相关参考[wavpackenc]

#### libwavpack选项 ####
wavpack命令行工具相应选项都列在括号中:


- frame_size (--blocksize)

    默认32768.
- compression_level

    设置速度与压缩的平衡，允许下面的参数：

    ‘0 (-f)’

        快速模式.
    ‘1’

        常规模式 (默认) 
    ‘2 (-h)’

        高质量模式
    ‘3 (-hh)’

        非常高质量模式
    ‘4-8 (-hh -xEXTRAPROC)’

        类似‘3’, 但允许扩展处理 enabled.

        ‘4’ 类似于 -x2 ‘8’类似于 -x6.

### wavpack ###
wavpack无损音频压缩

是libavcodec的原生wavpack编码。这个编码也可以利用`libwavpack`完成，但现在看来完全没有必要。

参看[libwavpack]

#### wavpack选项 ####
等效的wavpack命令行工具列在括号中

##### wavpack通用选项 #####
下面是wavpack编码的通用选项，下面只介绍了个别特别用于wavpack的选项，其他更多选项参考[编码选项部分]


- frame_size (--blocksize)

    对于这个编码器，参数范围128 至 131072。默认为自动检测（根据采样率和通道数）

    为了了解完整的计算公式，可以看`libavcodec/wavpackenc.c`
- compression_level (-f, -h, -hh, and -x)

    这个选项同于libwavpack的语法 

##### wavpack私有选项 #####

- joint_stereo (-j)

    设置是否启用联合立体声， 下列值有效:

    ‘on (1)’

        强制mid/side （中置和边）音频编码 
    ‘off (0)’

        强制left/right音频编码 
    ‘auto’

        自动检测 

- optimize_mono

    设置是否允许对单声道优化。此选项只对非单声道流有效。 可能值：

    ‘on’

        允许 
    ‘off’

        禁止 

## 17 视频编码器
介绍一些当前有效的视频编码器

### libtheora ###
libtheora的封装

编译需要头和库文件，还需要利用`--enable-libtheora`在配置中允许

更多信息参考[http://www.theora.org/](http://www.theora.org/)

#### libtheora选项 ####
下面是映射给libtheora的全局选项，它们对品质和码率产生影响。


- b

    对CBR（固定码率编码）设置码率，单位bit/s，在VBR（动态码率编码）模式下本选项被忽略。
- flags

    设置是否允许`qscale标志（恒定质量模式——VBR模式下）在`pass1`和`pass2`（2次编码方式）
- g

    设置GOP尺寸
- global_quality

    设置全局质量，在 lambda工具集中是一个整数单位的倍数

    仅在VBR模式中，同时允许了 +qscale。这个值会除以`FF_QP2LAMBDA转换为QP,范围为[0 - 10]，再乘以6.3得到本地有效libtheora范围[0-63]，越大质量越高
- q

    仅作VBR模式下，设置为非负数。作为双精度浮点质量单位值，用于转换计算QP

    值范围为 [0-10] ，再乘以 6.3 将获得libtheora有效质量参数，范围[0-63]

    这个选项仅用于ffmpeg命令行工具，库接口使用`global_quality` 

#### libtheora例子 ####

- 使用最大恒定质量（VBR）编码:

    ffmpeg -i INPUT -codec:v libtheora -q:v 10 OUTPUT.ogg

- 使用CBR 1000 kbps编码 Theora视频流:

    ffmpeg -i INPUT -codec:v libtheora -b:v 1000k OUTPUT.ogg

### libvpx ###
VP8/VP9格式支持，通过libvpx

编译需要头和库文件，还需要利用`--enable-libvpx`在配置中允许

#### libvpx选项 ####
下面的选项被libvpx封装支持，部分等效的`vpxenc-XXX`类型的选项或者值列在括号中。

为了减少文件复制，只有私有的选项和一些需要特别注明（注意）的记录在这里，其他的请参考[10 编码]章节

为了了解更多关于libvpx的选项，可以在命令行中使用`ffmpeg -h encoder=libvpx`或者`ffmpeg -h encoder=libvpx-vp9`或`vpxenc --help`来获取。进一步信息可以在`libvpx API`文档中获取。

- b (target-bitrate)

    设置码率，单位bits/**注意**FFmpeg中b选项的单位是bits/s，而在vpxenc中目标码率单位是kilobits/s。
- g (kf-max-dist)
- keyint_min (kf-min-dist)
- qmin (min-q)
- qmax (max-q)
- bufsize (buf-sz, buf-optimal-sz)

    设置码率控制缓冲区大小(单位bits)。**注意**在vpxenc中是指定为多少milliseconds（毫秒），这个封装库通过下面的公式进行转换： buf-sz = bufsize * 1000 / bitrate, buf-optimal-sz = bufsize * 1000 / bitrate * 5 / 6.
- rc_init_occupancy (buf-initial-sz)

    设置解码开始前需要加载到RC的预加载数, **注意**vpxenc中指定多少 milliseconds（毫秒），这个封装库按下面公式转换: rc_init_occupancy * 1000 / bitrate.
- undershoot-pct

    设置数据下冲（min）的目标比特率
- overshoot-pct

    设置数据上冲(max)目标比特率
- skip_threshold (drop-frame)
- qcomp (bias-pct)
- maxrate (maxsection-pct)

    设置GOP最大比特率，单位 bits/s ，**注意**vpxenc描述这个为目标码率，这个封装中按如下公式计算： (maxrate * 100 / bitrate).
- minrate (minsection-pct)

    设置GOP最小比特率，单位 bits/s ，**注意**vpxenc描述这个为目标码率，这个封装中按如下公式计算： (minrate * 100 / bitrate).
- minrate, maxrate, b end-usage=cbr

    (minrate == maxrate == bitrate).
- crf (end-usage=cq, cq-level)
- quality, deadline (deadline)

    ‘best’

        使用最优质量期限，不是非常慢，这个选项指定可以有不低于`good`的输出质量（稍微慢一些）。
    ‘good’

        使用高质量期限，它在速度、质量，以及CPU使用间进行均衡。 
    ‘realtime’

        使用实时质量期限 

- speed, cpu-used (cpu-used)

    设置质量/速度 比，高的参数值将加大编码质量成本
- nr (noise-sensitivity)
- static-thresh

    设置一个变化阀值，低于它将被编码器跳过
- slices (token-parts)

    **注意**，FFmpeg指定的是切片分区总数，而vpxenc中是标记部分的log2值
- max-intra-rate

    设置最大I帧比特率作为目标比特率的百分比，0表示不限
- force_key_frames

    VPX_EFLAG_FORCE_KF
- Alternate reference frame related

    auto-alt-ref

        启用备用参考帧，只在2次编码的pass2起效 
    arnr-max-frames

        设置altref降噪的最大帧数
    arnr-type

        设置altref降噪参考过滤类型: backward, forward, centered. 
    arnr-strength

        设置altref降噪滤波强度 
    rc-lookahead, lag-in-frames (lag-in-frames)

        设置向前参考帧码率控制 

- error-resilient

    允许错误弹性
- VP9-specific options

    lossless

        允许lossless（无损）模式 
    tile-columns

        设置采用的tile columns数，**注意**这里参数是log2(tile_columns)值，例如 8 tile columns要设置 tile-columns 选项值为3. 
    tile-rows

        设置采用的tile rows数， **注意**这里参数是log2(tile_rows). 例如 4 tile rows要设置 tile-rows 选项为2. 
    frame-parallel

        允许并行可译特性
    aq-mode

        设置自适应量化模式： (0:关闭 (默认), 1: 方差 2: 复合, 3: 循环刷新). 


### libwebp ###
WebP图片编码封装

liebwebp是google提高的对于WebP图像格式的编码器，它提供任意有损/无损编码模式。有损图像本质上是对VP8框架的封装。无损图像由google单独编码器支持。

#### libwebp 像素格式 ####
当前libwebp只支持YUV420的有损图像和RGB无损。两种模式都支持透明通道。因为API限制了进行RGB有损和YUV420无损编码时像素格式会自动转换使用libwebp库中要求的格式（暨无损用RGB，有损用YUV420）。所以这样做无意义，只是提供了接口。

#### libwebp选项 ####


- -lossless boolean

    允许/禁止无损编码，默认为0（禁止）
- -compression_level integer

    对于有损，设置质量/速度比，高的值表示获取高质量（同样尺寸）需要更多编码成本（时间）。对于无损，是尺寸/速度比，高的值意味要获取小的尺寸需要更多的成本。更具体的说，就是它控制了额外算法和压缩工具的使用，这些工具的组合使用将影响编码质量/效率。它映射到libwebp选项，有效范围是0-6，默认为4
- -qscale float

    对于有损编码，控制品质，范围0-100。对于无损编码，控制资源和时间花费在压缩更多。默认值为75.**注意**使用livavcodec时它对应于`global_quality`*`FF_QP2LAMBDA`.
- -preset type

    选取预置选项。提供一些常规可用设置：

    none

        不采用预置 
    default

        默认预置 
    picture

        数码图片，例如人像拍摄、室内拍摄、
    photo

        室外图像，自然光 lighting 
    drawing

        手绘或者画线，具有高对比度的细节
    icon

        小尺寸彩色图像 
    text

        文本之类的 

### libx264,libx264rgb ###
x264 H.264/MPEG-4 AVC 编码器封装

编译需要头和库文件，还需要利用`--enable-libx264`在配置中允许

libx264提供一些令人印象深刻的特性，包括8x8和4x4自适应空间变化，自适应B帧，CAVLC/CABAC 熵编码，交织（MBAFF），无损模式，物理优化细节保留（自适应量化、psy-RD，psy-trellis）等等

大多数libx264编码器选项均是映射值ffmpeg全局编码选项，仅有少量的是私有的，他们通过libx264中函数`x264_param_parse`, `x264opts`和`x264-params`提供的单个选项或`key=value`序列的多个选项

参考[ http://www.videolan.org/developers/x264.html]( http://www.videolan.org/developers/x264.html)以了解更多x264项目内容。

libx264rgb和libx264类似，只是一个是编码RGB像素格式，一个是针对YUV像素格式的。

#### 支持的像素格式 ####
x264支持8 到 10 bit的颜色空间。确切的颜色深度在x264配置时设置，在一个特定编译版本的FFmpeg中只支持一种颜色深度，换句话就是不同位深需要多个版本的ffmpeg x264.

#### libx264 libx264rgb 选项 ####
下面的选项被libx264（libx264rgb）封装支持，所有的等效 `x264-XXX`形式的选项和值都列在括号中。

这里只列出了需要特别说明或者私有的选项，其他选项参考[10 编码选项]部分。

为了更多的了解关于libx264的选项，可以使用`x264 --full-help`（需要x264命令行工具）或者参考`libx264`文档。


- b (bitrate)

    设置码率，单位bits/s，**注意**FFmpeg的码率单位是bits/s,而x264中码率单位是kilobits/s.
- bf (bframes)
- g (keyint)
- qmin (qpmin)

    最小量化尺度
- qmax (qpmax)

    最大量化尺度
- qdiff (qpstep)

    量化尺度最大差值
- qblur (qblur)

    模糊量化曲线
- qcomp (qcomp)

    量化曲线压缩因子
- refs (ref)

    每一帧可以使用参考帧数，范围0-16.
- sc_threshold (scenecut)

    设置场景变化检测阈值
- trellis (trellis)

    执行网格量化以提高效率。默认情况下启用。
- nr (nr)
- me_range (merange)

    像素运动最大搜索范围
- me_method (me)

    设置运动估计方法。按速度递减顺序可能值：

    ‘dia (dia)’
    ‘epzs (dia)’

        半径为1菱形搜索 (fastest). ‘epzs’是‘dia’的别名 
    ‘hex (hex)’

        半径为2的正六边形搜索。
    ‘umh (umh)’

        多层次六边形搜索。 
    ‘esa (esa)’

        穷举搜索。 
    ‘tesa (tesa)’

        Hadamard(阿达玛)穷举搜索（最慢）。 

- subq (subme)

    亚像素运动估计方法。
- b_strategy (b-adapt)

    自适应B帧布局决策算法。仅第一次使用。
- keyint_min (min-keyint)

    最小 GOP 尺寸
- coder

    设置熵编码器，可能值:

    ‘ac’

        允许CABAC.
    ‘vlc’

        允许CAVLC而且禁止 CABAC. 它类似于x264中的`--no-cabac` 

- cmp

    设置全像素运动估计比较算法，可能值:

    ‘chroma’

        允许chroma
    ‘sad’

        忽略chroma，其等效于 x264中的`--no-chroma-me` 

- threads (threads)

    编码线程数
- thread_type

    设置多线程技术，可能值:

    ‘slice’

        切片多线程，它等效于x264中的`--sliced-threads` 
    ‘frame’

        基于帧的多线程 

- flags

    设置编码标志，它和`-cgop`配合可以用来关闭GOP或者打开GOP，类似于x264中的`--open-gop`
- rc_init_occupancy (vbv-init)
- preset (preset)

    设置编码预置
- tune (tune)

    设置编码参数整定
- profile (profile)

    设置配置文件的限制。
- fastfirstpass

    参数为1则当第一次编码（pass1）允许快速设置，参数为0，表示禁止快速设置（等效于x264的`--slow-firstpass`）
- crf (crf)

    设为质量恒定模式（类VBR）
- crf_max (crf-max)

    CRF模式下，防止VBV降低质量超越的阀值
- qp (qp)

    设定量化率控制方法参数。
- aq-mode (aq-mode)

    设置AQ方法，可能值

    ‘none (0)’

        禁止.
    ‘variance (1)’

        方差AQ (复杂蒙版).
    ‘autovariance (2)’

        自动方差AQ (实验). 

- aq-strength (aq-strength)

    设置AQ强度，减少阻塞平面和纹理区域模糊。
- psy

    为1表示使用视觉优化。为0则禁用（等效 x264的`--no-psy`）
- psy-rd (psy-rd)

    在psy-rd：psy-trellis中设置视觉优化强度
- rc-lookahead (rc-lookahead)

    设置向前预测参考帧数.
- weightb

    为1设置帧加权预测，否则为0表示禁止（等效于x264的`--no-weightb`）
- weightp (weightp)

    设置P帧加权预测法，可能值:

    ‘none (0)’

        禁止 
    ‘simple (1)’

        使用加权参考 
    ‘smart (2)’

        使加权文献和重复 

- ssim (ssim)

    允许在编码结束后技术输出SSIM
- intra-refresh (intra-refresh)

    为1表示使用周期内刷新代替IDR帧设置
- avcintra-class (class)

    配置编码器生成AVC-Intra，有效值50，100，200
- bluray-compat (bluray-compat)

    配置兼容蓝光标准，是 "bluray-compat=1 force-cfr=1"的简写
- b-bias (b-bias)

    设置B帧如何被影响
- b-pyramid (b-pyramid)

    设置保持一些B帧作为参考集的方法，允许值:

    ‘none (none)’

        禁用. 
    ‘strict (strict)’

        严格的分层金字塔 
    ‘normal (normal)’

        Non-strict (非蓝光兼容). 

- mixed-refs

    为1表示每个分区使用一个参考，而不是每个宏块一个参考，否则为0，其等效于x264的 `--no-mixed-refs`
- 8x8dct

    为1指采用自适应空间变换矩阵大小 (8x8变换) ，否则为0，等效于x264的`--no-8x8dct`
- fast-pskip

    为1表示早期跳过检查。等效于x264的`--no-fast-pskip`
- aud (aud)

    为1启用访问单元分隔设置
- mbtree

    为1表示允许使用宏块树，否则（为0）等效于x264的`--no-mbtree` 
- deblock (deblock)

    设置环路滤波参数，参数型为alpha:beta
- cplxblur (cplxblur)

    QP波动减少（压缩前曲线压缩）
- partitions (partitions)

    设置分区规格，参考后面逗号分隔的列表，可能值有:

    ‘p8x8’

        8x8 P帧 分区 
    ‘p4x4’

        4x4 P帧 分区 . 
    ‘b8x8’

        4x4 B帧分区 
    ‘i8x8’

        8x8 I帧分区. 
    ‘i4x4’

        4x4 I帧分区 (‘p4x4’的前提是‘p8x8’也被设置，允许‘i8x8’ 则需要设置了8x8dct被允许) 
    ‘none (none)’

        不考虑分区 
    ‘all (all)’

        考虑所有可能分区 

- direct-pred (direct)

    设置直接MV预测模式，可能值:

    ‘none (none)’

        禁止MV预测 
    ‘spatial (spatial)’

        使空间预测
    ‘temporal (temporal)’

        使时间的预测
    ‘auto (auto)’

        自动识别 

- slice-max-size (slice-max-size)

    设置每个分片的字节大小限制，单位字节，如果不设置但RTP载荷设置了就使用RTP载荷
- stats (stats)

    设置多次编码的文件名称
- nal-hrd (nal-hrd)

    设置HRD信息信号 (要求vbv-bufsize被设置). 可能值:

    ‘none (none)’

        禁用HRD信息信号
    ‘vbr (vbr)’

        可变比特率
    ‘cbr (cbr)’

        固定比特率 (MP4容器不允许). 

- x264opts (N.A.)

    设置任意的x264选项，参看x264 --fullhelp 以获取列表

    参数是一个由':'分隔的`key=value`序列。对于`filter`和`psy-rd`选项，也是有":"被','代替作为分隔符。

    例如，要指定使用libx264编码：

    ffmpeg -i foo.mpg -vcodec libx264 -x264opts keyint=123:min-keyint=20 -an out.mkv

- x264-params (N.A.)

    使用 : 分隔的 key=value 参数覆盖x264配置，

    这个选项类似x264opts，但其兼容Libav

    例如:

    ffmpeg -i INPUT -c:v libx264 -x264-params level=30:bframes=0:weightp=0:\
    cabac=0:ref=1:vbv-maxrate=768:vbv-bufsize=2000:analyse=all:me=umh:\
    no-fast-pskip=1:subq=6:8x8dct=0:trellis=0 OUTPUT

此外编码`ffpresets`还支持一些通用的选项，可以参考前述[ 预置 ]相关文档。

### libx265 ###
x265 H.265/HEVC 编码器封装

编译需要头和库文件，还需要利用`--enable-libx265`在配置中允许

#### libx265选项 ####

- preset

    设置x265预置
- tune

    设置x265可调参数
- x265-params

    使用':'分隔的`key=value`列表进行选项设置，参考 x265 --help 获取支持的选项

    例如采用libx265,并利用-x265-params进行选项设置:

    ffmpeg -i input -c:v libx265 -x265-params crf=26:psy-rd=1 output.mp4

### libxvid ###
Xvid MPEG-4 Part 2 封装

编译需要livxvidcore头和library库文件，还需要利用`--enable-libxvid --enable-gpl`在配置中允许

当前原生的`mpeg4`编码器支持MPEG-4 Part 2格式，所以不一定需要这个库了。

#### libxvid选项 ####
下面选项是libxvid封装支持的选项，其中部分只列出，而没有文档介绍是因为其同[10 编码选项]中通用选项一致，其它没有列出的通用选项则在库中无效。

- b
- g
- qmin
- qmax
- mpeg_quant
- threads
- bf
- b_qfactor
- b_qoffset
- flags

    设置编码标志，可能值:

    ‘mv4’

        对宏块使用4个运动检测
    ‘aic’

        允许高品质AC预测
    ‘gray’

        只编码灰度
    ‘gmc’

        全局运动补偿(GMC).
    ‘qpel’

        1/4像素运动补偿
    ‘cgop’

        关闭GOP.
    ‘global_header’

        在每个关键帧放置全局头extradata

- trellis
- me_method

    设置运动估计方法.按速度降低，质量增加排列的可能值:

    ‘zero’

        不使用运动估计方法 (默认).
    ‘phods’
    ‘x1’
    ‘log’

        启用16x16块和16x16块半像素细化进行菱形区域搜索， ‘x1’和‘log’是‘phods’别名
    ‘epzs’

        允许前述所有值，再加上8x8菱形区域搜索，8x8半像素细化，并在色度平面进行运动估计
    ‘full’

        允许所有的 16x16和8x8 区域搜索 

- mbd

    设置宏块选择算法，依质量提高的可能值:

    ‘simple’

        使用宏块比较函数算法 (默认).
    ‘bits’

        允许16x16块半像素和1/4像素细化失真估计
    ‘rd’

        允许上述所有可能值，再加上8x8块半像素和1/4像素细化失真估计，并采用方形图案失真估计进行搜索。

- lumi_aq

    为1允许lumi遮蔽自适应量化，默认为0 (禁止).
- variance_aq

    为1允许方差的自适应量化,默认为0 (禁止).

    如果结合lumi_aq,由此产生的质量不会比任何一个单独规定。换句话说，所得到的质量会差于单独使用任何一个选项的效果。
- ssim

    设置结构信息（SSIM）显示方法。可能的值：

    ‘off’

        禁止SSIM信息
    ‘avg’

        在编码后输出平均SSIM。格式为：

        Average SSIM: %f

        对那些不熟悉C的的用户，f表示浮点数或者小数 (例如 0.939232)
    ‘frame’

        在编码过程中输出每帧SSIM，并且在编码结束后输出平均SSIM，每帧信息格式为：

               SSIM: avg: %1.3f min: %1.3f max: %1.3f

        对那些不熟悉C的的用户，%1.3f表示3位小数的浮点数(例如0.932).

- ssim_acc

    设置SSIM精度。可用的选项参数是在0-4范围的整数，而0给出了最准确的结果和计算速度最快的4。

### mpeg2 ###
MPEG-2编码器

#### mpeg2选项 ####

- seq_disp_ext integer

    指定是否写一个 sequence_display_extension到输出

    -1
    auto

        自动检测是否写，是默认值，如果数据被写入不同于默认或指定的值则判断是否写 
    0
    never

        从不写 
    1
    always

        一直写 

### png ###
png图像编码器

#### png选项 ####

dpi integer

    设置像素的物理密度，每英寸点数，没有默认设置
dpm integer

   设置像素的物理密度，每米点数，没有默认设置

### ProRes ###
Apple ProRes编码器

FFmpeg包含2种ProRes编码器，prores-aw和prores-ks。它们可以由`-vcodec`选项指定

#### prores-ks私有选项 ####

- profile integer

    选择ProRes属性（预置）配置来编码，可能值：

    ‘proxy’
    ‘lt’
    ‘standard’
    ‘hq’
    ‘4444’

- quant_mat integer

    选择的量化矩阵,可能值：

    ‘auto’
    ‘default’
    ‘proxy’
    ‘lt’
    ‘standard’
    ‘hq’

    如果选择auto, 匹配属性的量化矩阵会被选中，如果没有设置，则选择最高质量的量化矩阵
- bits_per_mb integer

    分配的宏块位，不同的属性在200-2400间，最大值为8000
- mbs_per_slice integer

    每个切片中宏块数（1-8），默认为8，几乎是所有情况下最好值
- vendor string

    重写4字节的供应商ID。例如apl0这个自定义供应商ID会被认为是由苹果编码器产生。
- alpha_bits integer

    指定alpha分量的比特数。可能的值是0，8和16。用0禁用alpha平面编码

#### 速度考虑 ####
在默认操作模式下，编码器以高质量为目的（即在不产生超过要求的帧数据限定下，使输出质量尽可能好）。这种情况下帧内很多小的细节是很难压缩的，编码器将花更多的时间为每个片寻找合适的量化。

所以设置更高的bits_per_mb限额将提高速度。

要获取最快的编码速度，则设置qscale参数（4为推荐值）和不设置帧数据大小限制。

## 字幕编码器
### dvdsub ###
这个编码器编码使用者DVD中的位图字幕格式。一般存储字VOBSUB文件中（包括*.idx *.sub），它也用于Matroska文件中。

#### dvdsub选项 ####

- even_rows_fix

    但设置为1，则让所有的行平顺。它解决了如果最后行是奇数行时可能非法截断的问题。这个选项仅仅在需要的地方添加了一个透明的行，它的开销很低，通常是一个平均字幕字节数。（位图特性上的一种修正）

    默认，work-around被禁止 

