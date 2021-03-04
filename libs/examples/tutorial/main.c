#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <libavutil/channel_layout.h>//用户音频声道布局操作
# include<libavutil/opt.h>//设置操作选项操作
#include<libavutil/mathematics.h>//用于数学相关操作
#include <libavutil/timestamp.h>//用于时间戳操作
#include<libavformat/avformat.h>//用于封装与解封装操作
# include<libswscale/swscale.h>//用于缩放、转换颜色格式操作
# include<libswresample/swresample.h>//用于进行音频采样率操作

int main(int argc, char **argv) {
    AVOutputFormat *fmt;
    AVFormatContext *oc;
    return 0;
};