package com.example.vrpdet;


import android.app.Application;
import android.content.pm.PackageManager;

public class PermissionCheck {
    private String TAG = "lzqtest";
    private String[] permissionList= new String[]{
            "android.permission.READ_CALENDAR",
            "android.permission.WRITE_CALENDAR",
            "android.permission.CAMERA.permission.READ_CONTACTS",
            "android.permission.WRITE_CONTACTS",
            "android.permission.GET_ACCOUNTS",
            "android.permission.ACCESS_FINE_LOCATION",
            "android.permission.ACCESS_COARSE_LOCATION",
            "android.permission.RECORD_AUDIO",
            "android.permission.READ_PHONE_STATE",
            "android.permission.CALL_PHONE",
            "android.permission.READ_CALL_LOG",
            "android.permission.ADD_VOICEMAIL",
            "android.permission.USE_SIP",
            "android.permission.PROCESS_OUTGOING_CALLS",
            "android.permission.BODY_SENSORS",
            "android.permission.SEND_SMS",
            "android.permission.RECEIVE_SMS",
            "android.permission.READ_SMS",
            "android.permission.RECEIVE_WAP_PUSH",
            "android.permission.RECEIVE_MMS",
            "android.permission.READ_EXTERNAL_STORAGE",
            "android.permission.WRITE_EXTERNAL_STORAGE"
    };

    public boolean check() {
        PackageManager packageManager = null;
        int unDefinePermissionnums = 0;
        try {
            Application application = AssetsReader.getApplicationUsingReflection();
            packageManager = application.getPackageManager();
            String packageName = application.getPackageName();
            for(String permission:permissionList){
                if(!AssetsReader.getInstance().getPermissionSet().contains(permission)){
                    int per = packageManager.checkPermission(permission, packageName);
                    if (PackageManager.PERMISSION_GRANTED == per) {
                        //Log.e(TAG, "required permission not granted . permission = " + permission);
                        unDefinePermissionnums++;
                    }
                }
            }
        } catch (Exception exception) {
            exception.printStackTrace();
        }
        //Log.e(TAG, "check: unDefinePermissionnums:" + unDefinePermissionnums);
        if (unDefinePermissionnums > 0) return true;
        else return false;
    }
}
