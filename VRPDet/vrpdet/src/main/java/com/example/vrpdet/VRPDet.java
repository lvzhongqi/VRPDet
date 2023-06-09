package com.example.vrpdet;

import android.util.Log;
import android.widget.Toast;

public class VRPDet {
    private static String TAG = "lzqtest";
    private static boolean isVA = false;
    private static int counter = 0;
    private static boolean isHook = false;

    private VRPDet() {
    }

    public static void VACheck() {
        AssetsReader.getInstance();
        StackTraceElement[] stackTrace = new Throwable().getStackTrace();
        boolean isVAStack = new VAStackTrace().check(stackTrace);
        boolean isVAPermission = new PermissionCheck().check();
        boolean isVAData = new DataLocCheck().check();
        JniNative jniNative = new JniNative();
        boolean isVAApk = jniNative.baseApkLocCheck();
        boolean isVASoLoc = jniNative.soLocCheck();
        Log.e("lzqtest", "VACheck:" + " isVAStack:" + isVAStack + " isVAPermission:" + isVAPermission + " isVAData:" + isVAData + " isVAApk:" + isVAApk + " isVASoLoc:" + isVASoLoc);
        isVA = isVAStack || isVAPermission || isVAData || isVAApk || isVASoLoc;
        resultDeal();
    }

    public static void HookCheck() {
        StackTraceElement[] stackTrace = new Throwable().getStackTrace();
        isHook = new HookStackTrace().check(stackTrace);
        resultDeal();
    }

    private static void resultDeal() {
        if (isVA) {
            Log.e(TAG, "resultDeal: " + "你在虚拟化环境哦");
            try {
                Toast toast = Toast.makeText(AssetsReader.getApplicationUsingReflection(), "你在虚拟化环境哦", Toast.LENGTH_LONG);
                if (counter <= 0) {
                    toast.show();
                } else {
                    counter++;
                }
            } catch (Exception exception) {
                Log.e(TAG, "resultDeal: " + "false");
                exception.printStackTrace();
            }
        } else {
            Log.e(TAG, "resultDeal: " + "在正常环境中执行哦");
        }
        if (isHook) {
            Log.e(TAG, "resultDeal: " + "你被hook了！！！");
            System.exit(0);
        } else {
            Log.e(TAG, "resultDeal: " + "还没被hook！！！");
        }
    }
}
