//
// Created by Administrator on 2020/9/25.
//

#include "native_render.h"

NativeRender::NativeRender(JNIEnv *env, jobject surface) {
    m_surface_ref = env->NewGlobalRef(surface);
}

NativeRender::~NativeRender() {

}

void NativeRender::InitRender(JNIEnv *env, int video_width,
                              int video_height, int *dst_size) {
    //初始化窗口
    m_native_window = ANativeWindow_fromSurface(env, m_surface_ref);
    //绘制区域的宽高
    int windowWidth = ANativeWindow_getWidth(m_native_window);
    int windowHeight = ANativeWindow_getHeight(m_native_window);
    //计算目标视频的宽高
    m_dst_w = windowWidth;
    m_dst_h = m_dst_w * video_height / video_width;
    if (m_dst_h > windowHeight) {
        m_dst_h = windowHeight;
        m_dst_w = windowHeight * video_width / video_height;
    }
    LOGE(TAG, "windowW: %d, windowH: %d, dstVideoW: %d, dstVideoH: %d",
         windowWidth, windowHeight, m_dst_w, m_dst_h);
    ANativeWindow_setBuffersGeometry(m_native_window, windowWidth, windowHeight,
                                     WINDOW_FORMAT_RGBA_8888);
    dst_size[0] = m_dst_w;
    dst_size[1] = m_dst_h;
}

void NativeRender::Render(OneFrame *one_frame) {
    //锁定窗口
    ANativeWindow_lock(m_native_window, &m_out_buffer, NULL);
    uint8_t *dst = (uint8_t *) m_out_buffer.bits;
    //获取stride ，一行
    int desStride = m_out_buffer.stride * 4;
    int srcStride = one_frame->line_size;

}
