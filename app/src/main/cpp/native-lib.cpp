#include <jni.h>
#include <string>
#include <unistd.h>
#include "media/player/def_player/player.h"

extern "C" JNIEXPORT jstring JNICALL
Java_com_demo_ffmpegjnidemo_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/avfilter.h>
#include <libavcodec/jni.h>
JNIEXPORT jstring JNICALL
Java_com_demo_ffmpegjnidemo_MainActivity_ffmpegInfo(JNIEnv *env, jobject thiz) {
    char info[40000] = {0};
    AVCodec *c_temp = av_codec_next(NULL);
    while (c_temp != NULL) {
        if (c_temp->decode != NULL) {
            sprintf(info, "%sdecode:", info);
        } else {
            sprintf(info, "%sencode:", info);
        }
        switch (c_temp->type) {
            case AVMEDIA_TYPE_VIDEO:
                sprintf(info, "%s(video):", info);
                break;
            case AVMEDIA_TYPE_AUDIO:
                sprintf(info, "%s(audio):", info);
                break;
            default:
                sprintf(info, "%s(other):", info);
                break;
        }
        sprintf(info, "%s[%s]\n", info, c_temp->name);
        c_temp = c_temp->next;
    }
    return env->NewStringUTF(info);
}
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_demo_ffmpegjnidemo_MainActivity_createPlayer(JNIEnv *env, jobject thiz, jstring path,
                                                      jobject surface) {
    Player *player = new Player(env, path, surface);
    return (jint) player;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_demo_ffmpegjnidemo_MainActivity_play(JNIEnv *env, jobject thiz, jint player) {
    Player *p = (Player *) player;
    p->play();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_demo_ffmpegjnidemo_MainActivity_pasue(JNIEnv *env, jobject thiz, jint player) {
    Player *p = (Player *) player;
    p->pause();
}