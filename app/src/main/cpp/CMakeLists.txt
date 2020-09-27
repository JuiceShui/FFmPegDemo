#设置cmake最低版本
cmake_minimum_required(VERSION 3.4.1)
#设置支持gnu++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

#定义so库和头文件所在目录
#CMAKE_SOURCE_DIR 为系统变量 指向 CMakeLists.txt所在目录
#ANDROID_ABI || CMAKE_ANDROID_ARCH_ABI 为系统变量，指向so库对应的CPU架构目录
#别人文章里写的是ANDROID_ABI  这里默认提示的是CMAKE_ANDROID_ARCH_ABI ，目前为止，好像作用一样
#如armeabi,armeabi-v7a,arm64-v8a
set(ffmpeg_lib_dir ${CMAKE_SOURCE_DIR}/../jniLibs/${CMAKE_ANDROID_ARCH_ABI})
#设置头文件所在目录
set(ffmpeg_head_dir ${CMAKE_SOURCE_DIR}/ffmpeg)

#添加so库头文件目录  如有多个，则换行继续加入
include_directories(
        ${ffmpeg_head_dir}/include
        #header2
        #header3
)

#add_libary 添加cpp下待编译的cpp文件
add_library(
        native-lib  #编译后的名字
        SHARED  #静态库方式
        native-lib.cpp #文件路径

        #解码相关
        ${CMAKE_SOURCE_DIR}/media/decoder/i_decoder.h
        ${CMAKE_SOURCE_DIR}/media/decoder/decode_state.h
        ${CMAKE_SOURCE_DIR}/media/decoder/base_decoder.cpp
        ${CMAKE_SOURCE_DIR}/media/decoder/video/v_decoder.h
        ${CMAKE_SOURCE_DIR}/media/decoder/video/v_decoder.cpp

        #工具类
        ${CMAKE_SOURCE_DIR}/utils/logger.h
        ${CMAKE_SOURCE_DIR}/utils/timer.c

        #渲染
        ${CMAKE_SOURCE_DIR}/media/render/video/video_render.h
        ${CMAKE_SOURCE_DIR}/media/render/video/native_render/native_render.cpp
        ${CMAKE_SOURCE_DIR}/media/one_frame.h

        #播放器
        ${CMAKE_SOURCE_DIR}/media/player/def_player/player.h
        ${CMAKE_SOURCE_DIR}/media/player/def_player/player.cpp


)

#add_libray添加FFmpeg 相关so库，
#set_target_properties 设置so库的对应目录及引入方式
#SHARED 表示以动态库的形式引入
#IMPORTED 表示是导入已存在的so库，不需要构建so
#IMPORTED_LOCATION 表示当是IMPORTED导入时对应的导入
add_library(
        avcodec
        SHARED
        IMPORTED  #外部导入so库不需要构建 但需要设置相关属性即需要set_target_properties设置
)
set_target_properties(
        avcodec
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir}/libavcodec.so #so库文件所在位置
)
add_library(
        avdevice
        SHARED
        IMPORTED
)
set_target_properties(
        avdevice
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir}/libavdevice.so
)
add_library(avfilter
        SHARED
        IMPORTED
        )
set_target_properties(
        avfilter
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir}/libavfilter.so
)
add_library(
        avformat
        SHARED
        IMPORTED
)
set_target_properties(
        avformat
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir}/libavformat.so
)
add_library(
        avutil
        SHARED
        IMPORTED
)
set_target_properties(
        avutil
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir}/libavutil.so
)
add_library(
        swresample
        SHARED
        IMPORTED
)
set_target_properties(
        swresample
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir}/libswresample.so
)
add_library(
        swscale
        SHARED
        IMPORTED
)
set_target_properties(
        swscale
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir}/libswscale.so
)

#添加代码中使用到的C++日志库
find_library(
        log-lib
        log
)

#指定编译目标库时 cmake要链接的库
target_link_libraries(
        #指定目标库  native-lib是在上面add_library中配置的目标库
        native-lib

        #链接相关看的库  （FFmpeg的库和系统库）
        avcodec
        avdevice
        avfilter
        avformat
        avutil
        swresample
        swscale
        -landroid
        ${log-lib}
)

#最后，通过 target_link_libraries
#把前面添加进来的 FFMpeg so 库都链接到目标库 native-lib 上。
#将 FFMpeg 相关的 so 库都引入到当前工程中