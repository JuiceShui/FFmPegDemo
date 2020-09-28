//
// Created by Administrator on 2020/9/28.
//

#ifndef FFMPEGJNIDEMO_EGL_SURFACE_H
#define FFMPEGJNIDEMO_EGL_SURFACE_H


#include <android/native_window.h>
#include "egl_core.h"

class EglSurface {
private:
    const char *TAG = "EGL_SURFACE";
    ANativeWindow *m_native_window = nullptr;
    EglCore *m_core;
    EGLSurface m_egl_surface;
public:
    EglSurface();

    ~EglSurface();

    bool Init();

    void CreateEglSurface(ANativeWindow *nativeWindow, int width, int height);

    void MakeCurrent();

    void SwapBuffers();

    void DestroyEGLSurface();

    void Release();
};


#endif //FFMPEGJNIDEMO_EGL_SURFACE_H
