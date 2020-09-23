//
// Created by Administrator on 2020/9/23.
//

#ifndef FFMPEGJNIDEMO_DECODE_STATE_H
#define FFMPEGJNIDEMO_DECODE_STATE_H

#endif //FFMPEGJNIDEMO_DECODE_STATE_H
/**
 * 解码状态
 */
enum DecodeState {
    STOP,//停止
    PREPARE,//准备
    START,//开始
    DECODING,//解码运行中
    PAUSE,//解码暂停
    FINISH,//解码完成
};