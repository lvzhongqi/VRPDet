package com.example.vrpdet;


import java.util.HashSet;

public class DataLocCheck {
    private String TAG = "lzqtest";
    private String dataData = "/data/data/";
    private String dataApp = "/data/app/";
    private String dataUser = "/data/user/0/";

    public boolean check() {
        try {
            String absolutePath = AssetsReader.getApplicationUsingReflection().getDataDir().getAbsolutePath();
            HashSet<String> packageNameSet = AssetsReader.getInstance().getPackageNameSet();
            //Log.e(TAG, "VACheck: getDataDir:" + absolutePath);
            if (absolutePath.startsWith(dataData)) {
                String substring = absolutePath.substring(dataData.length());
                String[] split = substring.split("/");
                for(String s: packageNameSet){
                    if(split[0].startsWith(s)){
                        return false;
                    }
                }
                return true;
            }
            if (absolutePath.startsWith(dataApp)) {
                String substring = absolutePath.substring(dataApp.length());
                String[] split = substring.split("/");
                for(String s: packageNameSet){
                    if(split[0].startsWith(s)){
                        return false;
                    }
                }
                return true;
            }
            if (absolutePath.startsWith(dataUser)) {
                String substring = absolutePath.substring(dataUser.length());
                String[] split = substring.split("/");
                for(String s:packageNameSet){
                    if(split[0].startsWith(s)){
                        return false;
                    }
                }
                return true;
            }
        } catch (Exception exception) {
            exception.printStackTrace();
        }
        return false;
    }
}
