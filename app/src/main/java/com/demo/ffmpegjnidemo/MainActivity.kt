package com.demo.ffmpegjnidemo

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.Surface
import android.view.SurfaceHolder
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {
    val path = "/sdcard/Android/data/com.baling.camera2OpenGl/files/video/test2.mp4"
    private var player: Int? = null;
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        // Example of a call to a native method
        tv.text = ffmpegInfo()
        initSfv()
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String
    external fun ffmpegInfo(): String
    external fun createPlayer(path: String, surface: Surface): Int
    external fun play(player: Int)
    external fun pasue(player: Int);

    companion object {
        // Used to load the 'native-lib' library on application startup.
        init {
            System.loadLibrary("native-lib")
        }
    }

    fun initSfv() {
        sfv.holder.addCallback(object : SurfaceHolder.Callback {
            override fun surfaceChanged(
                holder: SurfaceHolder?,
                format: Int,
                width: Int,
                height: Int
            ) {
                if (player == null) {
                    player = createPlayer(path, holder!!.surface);
                    play(player!!);
                }
            }

            override fun surfaceDestroyed(holder: SurfaceHolder?) {
            }

            override fun surfaceCreated(holder: SurfaceHolder?) {
            }

        })
    }
}
