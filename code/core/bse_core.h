#pragma once

//#define BSE_BUILD_SKIP_NETWORK
//#define BSE_BUILD_SKIP_GRAPHICS
//#define BSE_BUILD_SKIP_AUDIO
//#define BSE_BUILD_SKIP_INPUT

#include "core/include/bse_core_include.h"

namespace bse
{
  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Callback Function Definitions /////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////
  namespace platformcallback
  {
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////// Debug /////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    using debug_log_fn = void( debug::LogParameters const&, char const* message, s32 messageSize );

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////// Virtual Memory ////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    using allocate_virtual_memory_fn = void* (void* address, s64 size);
    using decommit_virtual_memory_fn = void( void*, s64 size );
    using free_virtual_memory_fn = void( void* );

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////// File IO ///////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    using get_file_info_fn = bool( char const* filePath, file::Info* out_fileInfo );
    using load_file_part_fn = bool( char const* filePath, u64 readOffset, void* targetBuffer, s64 bufferSize );
    using write_file_fn = bool( char const* filePath, void const* data, u32 size, file::WriteFlags );
    using create_directory_fn = bool( char const* directoryPath );
    using get_precompiled_asset_fn = bool( char const* name, void** out_data, u64* out_size );

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////// Threading /////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    using thread_create_fn = thread::ID( thread::entry_fn*, void* parameter );
    using mutex_create_fn = void* ();
    using mutex_destroy_fn = void( void* handle );
    using mutex_release_fn = void( void* handle );
    using semaphore_create_fn = void* (s32 initialCount, s32 maxCount);
    using semaphore_destroy_fn = void( void* handle );
    using semaphore_release_fn = s32( void* handle, s32 releaseCount );
    using wait_for_locking_object_fn = void( void* handle, u32 waitMilliseconds );

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////// System ////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    using shutdown_fn = void();
  };
  //////////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// End of Callback Function Definitions //////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  struct Platform
  {
    ////////// Debug /////////////////////////////////////////////////////////////////////////////////////
    platformcallback::debug_log_fn* debug_log;
    ////////// Memory ////////////////////////////////////////////////////////////////////////////////////
    platformcallback::allocate_virtual_memory_fn* allocate_virtual_memory;
    platformcallback::decommit_virtual_memory_fn* decommit_virtual_memory;
    platformcallback::free_virtual_memory_fn* free_virtual_memory;
    ////////// File IO ///////////////////////////////////////////////////////////////////////////////////
    platformcallback::get_file_info_fn* get_file_info;
    platformcallback::load_file_part_fn* load_file_part;
    platformcallback::write_file_fn* write_file;
    platformcallback::create_directory_fn* create_directory;
    platformcallback::get_precompiled_asset_fn* get_precompiled_asset;
    ////////// Threading /////////////////////////////////////////////////////////////////////////////////
    platformcallback::thread_create_fn* thread_create;
    platformcallback::mutex_create_fn* mutex_create;
    platformcallback::mutex_destroy_fn* mutex_destroy;
    platformcallback::mutex_release_fn* mutex_release;
    platformcallback::semaphore_create_fn* semaphore_create;
    platformcallback::semaphore_destroy_fn* semaphore_destroy;
    platformcallback::semaphore_release_fn* semaphore_release;
    platformcallback::wait_for_locking_object_fn* wait_for_locking_object;

    ////////// System ////////////////////////////////////////////////////////////////////////////////////
    platformcallback::shutdown_fn* shutdown;
    opengl_ext::get_proc_address_fn* opengl_get_process_address;

    //  bse::FileSystem* fileSystem = nullptr;
    //  bse::Font* font = nullptr;
    //  bse::GlyphTable* glyphTable = nullptr;
    // net::NetworkData network;
    //This is to be user defined
   // AppUserData* userData;

    struct Allocators
    {
      memory::VirtualMemoryLayout virtualMemory;
      memory::Arena* temporary[3600];
      memory::Multipool* mainThread;
      memory::Multipool* threadSafe;
    } allocators;

    struct DefaultSystems
    {
      // VirtualFileSystem vfs;

       //ThreadPool for worker threads and keeping track of ids
       //TODO maybe this means every thread gets its own allocator to keep em separate
       //overhead of multipools * threadcount might be ok

       //Font

    };

    struct PlatformInfo
    {
      String executablePath;
      s32 processorCount;
      u32 virtualMemoryPageSize;
      u32 virtualMemoryAllocationGranularity;
      char* virtualMemoryAddressBegin;
      char* virtualMemoryAddressEnd;
      ProcessorArchitecture processorArchitecture;
    } info;

    void* userData;

    //this memory gets invalidated frequently I suppose TODO
    struct FrameInfo
    {
      u64 frameIndex;
      float deltaTime;
      Input input;
    } thisFrame;

    struct
    {
      float timeMultiplier = 1.0f;
      float fpsCap = 30.0f;
      int2  windowSize;
    } settings;

  } extern* platform;

  struct PlatformInitParams
  {
    void* programHandle;
    Platform* platform;
    bool shutdownAfterInitializing;

    struct
    {
      s32 argumentCount;
      char** arguments;
    } commandLine;

    struct // INPUT
    {
      bool skipInitController;
    } input;

    struct // WINDOW
    {
      char const* name;
      int2 size;
      int2 position;
      bool fullscreen;
      bool skipInitWindow;
    } window;

    struct // WORKER THREADS
    {
      s32 syncableCount;
      s32 asyncCount;
    } workerThreads;

    struct // AUDIO
    {
      bool skipInitAudio;
    } audio;

    struct // NETWORK
    {
      bool skipInitNetwork;
    } network;

    struct // CONSOLE
    {
      bool skipInitConsole;
    } console;
  };

  using core_initialize_fn = void( PlatformInitParams*, Platform* );
  extern "C" core_initialize_fn core_initialize_internal;

  using core_on_reload_fn = void( Platform* );
  extern "C" core_on_reload_fn core_on_reload_internal;

  using core_tick_fn = void( Platform* );
  extern "C" core_tick_fn core_tick_internal;
};

#include "core/src/bse_core_src.cpp"