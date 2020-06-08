#include <jni.h>
#include <string>

#include "android/native_window.h"
#include "android/native_window_jni.h"
#include "egl/WlEglThread.h"
#include "shaderutil/WlShaderUtil.h"

ANativeWindow *nativeWindow = NULL;
WlEglThread *wlEglThread = NULL;


const char *vertex = "attribute vec4 v_Position;\n"
                     "attribute vec2 f_Position;\n"
                     "varying vec2 ft_Position;\n"
                     "void main() {\n"
                     "    ft_Position = f_Position;\n"
                     "    gl_Position = v_Position;\n"
                     "}";


const char *fragment = "precision mediump float;\n"
                       "varying vec2 ft_Position;\n"
                       "uniform sampler2D sTexture;\n"
                       "void main() {\n"
                       "    gl_FragColor=texture2D(sTexture, ft_Position);\n"
                       "}";

int program;
GLint vPosition;
GLint fPosition;
GLint sampler;
GLuint textureId;


int w;
int h;
void *pixels = NULL;




float vertexs[] = {
        1,-1,
        1,1,
        -1,-1,
        -1,1
};

float fragments[] ={
        1,1,
        1,0,
        0,1,
        0,0
};



void callback_SurfaceCrete(void *ctx)
{
    LOGD("callback_SurfaceCrete");
    WlEglThread *wlEglThread = static_cast<WlEglThread *>(ctx);

    program = createProgrm(vertex, fragment);
    LOGD("opengl program is %d", program);
    vPosition = glGetAttribLocation(program, "v_Position");//��������
    fPosition = glGetAttribLocation(program, "f_Position");//��������
    sampler = glGetUniformLocation(program, "sTexture");//2D����

    glGenTextures(1, &textureId);

    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    if(pixels != NULL)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    }
    glBindTexture(GL_TEXTURE_2D, 0);

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
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);

    glActiveTexture(GL_TEXTURE5);
    glUniform1i(sampler, 5);

    glBindTexture(GL_TEXTURE_2D, textureId);

    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 2, GL_FLOAT, false, 8, vertexs);

    glEnableVertexAttribArray(fPosition);
    glVertexAttribPointer(fPosition, 2, GL_FLOAT, false, 8, fragments);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindTexture(GL_TEXTURE_2D, 0);
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

extern "C"
JNIEXPORT void JNICALL
Java_com_ywl5320_opengl_NativeOpengl_imgData(JNIEnv *env, jobject instance, jint width, jint height,
                                             jint length, jbyteArray data_) {
    jbyte *data = env->GetByteArrayElements(data_, NULL);


    w = width;
    h = height;
    pixels = malloc(length);
    memcpy(pixels, data, length);


    env->ReleaseByteArrayElements(data_, data, 0);
}