#include <jni.h>
#include <string>

#include "android/native_window.h"
#include "android/native_window_jni.h"
#include "egl/WlEglThread.h"
#include "shaderutil/WlShaderUtil.h"

ANativeWindow *nativeWindow = NULL;
WlEglThread *wlEglThread = NULL;


const char *vertex = "attribute vec2 a_position;\n"
                     "\n"
                     "void main(){\n"
                     "    gl_Position = a_position;\n"
                     "}";
const char *fragment = "precision mediump float;\n"
                       "\n"
                       "void main(){\n"
                       "    gl_FragColor = vec4(1f,0f,0f,1f);\n"
                       "}";

void callback_SurfaceCrete(void *ctx)
{
    LOGD("callback_SurfaceCrete");
    WlEglThread *wlEglThread = static_cast<WlEglThread *>(ctx);
}

void callback_SurfacChange(int w, int h, void *ctx)
{
    LOGD("callback_SurfacChange");
    WlEglThread *wlEglThread = static_cast<WlEglThread *>(ctx);
    glViewport(0, 0, w, h);
}

void callback_SurfaceDraw(void *ctx)
{
    LOGD("callback_SurfaceDraw");
    WlEglThread *wlEglThread = static_cast<WlEglThread *>(ctx);
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

}


extern "C"
JNIEXPORT void JNICALL
Java_com_ywl5320_opengl_NativeOpengl_surfaceCreate(JNIEnv *env, jobject instance, jobject surface) {

    // TODO

    nativeWindow = ANativeWindow_fromSurface(env, surface);
    wlEglThread = new WlEglThread();
    wlEglThread->setRenderType(OPENGL_RENDER_HANDLE);
    wlEglThread->callBackOnCreate(callback_SurfaceCrete, wlEglThread);
    wlEglThread->callBackOnChange(callback_SurfacChange, wlEglThread);
    wlEglThread->callBackOnDraw(callback_SurfaceDraw, wlEglThread);

    wlEglThread->onSurfaceCreate(nativeWindow);



    int program = createProgrm(vertex, fragment);
    LOGD("opengl program is %d", program);

}

extern "C"
JNIEXPORT void JNICALL
Java_com_ywl5320_opengl_NativeOpengl_surfaceChange(JNIEnv *env, jobject instance, jint width,
                                                   jint height) {

    // TODO
    if(wlEglThread != NULL)
    {
        wlEglThread->onSurfaceChange(width, height);

        usleep(1000000);
        wlEglThread->notifyRender();
    }

}