package com.ywl5320.opengl;

import android.view.Surface;

public class NativeOpengl {


    static {
        System.loadLibrary("native-lib");
    }

    public native void surfaceCreate(Surface surface);

    public native void surfaceChange(int width, int height);

}
