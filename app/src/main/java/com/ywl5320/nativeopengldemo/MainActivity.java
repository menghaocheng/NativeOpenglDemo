package com.ywl5320.nativeopengldemo;

import androidx.appcompat.app.AppCompatActivity;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.widget.TextView;

import com.ywl5320.opengl.NativeOpengl;
import com.ywl5320.opengl.WlSurfaceView;

import java.nio.ByteBuffer;

public class MainActivity extends AppCompatActivity {

    private WlSurfaceView wlSurfaceView;
    private NativeOpengl nativeOpengl;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        wlSurfaceView = findViewById(R.id.wlsurfaceview);
        nativeOpengl = new NativeOpengl();
        wlSurfaceView.setNativeOpengl(nativeOpengl);


        final Bitmap bitmap = BitmapFactory.decodeResource(getResources(),
                R.drawable.mingren);
        ByteBuffer fcbuffer = ByteBuffer.allocate(bitmap.getHeight() * bitmap.getWidth() * 4);
        bitmap.copyPixelsToBuffer(fcbuffer);
        fcbuffer.flip();
        byte[] pixels = fcbuffer.array();
        nativeOpengl.imgData(bitmap.getWidth(), bitmap.getHeight(), pixels.length, pixels);
    }
}
