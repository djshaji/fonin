LOCAL_PATH := $(call my-dir) 
include $(CLEAR_VARS) 
LOCAL_MODULE := fonin-tubeamp.lv2 
LOCAL_SRC_FILES := tubeamp.c biquad.c
LOCAL_LDLIBS := -llog
include $(BUILD_SHARED_LIBRARY) 
LOCAL_LDFLAGS += -O3
