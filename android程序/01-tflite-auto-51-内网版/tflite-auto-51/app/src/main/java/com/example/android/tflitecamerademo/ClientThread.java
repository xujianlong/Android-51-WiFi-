package com.example.android.tflitecamerademo;

import android.annotation.SuppressLint;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.Socket;
import java.net.SocketTimeoutException;

import static com.example.android.tflitecamerademo.Camera2BasicFragment.IP;
import static com.example.android.tflitecamerademo.Camera2BasicFragment.port;


/**
 * Created by Lenovo on 2017/3/27.
 */

public class ClientThread implements Runnable {

    private Socket s;
    private Handler handler;
    public Handler revHandler;

    BufferedReader br = null;
    OutputStream os = null;

    public ClientThread(Handler handler) {
        this.handler = handler;
    }

    @SuppressLint("HandlerLeak")
    public void run() {
        try {
            s = new Socket(IP, port);
            br = new BufferedReader(new InputStreamReader(s.getInputStream()));
            os = s.getOutputStream();
            new Thread() {
                @Override
                public void run() {
                    Camera2BasicFragment.thread_flag=true;
                    String content =null;
                    try {
                        while (((content = (br.readLine())) != null)) 
                        {
                            Message msg = new Message();
                            msg.what = 0x123;
                            msg.obj = content;
                            handler.sendMessage(msg);
                        }
                    }
                    catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }.start();
            Looper.prepare();
            revHandler = new Handler() {
                @Override
                public void handleMessage(Message msg) {
                    if (msg.what == 0x456) {
                        try {
                            os.write( msg.obj.toString().getBytes());
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    }
                }
            };
            Looper.loop();
        } catch (SocketTimeoutException el) {
            System.out.println("网络连接超时！！");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
