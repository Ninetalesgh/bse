package com.bse.core;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;

//import com.bse.core.databinding.ActivityMainBinding;

public class BseMainActivity extends Activity {

    // Used to load the 'bse_android' library on application startup.
    static {
        System.loadLibrary("bse_core");
    }


    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
                
        this.setContentView (R.layout.hello);

       // binding = ActivityMainBinding.inflate(getLayoutInflater());
        //this.setContentView(binding.getRoot());

        // Example of a call to a native method
        TextView tv = (TextView) this.findViewById (R.id.MainTextView);
        tv.setText("WHAT");
        tv.setText(stringFromJNI());
    }

    /**
     * A native method that is implemented by the 'androidcpptest' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
}