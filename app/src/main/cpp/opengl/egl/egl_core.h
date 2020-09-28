//
// Created by Administrator on 2020/9/28.
//

#ifndef FFMPEGJNIDEMO_EGL_CORE_H
#define FFMPEGJNIDEMO_EGL_CORE_H

#include "../../utils/logger.h"

extern "C" {
#include <EGL/egl.h>
#include <EGL/eglext.h>
};

class EglCore {
private:
    const char *TAG = "EglCore";
    //EGL显示窗口
    EGLDisplay m_egl_dsp = EGL_NO_DISPLAY;
    //EGL环境
    EGLContext m_egl_cxt = EGL_NO_CONTEXT;
    //EGL配置
    EGLConfig m_egl_cfg;

    EGLConfig CreateEGLConfig();

public:
    EglCore();

    ~EglCore();

    bool Init(EGLContext share_ctx);

    EGLSurface CreateWindowSurface(ANativeWindow *window);

    EGLSurface CreateOffScreenSurface(int width, int height);

    //将OpenGL上下文和线程绑定
    void MakeCurrent(EGLSurface egl_surface);

    //将缓存数据交换到前台显示
    void SwapBuffers(EGLSurface egl_surface);

    //释放显示
    void DestroySurface(EGLSurface egl_surface);

    //释放EGL
    void Release();
};


#endif //FFMPEGJNIDEMO_EGL_CORE_H
