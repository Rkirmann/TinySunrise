package com.cyhex.flashcom.lib;

import android.hardware.camera2.CameraAccessException;
import android.hardware.camera2.CameraManager;
import android.util.Log;

public class Transmitter {

    private int timeLow = 40;
    private int timeHigh = 80;
    private int timeLightPulse = 40;

    private CameraManager cam;
    private String mCameraId;

    public int getTimeLow() {
        return timeLow;
    }

    public void setTimeLow(int timeLow) {
        this.timeLow = timeLow;
    }

    public int getTimeHigh() {
        return timeHigh;
    }

    public void setTimeHigh(int timeHigh) {
        this.timeHigh = timeHigh;
    }

    public int getTimeLightPulse() {
        return timeLightPulse;
    }

    public void setTimeLightPulse(int timeLightPulse) {
        this.timeLightPulse = timeLightPulse;
    }

    public Transmitter(CameraManager cam, String mCameraId) {
        this.cam = cam;
        this.mCameraId = mCameraId;
    }

    public void transmit(String str) throws InterruptedException {
        String binaryString = toBinaryString(str);
        Log.i("Transmitter", binaryString);
        on();
        Thread.sleep(1000);
        off();
        for (char c : binaryString.toCharArray()) {
            on();
            Thread.sleep(timeLightPulse);
            off();
            if (c == '0') {
                Thread.sleep(timeLow);
            } else {
                Thread.sleep(timeHigh);
            }
        }
        on();
        Thread.sleep(timeLightPulse);
        off();
        //Thread.sleep(timeHigh*4);
    }

    private void on() {
        try {
            cam.setTorchMode(mCameraId, true);
        } catch (CameraAccessException e) {
            e.printStackTrace();
        }
    }

    private void off() {
        try {
            cam.setTorchMode(mCameraId, false);
        } catch (CameraAccessException e) {
            e.printStackTrace();
        }
    }

    private String toBinaryString(String str) {
        StringBuilder r = new StringBuilder();
        byte[] bytes = str.getBytes();

        for (byte b : bytes) {
            r.append(String.format("%8s", Integer.toBinaryString(b)).replace(' ', '0'));
        }
        return r.toString();

    }

}
