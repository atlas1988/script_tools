LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_ARM_MODE := arm
LOCAL_MODULE := sqlite_main
LOCAL_MODULE_TAGS := optional
LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)
##LOCAL_C_INCLUDES := 
#####external/libpcap/pcap
LOCAL_SHARED_LIBRARIES += \
		libsqlite

LOCAL_SRC_FILES := \
                sqlite_main.c
include $(BUILD_EXECUTABLE)

######################################

