#pragma once 
#include "bse_android.h"

#include <android/log.h>

namespace android
{
  void register_platform_callbacks();

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Debug /////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  void debug_log( bse::debug::LogParameters const& parameters, char const* string, s32 size );

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Memory ////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  void* memory_allocate_virtual( s64 size );
  void memory_free_virtual( void* allocationToFree, s64 size );
  void memory_decommit_virtual( void* committedMemory, s64 size );

  // //////////////////////////////////////////////////////////////////////////////////////////////////////
  // ////////// File IO ///////////////////////////////////////////////////////////////////////////////////
  // //////////////////////////////////////////////////////////////////////////////////////////////////////

  // bool get_file_info( char const* filePath, bse::file::Info* out_fileInfo );
  // bool load_file_part( char const* filePath, u64 readOffset, void* targetBuffer, s64 bufferSize );
  // bool write_file( char const* filePath, void const* data, u32 size, bse::file::WriteFlags flags );
  // bool create_directory( char const* directoryPath );
  // bool get_precompiled_asset( char const* name, void** out_data, u64* out_size );

  // //////////////////////////////////////////////////////////////////////////////////////////////////////
  // ////////// Threading /////////////////////////////////////////////////////////////////////////////////
  // //////////////////////////////////////////////////////////////////////////////////////////////////////

  // void* mutex_create();
  // void mutex_lock( void* handle, u32 waitMilliseconds );
  // void mutex_release( void* handle );
  // void* semaphore_create( s32 initialCount, s32 maxCount );
  // void semaphore_wait( void* handle, u32 waitMilliseconds );
  // s32 semaphore_release( void* handle, s32 releaseCount );

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Vulkan ////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  VkSurfaceKHR vulkan_create_surface( VkInstance instance );
  bool vulkan_physical_device_supports_presentation( VkPhysicalDevice, u32 familyIndex );

  // //////////////////////////////////////////////////////////////////////////////////////////////////////
  // ////////// System ////////////////////////////////////////////////////////////////////////////////////
  // //////////////////////////////////////////////////////////////////////////////////////////////////////

  // void shutdown();

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Networking ////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////



  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Audio /////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////
};



namespace android
{
  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Debug /////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  void debug_log( bse::debug::LogParameters const& parameters, char const* string, s32 size )
  {
    s32 prio = parameters.severity == bse::debug::LogSeverity::BSE_LOG_SEVERITY_INFO ? ANDROID_LOG_INFO :
      parameters.severity == bse::debug::LogSeverity::BSE_LOG_SEVERITY_WARNING ? ANDROID_LOG_WARN :
      parameters.severity == bse::debug::LogSeverity::BSE_LOG_SEVERITY_ERROR ? ANDROID_LOG_ERROR : ANDROID_LOG_DEFAULT;

    __android_log_write( prio, "[BSE]", string );
  };

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Memory ////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  void* memory_allocate_virtual( s64 size )
  {
    if ( size > MegaBytes( 1 ) )
    {
      s64 MBs = 1 + size / MegaBytes( 1 );
      debug_log_info( "Reserving ", MBs, " MegaBytes of virtual Memory." );
    }
    else if ( size > KiloBytes( 1 ) )
    {
      s64 KBs = 1 + size / KiloBytes( 1 );
      debug_log_info( "Reserving ", KBs, " KiloBytes of virtual Memory." );
    }
    else
    {
      debug_log_info( "Reserving ", size, " Bytes of virtual Memory." );
    }

    s64 pageAlignment = (bse::platform->info.virtualMemoryPageSize - (size % bse::platform->info.virtualMemoryPageSize)) % bse::platform->info.virtualMemoryPageSize;

    if ( pageAlignment )
    {
      debug_log_info( "Page alignment increases the allocation from ", size, " to ", size + pageAlignment );
    }

    void* result = mmap( 0, size + pageAlignment, PROT_READ | PROT_WRITE | PROT_EXEC,
            MAP_PRIVATE | MAP_ANONYMOUS, -1, 0 );

    if ( result == MAP_FAILED )
    {
      log_info( "mmap() failed with errno: ", errno );
      assert( false );
    }
    return result;
  }

  // void memory_decommit_virtual( void* committedMemory, s64 size )
  // {
  //   if ( VirtualFree( committedMemory, size, MEM_DECOMMIT ) )
  //   {
  //     windows_error();
  //   }
  // }

  void memory_free_virtual( void* allocationToFree, s64 size )
  {
    if ( munmap( allocationToFree, size ) != 0 )
    {
      log_error( "free virtual memory failed with errno: ", errno );
    }
  }

  // //////////////////////////////////////////////////////////////////////////////////////////////////////
  // ////////// File IO ///////////////////////////////////////////////////////////////////////////////////
  // //////////////////////////////////////////////////////////////////////////////////////////////////////

  // bool get_file_info( char const* filePath, bse::file::Info* out_fileInfo )
  // {
  //   bool result = false;
  //   wchar_t wideChars[BSE_PATH_MAX];
  //   utf8_to_wchar( filePath, wideChars, BSE_PATH_MAX );

  //   _WIN32_FIND_DATAW findData;
  //   HANDLE findHandle = FindFirstFileW( wideChars, &findData );
  //   if ( findHandle != INVALID_HANDLE_VALUE )
  //   {
  //     FindClose( findHandle );
  //     if ( out_fileInfo )
  //     {
  //       out_fileInfo->size = u64( findData.nFileSizeLow ) + (u64( findData.nFileSizeHigh ) << 32);
  //     }
  //     result = true;
  //   }
  //   else
  //   {
  //     result = false; //file or directory doesn't exist 
  //   }

  //   return result;
  // }

  // bool load_file_part( char const* filePath, u64 readOffset, void* targetBuffer, s64 bufferSize )
  // {
  //   if ( readOffset )
  //   {
  //     BREAK; //TODO
  //   }

  //   DWORD bytesToRead = bufferSize > (s64) U32_MAX ? U32_MAX : (u32) bufferSize;

  //   bool result = false;
  //   if ( targetBuffer )
  //   {
  //     wchar_t wideChars[BSE_PATH_MAX];
  //     utf8_to_wchar( filePath, wideChars, BSE_PATH_MAX );
  //     HANDLE fileHandle = CreateFileW( wideChars,
  //                                      GENERIC_READ,
  //                                      FILE_SHARE_READ, 0,
  //                                      OPEN_EXISTING,
  //                                      FILE_ATTRIBUTE_NORMAL, 0 );

  //     if ( fileHandle != INVALID_HANDLE_VALUE )
  //     {
  //       DWORD bytesRead;
  //       if ( ReadFile( fileHandle, targetBuffer, bytesToRead, &bytesRead, 0 ) )
  //       {
  //         result = true;
  //       }

  //       CloseHandle( fileHandle );
  //     }
  //   }

  //   return result;
  // }

  // bool write_file( char const* filePath, void const* data, u32 size, bse::file::WriteFlags flags )
  // {
  //   bool result = true;
  //   wchar_t wideChars[BSE_PATH_MAX];
  //   utf8_to_wchar( filePath, wideChars, BSE_PATH_MAX );
  //   HANDLE fileHandle = nullptr;

  //   DWORD writeMode = (flags == bse::file::WriteFlags::AppendOrFail) ? FILE_APPEND_DATA : GENERIC_WRITE;
  //   DWORD createMode = (flags == bse::file::WriteFlags::OverwriteOrCreateNew) ? CREATE_ALWAYS : OPEN_EXISTING;

  //   fileHandle = CreateFileW( wideChars, writeMode, 0, 0, createMode, FILE_ATTRIBUTE_NORMAL, 0 );

  //   if ( fileHandle != INVALID_HANDLE_VALUE )
  //   {
  //     DWORD bytesWritten;
  //     u8 const* reader = (u8 const*) data;
  //     u32 sizeLeft = size;
  //     u32 const MAX_WRITE = (u32) KiloBytes( 4 );

  //     while ( sizeLeft > 0 )
  //     {
  //       if ( WriteFile( fileHandle, reader, min( MAX_WRITE, sizeLeft ), &bytesWritten, 0 ) )
  //       {
  //         assert( bytesWritten <= sizeLeft );
  //         sizeLeft -= bytesWritten;
  //         reader += bytesWritten;
  //       }
  //       else
  //       {
  //         result = false;
  //         BREAK;
  //         //  log_error( "[FILE]", "ERROR - couldn't write data to file: ", filePath );
  //         break;
  //       }
  //     }
  //   }
  //   else
  //   {
  //     result = false;
  //   }

  //   CloseHandle( fileHandle );

  //   return result;
  // }

  // bool create_directory( char const* directoryPath )
  // {
  //   wchar_t wideChars[BSE_PATH_MAX];
  //   utf8_to_wchar( directoryPath, wideChars, BSE_PATH_MAX );

  //   return CreateDirectoryW( wideChars, NULL );
  // }

  // bool get_precompiled_asset( char const* name, void** out_data, u64* out_size )
  // {
  //   s32 precompiledAssetCount = array_count( compiledasset::assetIndex );
  //   for ( s32 i = 0; i < precompiledAssetCount; ++i )
  //   {
  //     if ( bse::string_match( compiledasset::assetIndex[i].name, name ) )
  //     {
  //       if ( out_data )
  //       {
  //         *out_data = compiledasset::assetIndex[i].data;
  //       }
  //       if ( out_size )
  //       {
  //         *out_size = compiledasset::assetIndex[i].size;
  //       }

  //       return true;
  //     }
  //   }

  //   return false;
  // }

  // //////////////////////////////////////////////////////////////////////////////////////////////////////
  // ////////// Threading /////////////////////////////////////////////////////////////////////////////////
  // //////////////////////////////////////////////////////////////////////////////////////////////////////

  // bse::thread::ID thread_create( bse::thread::entry_fn* fn, void* parameter )
  // {
  //   bse::thread::ID id;
  //   CloseHandle( CreateThread( 0, 0, fn, parameter, 0, (LPDWORD) &id ) );
  //   return id;
  // }

  // void* mutex_create()
  // {
  //   HANDLE result = CreateMutexExW( 0, 0, 0, MUTEX_ALL_ACCESS );
  //   if ( !result )
  //   {
  //     windows_error();
  //   }
  //   return result;
  // }

  // void mutex_lock( void* handle, u32 waitMilliseconds )
  // {
  //   DWORD const result = WaitForSingleObjectEx( handle, waitMilliseconds, FALSE );
  //   if ( result == WAIT_FAILED )
  //   {
  //     windows_error();
  //   }
  // }

  // void mutex_release( void* handle )
  // {
  //   if ( !ReleaseMutex( handle ) )
  //   {
  //     windows_error();
  //   }
  // }

  // void* semaphore_create( s32 initialCount, s32 maxCount )
  // {
  //   HANDLE result = CreateSemaphoreExW( 0, initialCount, maxCount, nullptr, 0, SEMAPHORE_ALL_ACCESS );
  //   if ( !result )
  //   {
  //     windows_error();
  //   }
  //   return result;
  // }

  // void wait_for_locking_object( void* handle, u32 waitMilliseconds )
  // {
  //   DWORD const result = WaitForSingleObjectEx( handle, waitMilliseconds, FALSE );
  //   if ( result == WAIT_FAILED )
  //   {
  //     windows_error();
  //   }
  // }

  // s32 semaphore_release( void* handle, s32 releaseCount )
  // {
  //   long out;
  //   if ( !ReleaseSemaphore( handle, releaseCount, &out ) )
  //   {
  //     windows_error();
  //   }
  //   return s32( out );
  // }

  // void close_handle( void* handle )
  // {
  //   if ( !CloseHandle( handle ) )
  //   {
  //     windows_error();
  //   }
  // }

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Vulkan ////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  VkSurfaceKHR vulkan_create_surface( VkInstance instance )
  {
    VkSurfaceKHR result = 0;
    VkAndroidSurfaceCreateInfoKHR createInfo = { VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR };
    if ( global::window == nullptr )
    {
      log_error( "attempting to create vulkan surface without ANativeWindow being initialized." );
    }
    else
    {
      createInfo.flags = 0;
      createInfo.window = global::window;

      BSE_VK_CHECK( vkCreateAndroidSurfaceKHR( instance, &createInfo, 0, &result ) );
    }
    return result;
  }

  bool vulkan_physical_device_supports_presentation( VkPhysicalDevice, u32 familyIndex )
  {
    return true;
  }

  // //////////////////////////////////////////////////////////////////////////////////////////////////////
  // ////////// System ////////////////////////////////////////////////////////////////////////////////////
  // //////////////////////////////////////////////////////////////////////////////////////////////////////

  // void shutdown()
  // {
  //   global::running = false;
  // }

  // void* opengl_get_proc_address( char const* functionName )
  // {
  //   void* p = (void*) wglGetProcAddress( functionName );
  //   if ( p == 0 ||
  //     (p == (void*) 0x1) || (p == (void*) 0x2) || (p == (void*) 0x3) ||
  //     (p == (void*) -1) )
  //   {
  //     p = (void*) GetProcAddress( android::global::openglDll, functionName );
  //   }

  //   return p;
  // }

  //TODO time here
  //GetSystemTimePreciseAsFileTime -> FILETIME //64bit value 100ns interval since 12:00 1/1/1601
  //GetSystemTime or FileTimeToSystemTime -> SYSTEMTIME //human readable? 

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Networking ////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////



  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Audio /////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////



  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Register Callbacks ////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  void register_platform_callbacks()
  {
    ////////// Debug ///////////////////////////////////////////////////////////////////////////////////
    global::platform.debug_log = &debug_log;

    // ////////// Memory //////////////////////////////////////////////////////////////////////////////////
    global::platform.memory_allocate_virtual = &memory_allocate_virtual;
    // global::platform.memory_free_virtual = &memory_free_virtual;
    // global::platform.memory_decommit_virtual = &memory_decommit_virtual;

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
    global::platform.vulkan_create_surface = &vulkan_create_surface;
    global::platform.vulkan_physical_device_supports_presentation = &vulkan_physical_device_supports_presentation;

    // ////////// System //////////////////////////////////////////////////////////////////////////////////
    // global::platform.shutdown = &shutdown;
    // global::platform.opengl_get_process_address = &opengl_get_proc_address;
  }
};
