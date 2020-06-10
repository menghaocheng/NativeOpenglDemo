//
// Created by yangw on 2019-3-31.
//

#include "WlOpengl.h"


void callback_SurfaceCrete(void *ctx)
{

    WlOpengl *wlOpengl = static_cast<WlOpengl *>(ctx);

    if(wlOpengl != NULL)
    {
        if(wlOpengl->baseOpengl != NULL)
        {
            wlOpengl->baseOpengl->onCreate();
        }
    }
}

void callback_SurfacChange(int width, int height, void *ctx)
{
    WlOpengl *wlOpengl = static_cast<WlOpengl *>(ctx);
    if(wlOpengl != NULL)
    {
        if(wlOpengl->baseOpengl != NULL)
        {
            wlOpengl->baseOpengl->onChange(width, height);
        }
    }
}

void callback_SurfaceDraw(void *ctx)
{
    WlOpengl *wlOpengl = static_cast<WlOpengl *>(ctx);
    if(wlOpengl != NULL)
    {
        if(wlOpengl->baseOpengl != NULL)
        {
            wlOpengl->baseOpengl->draw();
        }
    }
}

void callback_SurfaceChangeFilter(int width, int height, void *ctx)
{
    WlOpengl *wlOpengl = static_cast<WlOpengl *>(ctx);
    if(wlOpengl != NULL)
    {
        if(wlOpengl->baseOpengl != NULL)
        {
            wlOpengl->baseOpengl->destroy();
            delete wlOpengl->baseOpengl;
            wlOpengl->baseOpengl = NULL;
        }
        LOGE("3 ��width %d height %d %d %d", width, height, wlOpengl->pic_width, wlOpengl->pic_height);
        wlOpengl->baseOpengl = new WlFilterTwo();
        wlOpengl->baseOpengl->onCreate();
        wlOpengl->baseOpengl->onChange(width, height);
        wlOpengl->baseOpengl->setPilex(wlOpengl->pilex, wlOpengl->pic_width, wlOpengl->pic_height, 0);
        wlOpengl->wlEglThread->notifyRender();
    }
}


WlOpengl::WlOpengl() {

}

WlOpengl::~WlOpengl() {

}

void WlOpengl::onCreateSurface(JNIEnv *env, jobject surface) {

    nativeWindow = ANativeWindow_fromSurface(env, surface);
    wlEglThread = new WlEglThread();
    wlEglThread->setRenderType(OPENGL_RENDER_HANDLE);
    wlEglThread->callBackOnCreate(callback_SurfaceCrete, this);
    wlEglThread->callBackOnChange(callback_SurfacChange, this);
    wlEglThread->callBackOnDraw(callback_SurfaceDraw, this);
    wlEglThread->callBackOnChangeFilter(callback_SurfaceChangeFilter, this);

    baseOpengl = new WlFilterOne();

    wlEglThread->onSurfaceCreate(nativeWindow);


}

void WlOpengl::onChangeSurface(int width, int height) {

    if(wlEglThread != NULL)
    {
        LOGE("2 ��width %d height %d", width, height);
        if(baseOpengl != NULL)
        {
            baseOpengl->surface_width = width;
            baseOpengl->surface_height = height;
        }
        wlEglThread->onSurfaceChange(width, height);
    }

}


void WlOpengl::onDestorySurface() {

    if(wlEglThread != NULL)
    {
        wlEglThread->destroy();
    }
    if(baseOpengl != NULL)
    {
        baseOpengl->destroy();
        delete baseOpengl;
        baseOpengl = NULL;
    }
    if(nativeWindow != NULL)
    {
        ANativeWindow_release(nativeWindow);
        nativeWindow = NULL;
    }

    if(pilex != NULL)
    {
        free(pilex);
        pilex = NULL;
    }
}

void WlOpengl::setPilex(void *data, int width, int height, int length) {

    pic_width = width;
    pic_height = height;
    pilex = malloc(length);
    memcpy(pilex, data, length);
    if(baseOpengl != NULL)
    {
        baseOpengl->setPilex(pilex, width, height, length);
    }
    if(wlEglThread != NULL)
    {
        wlEglThread->notifyRender();
    }



}

void WlOpengl::onChangeFilter() {
    if(wlEglThread != NULL)
    {
        wlEglThread->onSurfaceChangeFilter();
    }
}