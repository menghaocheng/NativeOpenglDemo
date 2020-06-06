#include <jni.h>
#include <string>

#include "egl/WlEglHelper.h"
#include "android/native_window.h"
#include "android/native_window_jni.h"
#include "GLES2/gl2.h"

WlEglHelper *wlEglHelper = NULL;
ANativeWindow *nativeWindow = NULL;


extern "C"
JNIEXPORT void JNICALL
Java_com_ywl5320_opengl_NativeOpengl_surfaceCreate(JNIEnv *env, jobject instance, jobject surface) {

    // TODO

    nativeWindow = ANativeWindow_fromSurface(env, surface);

    wlEglHelper = new WlEglHelper();
    wlEglHelper->initEgl(nativeWindow);

    //opnegl
    glViewport(0, 0, 720, 1280);

    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    wlEglHelper->swapBuffers();

}