//
// Created by Administrator on 2020/9/28.
//

#include "egl_core.h"

EGLConfig EglCore::CreateEGLConfig() {
    EGLint numConfigs;
    EGLConfig eglConfig;
    EGLint CONFIG_ATTRS[] = {
            EGL_BUFFER_SIZE, EGL_DONT_CARE,
            EGL_RED_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE, 8,
            EGL_ALPHA_SIZE, 8,
            EGL_DEPTH_SIZE, 16,
            EGL_STENCIL_SIZE, EGL_DONT_CARE,
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_NONE//结束标志
    };
    EGLBoolean success = eglChooseConfig(m_egl_dsp, CONFIG_ATTRS, &eglConfig,
                                         1, &numConfigs);
    if (success != EGL_TRUE || eglGetError() != EGL_SUCCESS) {
        LOGE(TAG, "EGL config failed");
        return nullptr;
    }
    return eglConfig;
}

EglCore::EglCore() {

}

EglCore::~EglCore() {

}

bool EglCore::Init(EGLContext share_ctx) {
    if (m_egl_dsp != EGL_NO_DISPLAY) {
        LOGE(TAG, "EGL_Display already set up");
        return true;
    }
    if (share_ctx == nullptr) {
        share_ctx = EGL_NO_CONTEXT;
    }
    m_egl_dsp = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (m_egl_dsp == EGL_NO_DISPLAY || eglGetError() != EGL_SUCCESS) {
        LOGE(TAG, "init egl_display failed");
        return false;
    }
    EGLint major_ver, minor_ver;
    EGLBoolean success = eglInitialize(m_egl_dsp, &major_ver, &minor_ver);
    if (success != EGL_TRUE || eglGetError() != EGL_SUCCESS) {
        LOGE(TAG, "init EGL failed");
    }
    LOGE(TAG, "EGL version %d,%d", major_ver, minor_ver);

    m_egl_cfg = CreateEGLConfig();

    EGLint attr[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
    m_egl_cxt = eglCreateContext(m_egl_dsp, m_egl_cfg, share_ctx, attr);
    if (m_egl_cxt == EGL_NO_CONTEXT) {
        LOGE(TAG, "Egl create context failed ,error is %d", eglGetError());
        return false;
    }
    EGLint egl_format;
    success = eglGetConfigAttrib(m_egl_dsp, m_egl_cfg, EGL_NATIVE_VISUAL_ID,
                                 &egl_format);
    if (success != EGL_TRUE || eglGetError() != EGL_SUCCESS) {
        LOGE(TAG, "EGL getConfig failed!");
        return false;
    }
    LOGE(TAG, "EGL init success");
    return true;
}

EGLSurface EglCore::CreateWindowSurface(ANativeWindow *window) {

    EGLSurface eglSurface = eglCreateWindowSurface(m_egl_dsp, m_egl_cfg,
                                                   window, 0);
    if (eglSurface == nullptr || eglGetError() != EGL_SUCCESS) {
        LOGE(TAG, "EGL create window failed");
        return nullptr;
    }
    return eglSurface;
}

EGLSurface EglCore::CreateOffScreenSurface(int width, int height) {
    EGLint ATTRS[]{
            EGL_WIDTH, width,
            EGL_HEIGHT, height,
            EGL_NONE
    };
    EGLSurface eglSurface = eglCreatePbufferSurface(m_egl_dsp, m_egl_cfg, ATTRS);
    if (eglSurface == nullptr || eglGetError() != EGL_SUCCESS) {
        LOGE(TAG, "EGL create offscreen surface error");
        return nullptr;
    }
    return eglSurface;
}

void EglCore::MakeCurrent(EGLSurface egl_surface) {
    EGLBoolean success =
            eglMakeCurrent(m_egl_dsp, egl_surface, egl_surface, m_egl_cxt);
    if (success != EGL_TRUE) {
        LOGE(TAG, "EGL makeCurrent failed");
    }
}

void EglCore::SwapBuffers(EGLSurface egl_surface) {
    EGLBoolean success = eglSwapBuffers(m_egl_dsp, egl_surface);
    if (success != EGL_TRUE) {
        LOGE(TAG, "EGL swapBuffer failed");
    }
}

void EglCore::DestroySurface(EGLSurface egl_surface) {
    eglMakeCurrent(m_egl_dsp, EGL_NO_SURFACE,
                   EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroySurface(m_egl_dsp, egl_surface);
}

void EglCore::Release() {
    if (m_egl_dsp != EGL_NO_DISPLAY) {
        eglMakeCurrent(m_egl_dsp, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        eglDestroyContext(m_egl_dsp, m_egl_cxt);
        eglReleaseThread();
        eglTerminate(m_egl_dsp);
    }
    m_egl_dsp = EGL_NO_DISPLAY;
    m_egl_cxt = EGL_NO_CONTEXT;
    m_egl_cfg = nullptr;
}
