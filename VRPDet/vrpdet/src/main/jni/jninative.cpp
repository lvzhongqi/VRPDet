//
// Created by dell on 2022/11/29.
//

#include "jninative.h"
#include "log.h"
#include <jni.h>
#include <string>
#include <unistd.h>
#include <sys/syscall.h>
#include <vector>
#include <stdio.h>

using namespace std;
const char *APP_SO_DATA_APP_PATH = "/data/app/";
int SO_DATA_APP_LEN = strlen(APP_SO_DATA_APP_PATH);
jmethodID midSoExist;


string getCurrentAppProcessName(int pid);

bool startsWith(const char *a, const char *b);

vector<string> getMapsPathInfo( int pid, char *name );



jboolean baseApkLocCheck(JNIEnv *env, jclass clazz) {
    // TODO: implement baseApkLocCheck()
    //LOGE("lzqtest implement baseApkLocCheck()");
    //LOGE("lzqtest pid 01");
    int pid = (int) syscall(__NR_getpid);
    //LOGE("lzqtest pid %d",pid);
    const char *process = getCurrentAppProcessName(pid).c_str();
    if (process == NULL) return false;
    size_t len = strlen(process);
    //LOGE("lzqtest %s %d",process,len);
    vector<string> v=getMapsPathInfo(pid, "base.apk");
    //LOGE("lzqtest maps info :%d", v.size());
    for (auto itt = v.begin(); itt != v.end(); itt++) {
        string path = *itt;
        const char *apk = path.c_str();
        //LOGE("lzqtest %s apk 路径01",apk);
        process = getCurrentAppProcessName(pid).c_str();
        len = strlen(process);
        if (startsWith(apk, APP_SO_DATA_APP_PATH)) {
            //LOGE("lzqtest %s apk 路径02",apk+ SO_DATA_APP_LEN);
            //LOGE("lzqtest %s %d",process,len);
            int a =strncmp(apk + SO_DATA_APP_LEN,process, len);
            if (a!=0){
                //LOGE("lzqtest %s VA apk 路径03 %d",apk,a);
                return true;
            }
        }
    }
    return false;
}


jboolean soLocCheck(JNIEnv *env, jclass clazz) {
    // TODO: implement soLocCheck()
    //LOGE("lzqtest implement soLocCheck()");
    int pid = (pid_t) syscall(__NR_getpid);
    //LOGE("lzqtest pid %d",pid);
    //Get current process name
    const char *process = getCurrentAppProcessName(pid).c_str();
    if (process == NULL) return false;
    size_t len = strlen(process);
    //LOGE("lzqtest %s %d",process,len);
    vector<string> v=getMapsPathInfo(pid, ".so");
    for (auto itt = v.begin(); itt != v.end(); itt++) {
        string path = *itt;
        const char *lib = path.c_str();
        //LOGE("lzqtest %s lib 路径01",lib);
        // APP_SO_DATA_APP_PATH = "/data/app/"
        // SO_DATA_APP_LEN = strlen ( APP_SO_DATA_APP_PATH )
        jbyteArray soPath = env->NewByteArray (strlen(lib));
        env->SetByteArrayRegion(soPath,0,strlen(lib),(jbyte*)lib);
        if (!env->CallStaticBooleanMethod(clazz,midSoExist,soPath)) {
            //LOGE("lzqtest isExsit false");
            return true;
        }
        process = getCurrentAppProcessName(pid).c_str();
        len = strlen(process);
        if (startsWith(lib, APP_SO_DATA_APP_PATH)) {
            //LOGE("lzqtest lib process %s",process);
            int a =strncmp(lib + SO_DATA_APP_LEN,process, len);
            //LOGE("lzqtest lib %s",lib + SO_DATA_APP_LEN);
            //LOGE("lzqtest a %d",a);
            if (a!=0){
                return true;
            }
        }
    }
    return false;
}
vector<string> getMapsPathInfo( int pid, char *name ){
    vector<string> v;
    char filepath[32];
    sprintf(filepath, "/proc/%d/maps", pid);
    //LOGE("maps info :%s", filepath);
    FILE *fp = fopen(filepath, "r");
    if(fp == NULL){
        //LOGE("lzqtest open failed");
    }
    char loc[4096];
    while(fgets(loc, sizeof(loc), fp)){
        //LOGE("maps info :%s", loc);
        //strstr(loc, "/data")&&
        if(strstr(loc, APP_SO_DATA_APP_PATH)&&strstr(loc, name)){
           // LOGE("lzqtest maps info :%s", loc);
            string s =string(loc);
            int pos = s.find_first_of("/");
            v.push_back(s.substr(pos,s.length()-1));
            //LOGE("maps info :%d", v.size());
        }
    }
    return v;
}

bool startsWith(const char *a, const char *b) {
    int lenb = strlen(b);
    if (strlen(a) >= lenb) {
        for (int i = 0; i < lenb; i++) {
            if (a[i] != b[i])return false;
        }
        return true;
    }
    return false;
}

string getCurrentAppProcessName(int pid) {
    char filepath[32];
    sprintf(filepath, "/proc/%d/cmdline", pid);
    FILE *fp = fopen(filepath, "r");
    if(fp == NULL){
        //LOGE("lzqtest open failed");
        return NULL;
    }
    char name[100];
    fgets(name, sizeof(name), fp);
    //LOGE("lzqtest %s",name);
    string s =string(name);
    return s;
}

static JNINativeMethod jniJniNative[] = {
        {
                "baseApkLocCheck",
                "()Z",
                (void *) baseApkLocCheck
        },
        {
                "soLocCheck",
                "()Z",
                (void *) soLocCheck
        }
};

static bool registerNativeMethods(JNIEnv *env, const char *className, JNINativeMethod *jniMethods,
                                  int methods) {
    jclass clazz = env->FindClass(className);
    if (clazz == NULL) {
        return false;
    }
    bool ret = env->RegisterNatives(clazz,jniMethods,methods)>=0;

    midSoExist = env->GetStaticMethodID(clazz,"soExistJni", "([B)Z");
    return ret;
}

jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {

    const char *CLASS_JAVA = "com/example/vrpdet/JniNative";

    int jniMethodSize = sizeof(JNINativeMethod);

    JNIEnv *env = NULL;

    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }


    if (!registerNativeMethods(env, CLASS_JAVA, jniJniNative,
                               sizeof(jniJniNative) / jniMethodSize)) {
        return -1;
    }

    //LOGE("lzqtest JNI Loaded");

    return JNI_VERSION_1_6;
}

