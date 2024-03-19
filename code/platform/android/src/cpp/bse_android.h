#pragma once
#include "core/bse_core.h"


#define VK_USE_PLATFORM_ANDROID_KHR
#include <vulkan/vulkan_android.h>

#include <jni.h>

#include <android/native_activity.h>
#include <android/native_window_jni.h>
#include <android/native_window.h>

#include <errno.h>
#include <sys/mman.h>
#include <sys/unistd.h>

namespace global
{
  static bse::Platform platform;
  static ANativeWindow* window;

};
