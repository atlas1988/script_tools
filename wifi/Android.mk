LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_ARM_MODE := arm
LOCAL_MODULE := libpcap_1
LOCAL_MODULE_TAGS := optional
LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)
##LOCAL_C_INCLUDES := 
#####external/libpcap/pcap
LOCAL_SHARED_LIBRARIES += \
		libpcap

LOCAL_SRC_FILES := \
                libpcap_1.c
include $(BUILD_EXECUTABLE)

#####################new item########
include $(CLEAR_VARS)
LOCAL_ARM_MODE := arm
LOCAL_MODULE := libpcap_mac
LOCAL_MODULE_TAGS := optional
LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)
##LOCAL_C_INCLUDES := 
#####external/libpcap/pcap
LOCAL_SHARED_LIBRARIES += \
		libpcap

LOCAL_SRC_FILES := \
                libpcap_mac.c
include $(BUILD_EXECUTABLE)

######################################

