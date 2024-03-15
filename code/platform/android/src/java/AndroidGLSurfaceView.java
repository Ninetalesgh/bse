package com.bse.core;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;

public class AndroidGLSurfaceView extends GLSurfaceView {

  private final AndroidGLRenderer renderer;

  public AndroidGLSurfaceView(Context context)
  {
    super(context);
    InitOpenGlView();
    renderer = new AndroidGLRenderer();
    setRenderer(renderer);
  }

  public AndroidGLSurfaceView(Context context, AttributeSet attrs) 
  {
    super(context,attrs);
    InitOpenGlView();
    renderer = new AndroidGLRenderer();
    setRenderer(renderer);
  }

  private void InitOpenGlView()
  {
    setEGLContextClientVersion(2);
    setPreserveEGLContextOnPause(true);

  }


}