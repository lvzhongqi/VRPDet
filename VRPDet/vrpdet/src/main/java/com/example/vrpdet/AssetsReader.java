package com.example.vrpdet;

import android.app.Application;
import android.content.res.AssetManager;
import android.util.Log;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.nio.charset.StandardCharsets;
import java.util.HashSet;

public class AssetsReader {
    private static String TAG = "lzqtest";
    private static AssetsReader instance = null;
    private HashSet<String> packageNameSet = null;
    private HashSet<String> androidPackageNameSet = null;
    private HashSet<String> permissionSet = null;
    private static HashSet<String> soList = null;

    private AssetsReader() {
        init();
    }

    private void init() {
        permissionSet = new HashSet<>();
        androidPackageNameSet = new HashSet<>();
        packageNameSet = new HashSet<>();
        soList = new HashSet<>();
        androidPackageNameSet.add("com.android");
        androidPackageNameSet.add("android.os");
        androidPackageNameSet.add("android.app");
        soList.add("libjninative.so");

        try {
            AssetManager mAssetManger = getApplicationUsingReflection().getAssets();
            InputStream file = mAssetManger.open("result.txt");
            BufferedReader reader = new BufferedReader(new InputStreamReader(file));
            String str = null;
            String sign = "";
            String result = "";
            while ((str = reader.readLine()) != null) {
                if (str.startsWith("permission: ")) {
                    result += str + "\r\n";
                    permissionSet.add(str.replace("permission: ", ""));
                }
                if (str.startsWith("package: ")) {
                    result += str + "\r\n";
                    packageNameSet.add(str.replace("package: ", ""));
                }
                if (str.startsWith("so: ")) {
                    result += str + "\r\n";
                    soList.add(str.replace("so: ", ""));
                }
                if (str.startsWith("sign: ")) {
                    sign = str.replace("sign: ", "");
                }
            }
//            Log.e(TAG, "result: " + result);
//            Log.e(TAG, "sign: " + sign);
            byte[] resultBytes = result.getBytes(StandardCharsets.UTF_8);
            byte[] hexStringToByteArray = hexStringToByteArray(sign);
            JniNative.checkSign(resultBytes, hexStringToByteArray);
            //Log.e(TAG, "init: packageNameSet.size():"+packageNameSet.size());
            //Log.e(TAG, "init: permissionSet.size():"+permissionSet.size());
            //Log.e(TAG, "init: soList.size():"+soList.size());
        } catch (Exception e) {
            e.printStackTrace();
        }
//        Log.e(TAG, "init: " + permissionSet.size());
//        Log.e(TAG, "init: " + androidPackageNameSet.size());
//        Log.e(TAG, "init: " + packageNameSet.size());

    }

    public static AssetsReader getInstance() {
        if (instance == null) {
            instance = new AssetsReader();
        }
        return instance;
    }

    private byte[] hexStringToByteArray(String hexString) {
        int len = hexString.length();
        byte[] byteArray = new byte[len / 2];
        for (int i = 0; i < len; i += 2) {
            byteArray[i / 2] = (byte) ((Character.digit(hexString.charAt(i), 16) << 4)
                    + Character.digit(hexString.charAt(i + 1), 16));
        }
        return byteArray;
    }


    public static Application getApplicationUsingReflection() throws Exception {
        return (Application) Class.forName("android.app.ActivityThread").
                getMethod("currentApplication").
                invoke(null, (Object[]) null);
    }

    public HashSet<String> getPackageNameSet() {
        return packageNameSet;
    }

    public HashSet<String> getAndroidPackageNameSet() {
        return androidPackageNameSet;
    }

    public HashSet<String> getPermissionSet() {
        return permissionSet;
    }

    public static boolean soExist(String soName) {
        //for (String str:soList) Log.e(TAG, "soExist: "+str);
        return soList.contains(soName);
    }
}
