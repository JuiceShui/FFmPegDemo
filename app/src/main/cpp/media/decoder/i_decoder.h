//
// Created by Administrator on 2020/9/23.
//

#ifndef FFMPEGJNIDEMO_I_DECODER_H
#define FFMPEGJNIDEMO_I_DECODER_H

class IDecoder {
public:
    //继续解码
    virtual void goOn() = 0;

    //暂停解码
    virtual void pause() = 0;

    //停止解码
    virtual void stop() = 0;

    //返回是否正在解码中
    virtual bool isRunning() = 0;

    //返回视频长度
    virtual long getDuration() = 0;

    //返回当前时间
    virtual long getCurPos() = 0;
};

#endif //FFMPEGJNIDEMO_I_DECODER_H