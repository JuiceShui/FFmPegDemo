//
// Created by Administrator on 2020/9/28.
//

#include "opengl_render.h"
#include <thread>
#include <android/native_window_jni.h>
#include <unistd.h>

extern "C" {
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
};

void OpenGLRender::InitRenderThread() {
    //使用智能指针，线程结束后自动删除
    std::shared_ptr<OpenGLRender> that(this);
    std::thread t(sRenderThread, that);
    t.detach();
}

bool OpenGLRender::InitEGL() {
    m_egl_surface = new EglSurface();
    return m_egl_surface->Init();
}

void OpenGLRender::InitDspWindow(JNIEnv *env) {
    if (m_surface_ref != nullptr) {
        //初始化绘制窗口
        m_native_window = ANativeWindow_fromSurface(env, m_surface_ref);
        //绘制区域高度
        m_window_width = ANativeWindow_getWidth(m_native_window);
        m_window_height = ANativeWindow_getWidth(m_native_window);
        //设置宽高限制缓存区内的像素数量
        ANativeWindow_setBuffersGeometry(m_native_window, m_window_width, m_window_height,
                                         WINDOW_FORMAT_RGBA_8888);
        LOGE(TAG, "viewPort width:%d,height:%d", m_window_width, m_window_height);
    }
}

void OpenGLRender::CreateSurface() {
    m_egl_surface->CreateEglSurface(m_native_window, m_window_width, m_window_height);
    glViewport(0, 0, m_window_width, m_window_height);
}

void OpenGLRender::DestroySurface() {

}

void OpenGLRender::Render() {
    if (m_state == RENDERING) {
        //TODO
    }
}

void OpenGLRender::ReleaseRender() {

}

void OpenGLRender::ReleaseDrawers() {

}

void OpenGLRender::ReleaseSurface() {

}

void OpenGLRender::ReleaseWindow() {

}

void OpenGLRender::sRenderThread(std::shared_ptr<OpenGLRender> that) {
    JNIEnv *env;
    //将线程附加到虚拟机，并获取env
    bool success = that->m_jvm_for_thread->AttachCurrentThread(&env, NULL);
    if (success != JNI_OK) {
        LOGE(that->TAG, "线程初始化异常");
        return;
    }
    if (!that->InitEGL()) {
        //解除线程与jvm的关联
        that->m_jvm_for_thread->DetachCurrentThread();
        return;
    }
    while (true) {
        switch (that->m_state) {
            case FRESH_SURFACE:
                LOGE(that->TAG, "loop render FRESH_SURFACE");
                that->InitDspWindow(env);
                that->CreateSurface();
                that->m_state = RENDERING;
                break;
            case RENDERING:
                that->Render();
                break;
            case SURFACE_DESTROY:
                LOGE(that->TAG, "Loop render SURFACE_DESTROY");
                that->DestroySurface();
                that->m_state = NO_SURFACE;
                break;
            case STOP:
                LOGE(that->TAG, "Loop render stop");
                that->ReleaseRender();
                that->m_jvm_for_thread->DetachCurrentThread();
                return;
            case NO_SURFACE:
            default:
                break;
        }
        usleep(20000);
    }
}

OpenGLRender::OpenGLRender(JNIEnv *env, DrawerProxy *drawer_proxy)
        : m_drawer_proxy(drawer_proxy) {
    this->m_env = env;
    env->GetJavaVM(&m_jvm_for_thread);
    InitRenderThread();

}

OpenGLRender::~OpenGLRender() {
    delete m_egl_surface;
}

void OpenGLRender::setSurface(jobject surface) {
    if (surface != nullptr) {
        m_surface_ref = m_env->NewGlobalRef(surface);
        m_state = FRESH_SURFACE;
    } else {
        m_env->DeleteGlobalRef(m_surface_ref);
        m_state = SURFACE_DESTROY;
    }
}

void OpenGLRender::setOffScreenSize(int width, int height) {

}

void OpenGLRender::Stop() {

}
