//
// Created by Administrator on 2020/9/27.
//

#include "player.h"
#include "../../render/video/native_render/native_render.h"

Player::Player(JNIEnv *env, jstring path, jobject surface) {
    m_v_decoder = new VideoDecoder(env, path);
    //本地窗口播放
    m_v_render = new NativeRender(env, surface);
    m_v_decoder->SetRender(m_v_render);
}

Player::~Player() {

}

void Player::play() {
    if (m_v_decoder != NULL) {
        m_v_decoder->GoOn();
    }
}

void Player::pause() {
    if (m_v_decoder != NULL) {
        m_v_decoder->Pause();
    }
}
