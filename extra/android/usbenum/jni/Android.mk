LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS) 

LOCAL_MODULE    := usbenum  

LOCAL_CPP_EXTENSION := .cpp .cc .cxx

LOCAL_SRC_FILES :=  ../src/main.cpp 

LOCAL_C_INCLUDES := ../../lib/serial/include

LOCAL_FORCE_STATIC_EXECUTABLE := true

#LOCAL_LDFLAGS := -static 

include $(BUILD_EXECUTABLE)