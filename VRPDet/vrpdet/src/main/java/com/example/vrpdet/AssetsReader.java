package com.example.vrpdet;

import android.app.Application;
import android.content.res.AssetManager;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
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
            while ((str = reader.readLine()) != null) {
                if (str.startsWith("permission: ")) {
                    permissionSet.add(str.replace("permission: ", ""));
                }
                if (str.startsWith("package: ")) {
                    packageNameSet.add(str.replace("package: ", ""));
                }
                if (str.startsWith("so: ")) {
                    soList.add(str.replace("so: ", ""));
                }
            }
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

    public static boolean soExist( String soName) {
        //for (String str:soList) Log.e(TAG, "soExist: "+str);
        return soList.contains(soName);
    }
}
