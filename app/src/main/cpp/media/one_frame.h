//
// Created by Administrator on 2020/9/25.
//

#ifndef FFMPEGJNIDEMO_ONE_FRAME_H
#define FFMPEGJNIDEMO_ONE_FRAME_H

#include <libavutil/rational.h>

extern "C" {

};

class OneFrame {
public:
    uint8_t *data = NULL;
    int line_size;
    int64_t pts;
    AVRational time_base;
    uint8_t *ext_data = NULL;

    //是否自动回收 data和 ext_data
    bool autoRecycle = true;


};

#endif //FFMPEGJNIDEMO_ONE_FRAME_H
