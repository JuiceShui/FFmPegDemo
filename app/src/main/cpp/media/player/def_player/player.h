//
// Created by Administrator on 2020/9/27.
//

#ifndef FFMPEGJNIDEMO_PLAYER_H
#define FFMPEGJNIDEMO_PLAYER_H


#include "../../render/video/video_render.h"
#include "../../decoder/video/v_decoder.h"

class Player {
private:
    VideoRender *m_v_render;
    VideoDecoder *m_v_decoder;
public:
    Player(JNIEnv *env, jstring path, jobject surface);

    ~Player();

    void play();

    void pause();
};


#endif //FFMPEGJNIDEMO_PLAYER_H
