package com.example.vrpdet;

public class JniNative {
    public JniNative() {
        System.loadLibrary("jninative");
    }
    public static native boolean baseApkLocCheck();

    public static native boolean soLocCheck();
    public static native boolean checkSign(byte[] data,byte[] sign);
    public static  boolean soExistJni(byte[] path){
        String sPath = new String(path);
        //Log.e("lzqtest", "soExistJni: path "+sPath);
        String[] split = sPath.split("/");
        //Log.e("lzqtest", "soExistJni: so "+split[split.length-1].replace("\n",""));
        boolean soExist = AssetsReader.soExist(split[split.length - 1].replace("\n",""));
        return soExist;
    }
}
