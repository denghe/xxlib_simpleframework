LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := libuv
LOCAL_SRC_FILES := libuv.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_FORCE_STATIC_EXECUTABLE := true
LOCAL_MODULE := xxuvlib
LOCAL_CPPFLAGS := -O2 -std=gnu++1z -frtti -fexceptions
LOCAL_CFLAGS := -O2 -std=gnu99 -D_FILE_OFFSET_BITS=32

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Sources \
					$(LOCAL_PATH)/../../Sources/libuv_include
					
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../Sources \
					$(LOCAL_PATH)/../../Sources/libuv_include

LOCAL_SRC_FILES := 	../../Sources/xxuvlib.cpp \
					../../Sources/ikcp.cpp
					
LOCAL_WHOLE_STATIC_LIBRARIES += libuv
include $(BUILD_SHARED_LIBRARY)
