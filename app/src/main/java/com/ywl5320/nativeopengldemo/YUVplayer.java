package com.ywl5320.nativeopengldemo;

import android.os.Bundle;
import android.view.View;

import com.ywl5320.opengl.NativeOpengl;
import com.ywl5320.opengl.WlSurfaceView;

import java.io.File;
import java.io.FileInputStream;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

public class YUVplayer extends AppCompatActivity {

    private WlSurfaceView wlSurfaceView;
    private NativeOpengl nativeOpengl;

    private FileInputStream fis;

    private boolean isexit = true;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_yuvplayer);
        wlSurfaceView = findViewById(R.id.wlsurfaceview);
        nativeOpengl = new NativeOpengl();
        wlSurfaceView.setNativeOpengl(nativeOpengl);
    }

    public void play(View view) {
        if(isexit)
        {
            isexit = false;
            new Thread(new Runnable() {
                @Override
                public void run() {
                    int w = 854;
                    int h = 480;
                    try {
                        fis = new FileInputStream(new File("/sdcard/Movies/blr.yuv"));
                        byte[]y = new byte[w * h];
                        byte[]u = new byte[w * h / 4];
                        byte[]v = new byte[w * h / 4];

                        while (true)
                        {
                            if(isexit)
                            {
                                break;
                            }
                            int ysize = fis.read(y);
                            int usize = fis.read(u);
                            int vsize = fis.read(v);
                            if(ysize > 0 && usize > 0 && vsize > 0)
                            {
                                nativeOpengl.setYuvData(y, u, v, w, h);
                                Thread.sleep(16);
                            }
                            else
                            {
                                isexit = true;
                            }
                        }
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
            }).start();
        }
    }

    public void stop(View view) {
        isexit = true;
    }
}
