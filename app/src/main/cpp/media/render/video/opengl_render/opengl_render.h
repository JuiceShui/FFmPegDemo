//
// Created by Administrator on 2020/9/28.
//

#ifndef FFMPEGJNIDEMO_OPENGL_RENDER_H
#define FFMPEGJNIDEMO_OPENGL_RENDER_H


#include <jni.h>
#include <android/native_window.h>
#include <zconf.h>
#include <memory>
#include "../../../../opengl/egl/egl_surface.h"
#include "../../../../opengl/drawer/proxy/drawer_proxy.h"

class OpenGLRender {
private:
    //------------------成员变量------------------------------
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

    STATE m_state = NO_SURFACE;
    //-----------------------------------------------------

    //--------------私有方法-------------------------------
    //初始化相关方法
    void InitRenderThread();

    bool InitEGL();

    void InitDspWindow(JNIEnv *env);

    //创建/销毁 Surface
    void CreateSurface();

    void DestroySurface();

    //渲染
    void Render();

    //释放资源相关
    void ReleaseRender();

    void ReleaseDrawers();

    void ReleaseSurface();

    void ReleaseWindow();

    //渲染线程回调方法
    static void sRenderThread(std::shared_ptr<OpenGLRender> that);

public:
    OpenGLRender(JNIEnv *env, DrawerProxy *drawer_proxy);

    ~OpenGLRender();

    void setSurface(jobject surface);

    void setOffScreenSize(int width, int height);

    void Stop();

};


#endif //FFMPEGJNIDEMO_OPENGL_RENDER_H
