LOCAL_PATH := $(call my-dir)
# repo root = app/jni/src/../../../..
REPO_ROOT := $(LOCAL_PATH)/../../../..
ENGINE_SRC := $(REPO_ROOT)/engine/src
GNULIB := $(REPO_ROOT)/engine/gnulib/lib

include $(CLEAR_VARS)
LOCAL_MODULE := main

# --- engine sources: everything except the editor, tests and legacy glue ---
ENGINE_ALL := $(wildcard $(ENGINE_SRC)/*.cpp)
ENGINE_EXCLUDE := \
  $(ENGINE_SRC)/freedinkedit.cpp \
  $(ENGINE_SRC)/AppFreeDinkedit.cpp \
  $(ENGINE_SRC)/FakeIOGfxDisplay.cpp \
  $(wildcard $(ENGINE_SRC)/Test*.cpp) \
  $(wildcard $(ENGINE_SRC)/test_*.cpp)
ENGINE_FILES := $(filter-out $(ENGINE_EXCLUDE), $(ENGINE_ALL))

# --- the few gnulib portability bits actually needed on bionic ---
GNULIB_FILES := \
  $(GNULIB)/xmalloc.c \
  $(GNULIB)/xalloc-die.c \
  $(GNULIB)/exitfail.c \
  $(GNULIB)/hash.c

# --- SDL2_gfx has no Android.mk upstream: compile it into main ---
GFX_DIR := $(LOCAL_PATH)/../SDL2_gfx
GFX_FILES := \
  $(GFX_DIR)/SDL2_rotozoom.c \
  $(GFX_DIR)/SDL2_gfxPrimitives.c \
  $(GFX_DIR)/SDL2_framerate.c \
  $(GFX_DIR)/SDL2_imageFilter.c

LOCAL_SRC_FILES := $(ENGINE_FILES) $(GNULIB_FILES) $(GFX_FILES)

LOCAL_C_INCLUDES := \
  $(LOCAL_PATH) \
  $(ENGINE_SRC) \
  $(GNULIB) \
  $(GFX_DIR) \
  $(LOCAL_PATH)/../SDL/include \
  $(LOCAL_PATH)/../SDL2_image/include \
  $(LOCAL_PATH)/../SDL2_mixer/include \
  $(LOCAL_PATH)/../SDL2_ttf

LOCAL_CFLAGS := -DHAVE_CONFIG_H -DENABLE_NLS=0 -O2 \
  -Wno-write-strings -Wno-format-security -Wno-deprecated-declarations

LOCAL_SHARED_LIBRARIES := SDL2 SDL2_image SDL2_mixer SDL2_ttf
LOCAL_LDLIBS := -llog -lGLESv1_CM -lGLESv2 -lz

include $(BUILD_SHARED_LIBRARY)
