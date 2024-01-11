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
#include <openssl/sha.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/evp.h>

using namespace std;
const char *APP_SO_DATA_APP_PATH = "/data/app/";
int SO_DATA_APP_LEN = strlen(APP_SO_DATA_APP_PATH);
jmethodID midSoExist;


string getCurrentAppProcessName(int pid);

bool startsWith(const char *a, const char *b);

vector<string> getMapsPathInfo(int pid, char *name);


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
    vector<string> v = getMapsPathInfo(pid, "base.apk");
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
            int a = strncmp(apk + SO_DATA_APP_LEN, process, len);
            if (a != 0) {
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
    vector<string> v = getMapsPathInfo(pid, ".so");
    for (auto itt = v.begin(); itt != v.end(); itt++) {
        string path = *itt;
        const char *lib = path.c_str();
        //LOGE("lzqtest %s lib 路径01",lib);
        // APP_SO_DATA_APP_PATH = "/data/app/"
        // SO_DATA_APP_LEN = strlen ( APP_SO_DATA_APP_PATH )
        jbyteArray soPath = env->NewByteArray(strlen(lib));
        env->SetByteArrayRegion(soPath, 0, strlen(lib), (jbyte *) lib);
        if (!env->CallStaticBooleanMethod(clazz, midSoExist, soPath)) {
            //LOGE("lzqtest isExsit false");
            return true;
        }
        process = getCurrentAppProcessName(pid).c_str();
        len = strlen(process);
        if (startsWith(lib, APP_SO_DATA_APP_PATH)) {
            //LOGE("lzqtest lib process %s",process);
            int a = strncmp(lib + SO_DATA_APP_LEN, process, len);
            //LOGE("lzqtest lib %s",lib + SO_DATA_APP_LEN);
            //LOGE("lzqtest a %d",a);
            if (a != 0) {
                return true;
            }
        }
    }
    return false;
}

void print_hex_android_log(const unsigned char *data, size_t len) {
    char buffer[len * 2 + 1];
    for (size_t i = 0; i < len; i++) {
        sprintf(&buffer[i * 2], "%02x", data[i]);
    }
    LOGE("lzqtest %s", buffer);
}

// 公钥解密函数
int rsaPublicDecrypt(const unsigned char *encryptedData, int encryptedDataLength,
                     unsigned char *decryptedData) {
    const char *publicKeyStr = "-----BEGIN PUBLIC KEY-----\n"
                               "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA5hz06Lrs7svSs/MS0jQw\n"
                               "Tmj6MN+9tH0v3pSQd7qwxlcD3lkkLZq7xBvt9x8UF2TmF2u7jtqece2EX2eNADKn\n"
                               "ciw78ySVV7AfHtRH9Zx1g5LnOnQ2CpDwsTWSNt/CtfDeXQ0vNihz3+8VcCXVRnex\n"
                               "urA5g0+FmqdWGrYC2JUoXNIlC6Re2yWx4thy9Lgadb3bYkYIf09JVFHaZEVU4eAu\n"
                               "V159I3eB84Vddw3lcAmTNtOSpon+gCixx4ECzT1eg3GxtG1hZ8IxmedUagsq4xKE\n"
                               "54Uj8EVx8IzTSxJGNG3e899Qf3wd+pg8bvgx11IUjgwd90xc0dc9lWYzcKlu1mZP\n"
                               "pQIDAQAB\n"
                               "-----END PUBLIC KEY-----\n";
    BIO *bio = BIO_new_mem_buf((void *) publicKeyStr, -1);
    if (!bio) {
        LOGE("Failed to create BIO");
        return -1;
    }

    RSA *rsaKey = PEM_read_bio_RSA_PUBKEY(bio, NULL, NULL, NULL);
    if (!rsaKey) {
        LOGE("Failed to read public key");
        BIO_free(bio);
        return -1;
    }

    int padding = RSA_PKCS1_PADDING; // 使用 PKCS1 填充模式
    //print_hex_android_log(encryptedData,encryptedDataLength);
    //print_hex_android_log(decryptedData,32);
    int result = RSA_public_decrypt(encryptedDataLength, encryptedData, decryptedData, rsaKey,
                                    padding);
    //print_hex_android_log(decryptedData,32);
    if (result == -1) {
        LOGE("Failed to decrypt data using public key");
    }

    RSA_free(rsaKey);
    BIO_free(bio);

    return result;
}

jboolean checkSign(JNIEnv *env, jclass clazz, jbyteArray data, jbyteArray sign) {
    int hashLength = 32;
    // TODO: implement checkSign()
    if (data == nullptr || sign == nullptr) {
        LOGE("lzqtest 数据或签名异常");
        abort();
        return JNI_FALSE;
    }
    // 将jbyteArray转换为C数组
    jbyte *dataBytes = env->GetByteArrayElements(data, NULL);
    jbyte *signBytes = env->GetByteArrayElements(sign, NULL);
    // 获取data和sign的长度
    jsize dataLength = env->GetArrayLength(data);
    jsize signLength = env->GetArrayLength(sign);
    if (dataLength == 0 || dataLength == 0) {
        LOGE("lzqtest 数据或签名异常");
        abort();
        return JNI_FALSE;
    }

    unsigned char *decryptedSign = (unsigned char *) malloc(256 * sizeof(unsigned char *));
    unsigned char *signData = (unsigned char *) malloc(hashLength * sizeof(unsigned char *));

    int result = rsaPublicDecrypt((const unsigned char *) signBytes, signLength, decryptedSign);
    //LOGE("lzqtest result %d", result);
    SHA256((const unsigned char *) dataBytes, dataLength, signData);
    //print_hex_android_log(decryptedSign,result);
    //print_hex_android_log(signData,hashLength);
    // 根据验证结果返回对应的jboolean值
    if (memcmp((const unsigned char *) (decryptedSign + result - hashLength), signData,
               hashLength) == 0) {
        LOGE("lzqtest 文件签名验证通过");
        return JNI_TRUE;
    } else {
        LOGE("lzqtest 文件完整性验证失败");
        abort();
        return JNI_FALSE;
    }
}

vector<string> getMapsPathInfo(int pid, char *name) {
    vector<string> v;
    char filepath[32];
    sprintf(filepath, "/proc/%d/maps", pid);
    //LOGE("maps info :%s", filepath);
    FILE *fp = fopen(filepath, "r");
    if (fp == NULL) {
        //LOGE("lzqtest open failed");
    }
    char loc[4096];
    while (fgets(loc, sizeof(loc), fp)) {
        //LOGE("maps info :%s", loc);
        //strstr(loc, "/data")&&
        if (strstr(loc, APP_SO_DATA_APP_PATH) && strstr(loc, name)) {
            // LOGE("lzqtest maps info :%s", loc);
            string s = string(loc);
            int pos = s.find_first_of("/");
            v.push_back(s.substr(pos, s.length() - 1));
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
    if (fp == NULL) {
        //LOGE("lzqtest open failed");
        return NULL;
    }
    char name[100];
    fgets(name, sizeof(name), fp);
    //LOGE("lzqtest %s",name);
    string s = string(name);
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
        },
        {
                "checkSign",
                "([B[B)Z",
                (void *) checkSign
        }
};

static bool registerNativeMethods(JNIEnv *env, const char *className, JNINativeMethod *jniMethods,
                                  int methods) {
    jclass clazz = env->FindClass(className);
    if (clazz == NULL) {
        return false;
    }
    bool ret = env->RegisterNatives(clazz, jniMethods, methods) >= 0;

    midSoExist = env->GetStaticMethodID(clazz, "soExistJni", "([B)Z");
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

