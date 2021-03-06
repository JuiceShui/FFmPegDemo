//
// Created by Administrator on 2020/9/24.
//

#ifndef FFMPEGJNIDEMO_VIDEO_RENDER_H
#define FFMPEGJNIDEMO_VIDEO_RENDER_H

#include <jni.h>
#include "../../one_frame.h"

class VideoRender {
public:
    virtual void InitRender(JNIEnv *env, int video_width,
                            int video_height, int *dst_size) = 0;

    virtual void Render(OneFrame *one_frame) = 0;

    virtual void ReleaseRender() = 0;
};

#endif //FFMPEGJNIDEMO_VIDEO_RENDER_H
