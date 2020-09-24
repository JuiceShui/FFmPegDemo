//
// Created by Administrator on 2020/9/24.
//

#include <libavutil/imgutils.h>
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

}

VideoDecoder::~VideoDecoder() {

}

void VideoDecoder::SetRender(VideoRender *render) {

}

bool VideoDecoder::NeedLoopDecode() {
    return false;
}

void VideoDecoder::Render(AVFrame *frame) {

}

void VideoDecoder::Release() {

}

