#pragma once
#include "bse_linux.h"

namespace linux
{
  void register_platform_callbacks();

};


namespace linux
{
  void register_platform_callbacks()
  {
    ////////// Debug ///////////////////////////////////////////////////////////////////////////////////
    //global::platform.debug_log = &debug_log;

    // ////////// Memory //////////////////////////////////////////////////////////////////////////////////
    //global::platform.allocate_virtual_memory = &allocate_virtual_memory;
    // global::platform.free_virtual_memory = &free_virtual_memory;
    // global::platform.decommit_virtual_memory = &decommit_virtual_memory;

    // ////////// File IO /////////////////////////////////////////////////////////////////////////////////
    // global::platform.get_file_info = &get_file_info;
    // global::platform.load_file_part = &load_file_part;
    // global::platform.write_file = &write_file;
    // global::platform.create_directory = &create_directory;
    // global::platform.get_precompiled_asset = &get_precompiled_asset;

    // ////////// Threading /////////////////////////////////////////////////////////////////////////////////
    // global::platform.thread_create = &thread_create;
    // global::platform.mutex_create = &mutex_create;
    // global::platform.mutex_destroy = &close_handle;
    // global::platform.mutex_release = &mutex_release;
    // global::platform.semaphore_create = &semaphore_create;
    // global::platform.semaphore_destroy = &close_handle;
    // global::platform.semaphore_release = &semaphore_release;
    // global::platform.wait_for_locking_object = &wait_for_locking_object;

    ////////// Vulkan ////////////////////////////////////////////////////////////////////////////////////
    //global::platform.vulkan_create_surface = &vulkan_create_surface;
    //global::platform.vulkan_physical_device_supports_presentation = &vulkan_physical_device_supports_presentation;

    // ////////// System //////////////////////////////////////////////////////////////////////////////////
    // global::platform.shutdown = &shutdown;
    // global::platform.opengl_get_process_address = &opengl_get_proc_address;
  }

};