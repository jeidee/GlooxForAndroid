LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := libcrypto

LOCAL_SRC_FILES := libs/libcrypto.so

include $(PREBUILT_SHARED_LIBRARY)


include $(CLEAR_VARS)

LOCAL_MODULE    := libssl

LOCAL_SRC_FILES := libs/libssl.so

include $(PREBUILT_SHARED_LIBRARY)


include $(CLEAR_VARS)

LOCAL_MODULE    := libgloox

LOCAL_SRC_FILES := libs/libgloox_lib.so

include $(PREBUILT_SHARED_LIBRARY)


include $(CLEAR_VARS)

LOCAL_MODULE    := msg_lib
LOCAL_SRC_FILES :=  msg_lib.cpp \
                    msg_client.cpp \
                    jni_util.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/openssl/include/
LOCAL_C_INCLUDES += $(LOCAL_PATH)/openssl/

LOCAL_SHARED_LIBRARIES := libssl \ libcrypto \ libgloox

LOCAL_LDLIBS := -llog
LOCAL_LDLIBS += -lz

include $(BUILD_SHARED_LIBRARY)