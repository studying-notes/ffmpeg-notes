cmake_minimum_required(VERSION 3.14)
project(muxing C)

set(CMAKE_C_STANDARD 99)

include_directories(./ffmpeg/include) # 头文件路径
link_directories(./ffmpeg/lib) # 链接库路径

# 链接库
link_libraries(
        avcodec
        avformat
        avutil
        avdevice
        avfilter
        postproc
        swresample
        swscale
)

add_executable(muxing main.c)