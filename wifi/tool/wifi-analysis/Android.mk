LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_ARM_MODE := arm
LOCAL_MODULE := wifi_analysis_main
LOCAL_MODULE_TAGS := optional
LOCAL_C_INCLUDES := $(LOCAL_PATH) \
					$(LOCAL_PATH)/radiotap-library 
##LOCAL_C_INCLUDES := 
#####external/libpcap/pcap
LOCAL_SHARED_LIBRARIES += \
		libpcap

LOCAL_SRC_FILES := \
                wifi_analysis_main.c \
                radiotap-library/radiotap.c 
include $(BUILD_EXECUTABLE)

######################################

