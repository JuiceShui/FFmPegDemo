//
// Created by Administrator on 2020/9/23.
//

#ifndef FFMPEGJNIDEMO_BASE_DECODER_H
#define FFMPEGJNIDEMO_BASE_DECODER_H
#define MAX 5

#include "../../utils/logger.h"
#include "i_decoder.h"
#include "decode_state.h"
#include <jni.h>
#include <memory>
#include <thread>

extern "C" {
#include <libavutil/frame.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
};

class BaseDecoder : public IDecoder {
private:
    const char *TAG = "BaseDecoder";
    //-------------------解码器相关--------------------------------
    //解码信息context
    AVFormatContext *m_format_ctx = NULL;
    //解码器context
    AVCodecContext *m_codec_ctx = NULL;
    //解码器
    AVCodec *m_codec = NULL;
    //待解码的包
    AVPacket *m_packet = NULL;
    //解码后的包
    AVFrame *m_frame = NULL;
    //当前播放时间
    long m_cur_t_s = 0;
    //总时长
    long m_duration = 0;
    //开始播放时间
    int64_t m_started_t = -1;
    //解码状态
    DecodeState m_state = STOP;
    //数据流索引
    int m_stream_index = -1;
    //---------------------------------------------------------------


    //------------------线程相关--------------------------------------
    //线程依附的JVM环境
    JavaVM *m_jvm_for_thread = NULL;

    //原始路径jstring引用，否则无法在线程中操作
    jobject m_path_ref = NULL;

    //转化后的路径
    const char *m_path = NULL;

    //线程等待的锁变量
    pthread_mutex_t m_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t m_cond = PTHREAD_COND_INITIALIZER;

    /**
     * 新建解码线程
     */
    void CreateDecodeThread();

    /**
     * 静态解码方法，用于解码线程的回调
     * @param that  当前解码器   shared_ptr 智能指针
     */
    static void Decode(std::shared_ptr<BaseDecoder> that);

    //-------------------------------------------------------------


    //---------------------私有方法-----------------------------

    void Init(JNIEnv *env, jstring path);
    //--------------------------------------------------------------------

    //--------------------FFmPeg相关---------------------------------

    /**
     * 初始化FFmPeg相关参数
     * @param env  jvm 环境
     */
    void InitFFmPegDecoder(JNIEnv *env);

    /**
     *  分配解码过程中需要的缓存
    */
    void AllocFrameBuffer();

    /**
     * 循环解码
     */

    void LoopDecode();

    /**
     * 获取当前时间戳
     */

    void ObtainTimeStamp();

    /**
     * 解码完成
     * @param env jvm环境
     */
    void DoneDecode(JNIEnv *env);

    /**
     * 时间同步
     */
    void SyncRender();

protected:
    /**
     * 进入等待
     * @param second
     */
    void Wait(long second = 0);

    /**
     * 恢复解码
     */
    void SendSignal();

    //-------------------子类需实现的虚函数----------------------------------
    /**
     *子类准备 的回调方法
     * @note  在解码线程中回调
     * @param env 解码线程绑定的jvm
     */
    virtual void Prepare(JNIEnv *env) = 0;

    /**
     * 子类渲染 的回调方法
     * @note 在解码线程中回调
     * @param frame 视频：一帧yuv数据 音频：一帧PCM数据
     */
    virtual void Render(AVFrame *frame) = 0;

    /**
     * 子类释放资源
     */
    virtual void Release() = 0;

    /**
     * 音视频索引
     * @return
     */
    virtual AVMediaType GetMediaType() = 0;
    //----------------------------------------------------------------------

public:
    //-------------构造方法和析构函数------------------------------------------
    /**
     * 构造方法
     * @param env jvm 环境
     * @param path 播放文件的地址
     */
    BaseDecoder(JNIEnv *env, jstring path);

    virtual ~BaseDecoder();
    //----------------------------------------------------------------------

    //-------------------i_decoder的方法实现---------------------------------

    void goOn() override;

    void pause() override;

    void stop() override;

    bool isRunning() override;

    long getDuration() override;

    long getCurPos() override;
    //---------------------------------------------------------------------
};


#endif //FFMPEGJNIDEMO_BASE_DECODER_H
