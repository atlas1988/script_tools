LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_ARM_MODE := arm
LOCAL_MODULE := wifi_moniter
LOCAL_MODULE_TAGS := optional
LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)
##LOCAL_C_INCLUDES := 
#####external/libpcap/pcap
LOCAL_SHARED_LIBRARIES += \
		libpcap

LOCAL_SRC_FILES := \
                wifi_stat_change.c
include $(BUILD_EXECUTABLE)

######################################

