package com.bse.core;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;
import android.view.Surface;
import android.view.SurfaceView;
import android.view.SurfaceHolder;
import android.util.Log;

public class BseMainActivity extends Activity implements SurfaceHolder.Callback {

  static {
    System.loadLibrary("androidtest");
  }

  private SurfaceView vulkanView;
  private Surface surface;

  
  @Override protected void onCreate(Bundle savedInstanceState) 
  {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.android_vulkan_view);    
    
    vulkanView = findViewById(R.id.vulkan_surface_view);
    vulkanView.setKeepScreenOn(true);
    vulkanView.getHolder().addCallback(this);
    Log.i("[BSE]", "activity launched");
  }

  @Override public void surfaceCreated(SurfaceHolder holder)
  {
    Log.i("[BSE]", "surface created");
    surface = holder.getSurface();
    if (surface != null)
    {
      initializeNative(surface);
    }
  }

  @Override public void surfaceChanged(SurfaceHolder holder, int format, int width, int height)
  {
    Log.i("[BSE]", "surface changed");
    //TODO
  }

  @Override public void surfaceDestroyed(SurfaceHolder holder)
  {
    Log.i("[BSE]", "surface destroyed");
    shutdownNative();
  }

  @Override protected void onPause()
  {
    super.onPause();
  }

  @Override protected void onResume()
  {
    super.onResume();
  }

  private native void initializeNative(Surface surface);
  private native void shutdownNative();
}