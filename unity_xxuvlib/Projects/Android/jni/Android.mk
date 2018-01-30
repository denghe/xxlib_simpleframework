LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_FORCE_STATIC_EXECUTABLE := true
LOCAL_MODULE := xxnbsocketlib
LOCAL_CPPFLAGS := -O2 -std=gnu++1z -frtti -fexceptions
LOCAL_CFLAGS := -O2 -std=gnu99 -D_FILE_OFFSET_BITS=32

LOCAL_EXPORT_C_INCLUDES :=  ../../Sources

LOCAL_SRC_FILES := 	../../Sources/xxnbsocketlib.cpp
					
include $(BUILD_SHARED_LIBRARY)
