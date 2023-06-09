LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
PROJECT_ROOT:= $(LOCAL_PATH)
### 设置头文件路径
LOCAL_C_INCLUDES += $(PROJECT_ROOT) \

### source code list
LOCAL_SRC_FILES :=  $(PROJECT_ROOT)/jninative.cpp \

LOCAL_MODULE := jninative

LOCAL_LDLIBS	:= -lm -llog

include $(BUILD_SHARED_LIBRARY)