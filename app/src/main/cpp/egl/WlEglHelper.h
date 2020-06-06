//
// Created by Administrator on 2020/6/6.
//

#ifndef NATIVEOPENGLDEMO_WLEGLHELPER_H
#define NATIVEOPENGLDEMO_WLEGLHELPER_H

#include "EGL/egl.h"
#include "../log/WlAndroidLog.h"

class WlEglHelper {

public:
    EGLDisplay  mEglDisplay;
    EGLSurface  mEglSurface;
    EGLConfig  mEglConfig;
    EGLContext mEglContext;

public:
    WlEglHelper();
    ~WlEglHelper();

    int initEgl(EGLNativeWindowType win);

    int swapBuffers();

    void destoryEgl();




};


#endif //NATIVEOPENGLDEMO_WLEGLHELPER_H
