package com.example.vrpdet;

public class HookStackTrace {
    private String TAG = "lzqtest";

    public HookStackTrace() {
        super();
    }

    public boolean check(StackTraceElement[] stackTrace) {
        int lastAndroid = stackTrace.length;
        int lastApp = stackTrace.length;
        int lastJava = stackTrace.length;
        int errorJava = stackTrace.length;
        for (int i = stackTrace.length - 1; i >= 0; i--) {
            //Log.e(TAG, "check: stackTrace[" + i + "]: " + stackTrace[i].getClassName() + " " + stackTrace[i].getMethodName());
            if (isAndroid(stackTrace[i].getClassName())) {
                if (errorJava != stackTrace.length) errorJava = stackTrace.length;
                lastAndroid = i;
            } else if (isApp(stackTrace[i].getClassName())) {
                if ((i + 1) != lastAndroid && (i + 1) != lastApp && (i + 1) != lastJava || errorJava != stackTrace.length) {
                    return true;
                }
                lastApp = i;
            } else if (isJava(stackTrace[i].getClassName())) {
                if ((i + 1) != lastAndroid && (i + 1) != lastApp && (i + 1) != lastJava) {
                    if (lastApp != stackTrace.length) return true;
                    else errorJava = i;
                }
                lastJava = i;
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
