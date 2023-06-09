package com.example.vrpdet;


public class VAStackTrace {
    private static final String TAG = "lzqtest";

    public VAStackTrace() {
        super();
    }


    public boolean check(StackTraceElement[] stackTrace) {
        int other = stackTrace.length;
        for (int i = stackTrace.length - 1; i >= 0; i--) {
            //Log.e(TAG, "check: stackTrace[" + i + "]: " + stackTrace[i].getClassName() + " " + stackTrace[i].getMethodName());
            if (isAndroid(stackTrace[i].getClassName())) {
                if (other != stackTrace.length) return true;
            } else if (isApp(stackTrace[i].getClassName())) {
                return false;
            } else if (!isJava(stackTrace[i].getClassName())) {
                other = i;
            }
        }
        return false;
    }

    private boolean isJava(String className) {
        return className.startsWith("java");
    }

    private boolean isApp(String className) {
        StringBuilder pkg = new StringBuilder();
        String[] split = className.split("\\.");
        for (String s : split) {
            pkg.append(s);
            if (AssetsReader.getInstance().getPackageNameSet().contains(pkg.toString())) {
                return true;
            }
            pkg.append(".");
        }
        return false;
    }

    private boolean isAndroid(String className) {
        StringBuilder pkg = new StringBuilder();
        String[] split = className.split("\\.");
        for (String s : split) {
            pkg.append(s);
            if (AssetsReader.getInstance().getAndroidPackageNameSet().contains(pkg.toString())) {
                return true;
            }
            pkg.append(".");
        }
        return false;
    }

}
