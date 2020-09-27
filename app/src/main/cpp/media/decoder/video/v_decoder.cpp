//
// Created by Administrator on 2020/9/24.
//

#include "v_decoder.h"

VideoDecoder::VideoDecoder(JNIEnv *env, jstring path,
                           bool for_synthesizer) : BaseDecoder(env, path, for_synthesizer) {

}

void VideoDecoder::Prepare(JNIEnv *env) {
    InitRender(env);
    InitBuffer();
    InitSws();
}

void VideoDecoder::InitRender(JNIEnv *env) {
    if (m_video_render != NULL) {
        int dst_size[2] = {-1, -1};
        m_video_render->InitRender(env, width(),
                                   height(), dst_size);
        m_dst_w = dst_size[0];
        m_dst_h = dst_size[1];
        if (m_dst_w == -1) {
            m_dst_w = width();
        }
        if (m_dst_h == -1) {
            m_dst_h = height();
        }
        LOGE(TAG, "dst w:%d ,h:%d", m_dst_w, m_dst_h);
    } else {
        LOGE(TAG, "init render error ,should call setRender first");
    }
}

void VideoDecoder::InitBuffer() {
    //初始化一个AVFrame  @note 当前未分配缓存内存
    m_rgb_frame = av_frame_alloc();
    //获取缓存大小
    int numByte = av_image_get_buffer_size(DST_FORMAT, m_dst_w, m_dst_h, 1);
    //分配内存
    m_buf_for_rgb_frame = (uint8_t *) av_malloc(numByte * sizeof(uint8_t));
    //将内存分配给RGBFrame ，并将内存格式化为3个通道后，分别保存其地址
    av_image_fill_arrays(m_rgb_frame->data, m_rgb_frame->linesize,
                         m_buf_for_rgb_frame, DST_FORMAT, m_dst_w, m_dst_h, 1);
}

void VideoDecoder::InitSws() {
    //初始化格式转换工具
    m_sws_ctx = sws_getContext(width(), height(), video_pixel_format(),
                               m_dst_w, m_dst_h, DST_FORMAT, SWS_FAST_BILINEAR,
                               NULL, NULL, NULL);

}

VideoDecoder::~VideoDecoder() {

}

void VideoDecoder::SetRender(VideoRender *render) {
    m_video_render = render;
}

bool VideoDecoder::NeedLoopDecode() {
    return false;
}

void VideoDecoder::Render(AVFrame *frame) {
    sws_scale(m_sws_ctx, frame->data, frame->linesize, 0,
              height(), m_rgb_frame->data,
              m_rgb_frame->linesize);
    OneFrame *oneFrame =
            new OneFrame(m_rgb_frame->data[0],
                         m_rgb_frame->linesize[0], frame->pts,
                         time_base(), NULL, false);
    m_video_render->Render(oneFrame);
}

//释放相关资源
void VideoDecoder::Release() {
    LOGE(TAG, "{VideoDecoder} release");
    if (m_rgb_frame != NULL) {
        av_frame_free(&m_rgb_frame);
        m_rgb_frame = NULL;
    }
    if (m_buf_for_rgb_frame != NULL) {
        free(m_buf_for_rgb_frame);
        m_buf_for_rgb_frame = NULL;
    }
    if (m_sws_ctx != NULL) {
        sws_freeContext(m_sws_ctx);
        m_sws_ctx = NULL;
    }
    if (m_video_render != NULL) {
        m_video_render->ReleaseRender();
        m_video_render = NULL;
    }
}

