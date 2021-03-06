//
// Created by Administrator on 2020/9/24.
//

#ifndef FFMPEGJNIDEMO_V_DECODER_H
#define FFMPEGJNIDEMO_V_DECODER_H


#include "../base_decoder.h"
#include "../../render/video/video_render.h"

extern "C" {
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
};

class VideoDecoder : public BaseDecoder {
private:
    const char *TAG = "VideoDecoder";
    //视频数据目标格式
    AVPixelFormat DST_FORMAT = AV_PIX_FMT_RGBA;
    //存放yuv转化为RGBA后的数据
    AVFrame *m_rgb_frame = NULL;
    uint8_t *m_buf_for_rgb_frame = NULL;

    //视频格式转换器
    SwsContext *m_sws_ctx = NULL;

    //视频渲染器
    VideoRender *m_video_render = NULL;

    //显示的目标宽
    int m_dst_w;
    //显示的目标高
    int m_dst_h;

    /**
     * 初始化渲染器
     * @param env
     */
    void InitRender(JNIEnv *env);

    /**
     * 初始化显示器
     */
    void InitBuffer();

    /**
     * 初始化视频数据转换器
     */
    void InitSws();

public:
    VideoDecoder(JNIEnv *env, jstring path, bool for_synthsizer = false);

    ~VideoDecoder();

    void SetRender(VideoRender *render);

protected:
    AVMediaType GetMediaType() override {
        return AVMEDIA_TYPE_VIDEO;
    }

    /**
     * 是否需要循环解码
     * @return
     */
    bool NeedLoopDecode() override;

    /**
     * 准备解码环境
     * @note 在解码线程回调
     * @param env  解码线程绑定的jni环境
     */
    void Prepare(JNIEnv *env) override;

    /**
     * 渲染
     * @note 在解码线程中回调
     * @param frame  解码的RGBA数据
     */
    void Render(AVFrame *frame) override;

    /**
     * 释放的回调
     */
    void Release() override;

    const char *const LogSpec() override {
        return "VIDEO";
    };

};


#endif //FFMPEGJNIDEMO_V_DECODER_H
