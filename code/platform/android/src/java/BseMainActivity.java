package com.bse.core;

import com.bse.core.AndroidGLSurfaceView;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;

//import com.bse.core.databinding.ActivityMainBinding;

public class BseMainActivity extends Activity {

    static {
        System.loadLibrary("androidtest");
    }

    AndroidGLSurfaceView openglView;


    @Override protected void onCreate(Bundle savedInstanceState) 
    {
        super.onCreate(savedInstanceState);
        
        //for hello.xml

        // this.setContentView (R.layout.hello);
        // TextView tv = (TextView) this.findViewById (R.id.MainTextView);
        // tv.setText(stringFromJNI());

        stringFromJNI();

        openglView = new AndroidGLSurfaceView(this);

        this.setContentView(openglView);


       // binding = ActivityMainBinding.inflate(getLayoutInflater());
        //this.setContentView(binding.getRoot());

        // Example of a call to a native method
        //tv.setText("WHAT");
    }

    @Override protected void onPause()
    {
      super.onPause();
    }

    @Override protected void onResume()
    {
      super.onResume();
    }



    public native String stringFromJNI();
}