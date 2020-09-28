//
// Created by Administrator on 2020/9/28.
//

#ifndef FFMPEGJNIDEMO_OPENGL_RENDER_H
#define FFMPEGJNIDEMO_OPENGL_RENDER_H


#include <jni.h>
#include <android/native_window.h>
#include "../../../../opengl/egl/egl_surface.h"
#include "../../../../opengl/drawer/proxy/drawer_proxy.h"

class OpenGLRender {
private:
    const char *TAG = "OPEN_GL_RENDER";
    enum STATE {
        NO_SURFACE,
        FRESH_SURFACE,
        RENDERING,
        SURFACE_DESTROY,
        STOP
    };
    JNIEnv *m_env = nullptr;

    //线程依附的jvm环境
    JavaVM *m_jvm_for_thread = nullptr;

    //surface的引用，必须使用引用，否则无法在线程中操作
    jobject m_surface_ref = nullptr;

    //本地屏幕
    ANativeWindow *m_native_window = nullptr;

    //EGL显示界面
    EglSurface *m_egl_surface = nullptr;

    //绘制代理
    DrawerProxy *m_drawer_proxy = nullptr;

    int m_window_width = 0;
    int m_window_height = 0;

    STATE m_state = STOP;
};


#endif //FFMPEGJNIDEMO_OPENGL_RENDER_H
