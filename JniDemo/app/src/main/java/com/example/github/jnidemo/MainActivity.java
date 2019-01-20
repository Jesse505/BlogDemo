package com.example.github.jnidemo;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import java.lang.reflect.Array;
import java.util.Arrays;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    static final String TAG = "zhaoyifei";

    int[] arrays = {7,1,3,5,2,0,4,8,6};

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());
        helloFromJNI();
        printStuInfoAtNative(new Student());
        inputSortArray(arrays);
        Log.i(TAG, Arrays.toString(arrays));
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public native String helloFromJNI();

    public native void printStuInfoAtNative(Student stu);

    public native void inputSortArray(int[] array);

    class Student{
        int age = 21;
        String name = "zhaoyifei";
    }
}
