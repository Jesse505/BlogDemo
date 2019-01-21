package com.example.github.jnidemo;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    static final String TAG = "zhaoyifei";

    int[] arrays = {7, 1, 3, 5, 2, 0, 4, 8, 6};

    private static String static_str= "静态属性";

    private String str = "成员属性";

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
        Log.i(TAG, "Java层传递给Native层数组，升序排序后：" + Arrays.toString(arrays));
        Log.i(TAG, "Native层返回的数组：" + Arrays.toString(getArray(10)));
        for (int i = 0; i < 3; i++) {
            Log.i(TAG, "Native层返回的二维数组：" + Arrays.toString(getTwoArray(3)[i]));
        }
        try {
            Log.i(TAG, "Native层返回的复杂对象 " + getStuInfo().toString());
        } catch (Throwable t) {
            Log.i(TAG, "error: " + t.getMessage());
        }

        ArrayList<Student> students = getListStudents();
        for (Student stu : students) {
            Log.i(TAG, "Native层返回的集合对象：" + stu.toString());
        }

        accessField();
        Log.i(TAG, "str：" + str);

        accessStaticField();
        Log.i(TAG, "static_str：" + static_str);

        accessMethod();

        accessStaticMethod();
    }


    public static class Student {
        int age = 21;
        String name = "zhaoyifei";

        public Student() {
        }

        public Student(int age, String name) {
            this.age = age;
            this.name = name;
        }

        @Override
        public String toString() {
            return "name:" + name + " age:" + age;
        }
    }

    private int sum(int a, int b) {
        return a + b;
    }

    private static int diff(int a, int b) {
        return a - b;
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public native String helloFromJNI();

    public native void printStuInfoAtNative(Student stu);

    public native void inputSortArray(int[] array);

    public native int[] getArray(int len);

    public native int[][] getTwoArray(int len);

    public native Student getStuInfo();

    public native ArrayList<Student> getListStudents();

    public native void accessField();

    public native void accessStaticField();

    public native void accessMethod();

    public native void accessStaticMethod();
}
