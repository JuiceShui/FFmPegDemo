//
// Created by Administrator on 2020/9/28.
//

#ifndef FFMPEGJNIDEMO_DRAWER_H
#define FFMPEGJNIDEMO_DRAWER_H


#include <GLES2/gl2.h>

class Drawer {
private:
    const char *TAG = "Drawer";

    /**上下颠倒的顶点矩阵*/
    const GLfloat m_reserve_vertex_coors[8] = {
            -1.0f, 1.0f,
            1.0f, 1.0f,
            -1.0f, -1.0f,
            1.0f, -1.0f
    };

    const GLfloat m_vertex_coors[8] = {
            -1.0f, -1.0f,
            1.0f, -1.0f,
            -1.0f, 1.0f,
            1.0f, 1.0f
    };

    const GLfloat m_texture_coors[8] = {
            0.0f, 1.0f,
            1.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 0.0f
    };
};


#endif //FFMPEGJNIDEMO_DRAWER_H
