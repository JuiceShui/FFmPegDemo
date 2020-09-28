//
// Created by Administrator on 2020/9/28.
//

#include "egl_surface.h"

EglSurface::EglSurface() {
    m_core = new EglCore();
}

EglSurface::~EglSurface() {
    delete m_core;
}

bool EglSurface::Init() {
    return m_core->Init(NULL);
}

void EglSurface::CreateEglSurface(ANativeWindow *nativeWindow, int width, int height) {
    if (nativeWindow != nullptr) {
        m_native_window = nativeWindow;
        m_egl_surface = m_core->CreateWindowSurface(m_native_window);
    } else {
        m_egl_surface = m_core->CreateOffScreenSurface(width, height);
    }
    if (m_egl_surface == nullptr) {
        LOGE(TAG, "EGL create eglSurface failed");
        Release();
    }
    MakeCurrent();
}

void EglSurface::MakeCurrent() {
    m_core->MakeCurrent(m_egl_surface);
}

void EglSurface::SwapBuffers() {
    m_core->SwapBuffers(m_egl_surface);
}

void EglSurface::DestroyEGLSurface() {
    if (m_egl_surface != nullptr) {
        if (m_core != nullptr) {
            m_core->DestroySurface(m_egl_surface);
        }
        m_egl_surface = nullptr;
    }
}

void EglSurface::Release() {
    DestroyEGLSurface();
    if (m_core != nullptr) {
        m_core->Release();
    }
}
