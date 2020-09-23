//
// Created by Administrator on 2020/9/23.
//

#include "base_decoder.h"

BaseDecoder::BaseDecoder(JNIEnv *env, jstring path) {
    Init(env, path);
    CreateDecodeThread();
}

void BaseDecoder::Init(JNIEnv *env, jstring path) {
    m_path_ref = path;
    m_path = env->GetStringUTFChars(path, NULL);
    //获取JVM，为创建线程准备
    env->GetJavaVM(&m_jvm_for_thread);
}

BaseDecoder::~BaseDecoder() {
    if (m_format_ctx != NULL) delete m_format_ctx;
    if (m_codec_ctx != NULL) delete m_codec_ctx;
    if (m_frame != NULL) delete m_frame;
    if (m_packet != NULL) delete m_packet;
    if (m_codec != NULL) delete m_codec;
}

void BaseDecoder::CreateDecodeThread() {
    //使用智能指针，线程结束后，删除本类指针
    std::shared_ptr<BaseDecoder> that_t(this);
    std::thread t_thread(Decode, that_t);
    t_thread.detach();
}

//封装解码流程
void BaseDecoder::Decode(std::shared_ptr<BaseDecoder> that) {
    JNIEnv *env;
    if (that->m_jvm_for_thread->AttachCurrentThread(&env, NULL) != JNI_OK) {
        LOGE(that->TAG, "Failed to init decode thread!!")
        return;
    }
    //初始化解码器
    that->InitFFmPegDecoder(env);
    //分配内存
    that->AllocFrameBuffer();
    //回调子类方法，通知子类解码器初始化完毕
    that->Prepare(env);
    //进入解码循环
    that->LoopDecode();
    //解码完成
    that->DoneDecode(env);
    //解除线程和jvm的关联
    that->m_jvm_for_thread->DetachCurrentThread();
}

void BaseDecoder::InitFFmPegDecoder(JNIEnv *env) {
    //初始化formatContext
    m_format_ctx = avformat_alloc_context();
    //打开文件
    int result = avformat_open_input(&m_format_ctx, m_path, NULL, NULL);
    if (result != 0) {
        //打开失败，直接返回
        LOGE(TAG, "Failed to open file [%s]", m_path);
        DoneDecode(env);
        return;
    }
    //获取音视频流信息
    result = avformat_find_stream_info(m_format_ctx, NULL);
    if (result < 0) {
        //获取音视频信息错误,直接返回
        LOGE(TAG, "Failed to find stream info");
        DoneDecode(env);
        return;
    }
    //查找编解码器
    int av_index = -1;
    for (int i = 0; i < m_format_ctx->nb_streams; ++i) {
        if (m_format_ctx->streams[i]->codecpar->codec_type == GetMediaType()) {
            av_index = i;
            break;
        }
    }
    if (av_index == -1) {
        LOGE(TAG, "Failed to find stream index");
        DoneDecode(env);
        return;
    }
    m_stream_index = av_index;
    //获取解码器参数
    AVCodecParameters *codecParameters = m_format_ctx->streams[av_index]->codecpar;
    //获取编码器
    m_codec = avcodec_find_decoder(codecParameters->codec_id);
    //获取解码器上下文
    m_codec_ctx = avcodec_alloc_context3(m_codec);
    result = avcodec_parameters_to_context(m_codec_ctx, codecParameters);
    if (result < 0) {
        LOGE(TAG, "Failed to get av codec context");
        DoneDecode(env);
        return;
    }
    //打开编码器
    result = avcodec_open2(m_codec_ctx, m_codec, NULL);
    if (result < 0) {
        LOGE(TAG, "Failed to open av codec");
        DoneDecode(env);
        return;
    }
    m_duration = m_format_ctx->duration / AV_TIME_BASE * 1000;
    LOGE(TAG, "FFmPeg init success");
}