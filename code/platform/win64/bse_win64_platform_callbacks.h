#pragma once

#include "bse_win64.h"

namespace win64
{
  void register_platform_callbacks();

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Debug /////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  void debug_log( bse::debug::LogParameters const& parameters, char const* string, s32 size );

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Memory ////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  void* allocate_virtual_memory( s64 size );
  void free_virtual_memory( void* allocationToFree, s64 size );
  void decommit_virtual_memory( void* committedMemory, s64 size );

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// File IO ///////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  bool get_file_info( char const* filePath, bse::file::Info* out_fileInfo );
  bool load_file_part( char const* filePath, u64 readOffset, void* targetBuffer, s64 bufferSize );
  bool write_file( char const* filePath, void const* data, u32 size, bse::file::WriteFlags flags );
  bool create_directory( char const* directoryPath );
  bool get_precompiled_asset( char const* name, void** out_data, u64* out_size );

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Threading /////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  void* mutex_create();
  void mutex_lock( void* handle, u32 waitMilliseconds );
  void mutex_release( void* handle );
  void* semaphore_create( s32 initialCount, s32 maxCount );
  void semaphore_wait( void* handle, u32 waitMilliseconds );
  s32 semaphore_release( void* handle, s32 releaseCount );

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Vulkan ////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  VkSurfaceKHR vulkan_create_surface( VkInstance instance );
  bool vulkan_physical_device_supports_presentation( VkPhysicalDevice physicalDevice, u32 familyIndex );

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// System ////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  void shutdown();

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Networking ////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////



  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Audio /////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////
};

namespace win64
{
  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Debug /////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  void debug_log( bse::debug::LogParameters const& parameters, char const* string, s32 size )
  {
    //wchar_t wideChars[bs::debug::MAX_DEBUG_MESSAGE_LENGTH];
   // utf8_to_wchar( string, wideChars, array_count( wideChars ) );

   // OutputDebugStringA( string );
    static char const* errorPrefix = "[Error] ";
    static char const* warningPrefix = "[Warning] ";
    static char const* infoPrefix = "";

    // char const* prefix = parameters.severity == bse::debug::LogSeverity::BSE_LOG_SEVERITY_INFO ? infoPrefix :
     //  parameters.severity == bse::debug::LogSeverity::BSE_LOG_SEVERITY_WARNING ? warningPrefix : errorPrefix;

     //printf( prefix );

    printf( string );
    // if ( flags & bs::DebugLogFlags::WRITE_TO_DEBUG_LOG_FILE )
    // {
     //  static HANDLE debug_log_file = CreateFileW( L"debug.log", GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0 );
     //  s32 bytesWritten {};

       // LockFile(debug_log_file, dwPos, 0, dwBytesRead, 0); 
     //  WriteFile( debug_log_file, string, size, (LPDWORD) &bytesWritten, 0 );
       // UnlockFile(debug_log_file, dwPos, 0, dwBytesRead, 0);
    // }
  };

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Memory ////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  void* allocate_virtual_memory( s64 size )
  {
    // if ( size > MegaBytes( 1 ) )
    // {
    //   s64 MBs = 1 + size / MegaBytes( 1 );
    //   debug_log_info( "Reserving ", MBs, " MegaBytes of virtual Memory." );
    // }
    // else if ( size > KiloBytes( 1 ) )
    // {
    //   s64 KBs = 1 + size / KiloBytes( 1 );
    //   debug_log_info( "Reserving ", KBs, " KiloBytes of virtual Memory." );
    // }
    // else
    // {
    //   debug_log_info( "Reserving ", size, " Bytes of virtual Memory." );
    // }

    void* result = VirtualAlloc( 0, (s64) size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE );
    if ( !result )
    {
      windows_error();
    }
    return result;
  }

  void decommit_virtual_memory( void* committedMemory, s64 size )
  {
    if ( VirtualFree( committedMemory, size, MEM_DECOMMIT ) )
    {
      windows_error();
    }
  }

  void free_virtual_memory( void* allocationToFree, s64 size )
  {
    //free( allocationToFree );
    if ( !VirtualFree( allocationToFree, 0, MEM_RELEASE ) )
    {
      windows_error();
    }
  }

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// File IO ///////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  bool get_file_info( char const* filePath, bse::file::Info* out_fileInfo )
  {
    bool result = false;
    wchar_t wideChars[BSE_PATH_MAX];
    utf8_to_wchar( filePath, wideChars, BSE_PATH_MAX );

    _WIN32_FIND_DATAW findData;
    HANDLE findHandle = FindFirstFileW( wideChars, &findData );
    if ( findHandle != INVALID_HANDLE_VALUE )
    {
      FindClose( findHandle );
      if ( out_fileInfo )
      {
        out_fileInfo->size = u64( findData.nFileSizeLow ) + (u64( findData.nFileSizeHigh ) << 32);
      }
      result = true;
    }
    else
    {
      result = false; //file or directory doesn't exist 
    }

    return result;
  }

  bool load_file_part( char const* filePath, u64 readOffset, void* targetBuffer, s64 bufferSize )
  {
    if ( readOffset )
    {
      BREAK; //TODO
    }

    DWORD bytesToRead = bufferSize > (s64) U32_MAX ? U32_MAX : (u32) bufferSize;

    bool result = false;
    if ( targetBuffer )
    {
      wchar_t wideChars[BSE_PATH_MAX];
      utf8_to_wchar( filePath, wideChars, BSE_PATH_MAX );
      HANDLE fileHandle = CreateFileW( wideChars,
                                       GENERIC_READ,
                                       FILE_SHARE_READ, 0,
                                       OPEN_EXISTING,
                                       FILE_ATTRIBUTE_NORMAL, 0 );

      if ( fileHandle != INVALID_HANDLE_VALUE )
      {
        DWORD bytesRead;
        if ( ReadFile( fileHandle, targetBuffer, bytesToRead, &bytesRead, 0 ) )
        {
          result = true;
        }

        CloseHandle( fileHandle );
      }
    }

    return result;
  }

  bool write_file( char const* filePath, void const* data, u32 size, bse::file::WriteFlags flags )
  {
    bool result = true;
    wchar_t wideChars[BSE_PATH_MAX];
    utf8_to_wchar( filePath, wideChars, BSE_PATH_MAX );
    HANDLE fileHandle = nullptr;

    DWORD writeMode = (flags == bse::file::WriteFlags::AppendOrFail) ? FILE_APPEND_DATA : GENERIC_WRITE;
    DWORD createMode = (flags == bse::file::WriteFlags::OverwriteOrCreateNew) ? CREATE_ALWAYS : OPEN_EXISTING;

    fileHandle = CreateFileW( wideChars, writeMode, 0, 0, createMode, FILE_ATTRIBUTE_NORMAL, 0 );

    if ( fileHandle != INVALID_HANDLE_VALUE )
    {
      DWORD bytesWritten;
      u8 const* reader = (u8 const*) data;
      u32 sizeLeft = size;
      u32 const MAX_WRITE = (u32) KiloBytes( 4 );

      while ( sizeLeft > 0 )
      {
        if ( WriteFile( fileHandle, reader, min( MAX_WRITE, sizeLeft ), &bytesWritten, 0 ) )
        {
          assert( bytesWritten <= sizeLeft );
          sizeLeft -= bytesWritten;
          reader += bytesWritten;
        }
        else
        {
          result = false;
          BREAK;
          //  log_error( "[FILE]", "ERROR - couldn't write data to file: ", filePath );
          break;
        }
      }
    }
    else
    {
      result = false;
    }

    CloseHandle( fileHandle );

    return result;
  }

  bool create_directory( char const* directoryPath )
  {
    wchar_t wideChars[BSE_PATH_MAX];
    utf8_to_wchar( directoryPath, wideChars, BSE_PATH_MAX );

    return CreateDirectoryW( wideChars, NULL );
  }

  bool get_precompiled_asset( char const* name, void** out_data, u64* out_size )
  {
    s32 precompiledAssetCount = array_count( compiledasset::assetIndex );
    for ( s32 i = 0; i < precompiledAssetCount; ++i )
    {
      if ( bse::string_match( compiledasset::assetIndex[i].name, name ) )
      {
        if ( out_data )
        {
          *out_data = compiledasset::assetIndex[i].data;
        }
        if ( out_size )
        {
          *out_size = compiledasset::assetIndex[i].size;
        }

        return true;
      }
    }

    return false;
  }

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Threading /////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  bse::thread::ID thread_create( bse::thread::entry_fn* fn, void* parameter )
  {
    bse::thread::ID id;
    CloseHandle( CreateThread( 0, 0, fn, parameter, 0, (LPDWORD) &id ) );
    return id;
  }

  void* mutex_create()
  {
    HANDLE result = CreateMutexExW( 0, 0, 0, MUTEX_ALL_ACCESS );
    if ( !result )
    {
      windows_error();
    }
    return result;
  }

  void mutex_lock( void* handle, u32 waitMilliseconds )
  {
    DWORD const result = WaitForSingleObjectEx( handle, waitMilliseconds, FALSE );
    if ( result == WAIT_FAILED )
    {
      windows_error();
    }
  }

  void mutex_release( void* handle )
  {
    if ( !ReleaseMutex( handle ) )
    {
      windows_error();
    }
  }

  void* semaphore_create( s32 initialCount, s32 maxCount )
  {
    HANDLE result = CreateSemaphoreExW( 0, initialCount, maxCount, nullptr, 0, SEMAPHORE_ALL_ACCESS );
    if ( !result )
    {
      windows_error();
    }
    return result;
  }

  void wait_for_locking_object( void* handle, u32 waitMilliseconds )
  {
    DWORD const result = WaitForSingleObjectEx( handle, waitMilliseconds, FALSE );
    if ( result == WAIT_FAILED )
    {
      windows_error();
    }
  }

  s32 semaphore_release( void* handle, s32 releaseCount )
  {
    long out;
    if ( !ReleaseSemaphore( handle, releaseCount, &out ) )
    {
      windows_error();
    }
    return s32( out );
  }

  void close_handle( void* handle )
  {
    if ( !CloseHandle( handle ) )
    {
      windows_error();
    }
  }

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Vulkan ////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  VkSurfaceKHR vulkan_create_surface( VkInstance instance )
  {
    VkWin32SurfaceCreateInfoKHR createInfo = { VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR };
    createInfo.hinstance = GetModuleHandle( 0 );
    createInfo.hwnd = global::mainWindow.handle;

    VkSurfaceKHR surface = 0;
    BSE_VK_CHECK( vkCreateWin32SurfaceKHR( instance, &createInfo, 0, &surface ) );
    return surface;
  }

  bool vulkan_physical_device_supports_presentation( VkPhysicalDevice physicalDevice, u32 familyIndex )
  {
    return vkGetPhysicalDeviceWin32PresentationSupportKHR( physicalDevice, familyIndex );
  }


  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// System ////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  void shutdown()
  {
    global::running = false;
  }

  void* opengl_get_proc_address( char const* functionName )
  {
    void* p = (void*) wglGetProcAddress( functionName );
    if ( p == 0 ||
      (p == (void*) 0x1) || (p == (void*) 0x2) || (p == (void*) 0x3) ||
      (p == (void*) -1) )
    {
      p = (void*) GetProcAddress( win64::global::openglDll, functionName );
    }

    return p;
  }

  //TODO time here
  //GetSystemTimePreciseAsFileTime -> FILETIME //64bit value 100ns interval since 12:00 1/1/1601
  //GetSystemTime or FileTimeToSystemTime -> SYSTEMTIME //human readable? 

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Networking ////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  bse::Socket socket_create( bse::SocketType type )
  {
    return (bse::Socket) socket( AF_INET, (s32) type, 0 );
  }

  void socket_destroy( s32 socket )
  {
    closesocket( socket );
  }

  bool socket_connect( bse::Socket socket, bse::Ipv4Address const& ipv4Address )
  {
    sockaddr_in address;
    address.sin_port = htons( ipv4Address.port );
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = ipv4Address.address;

    return connect( socket, (const sockaddr*) &address, sizeof( address ) ) == 0;
  }

  bool socket_bind( bse::Socket socket, bse::Ipv4Address const& ipv4Address )
  {
    sockaddr_in address;
    address.sin_port = htons( ipv4Address.port );
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = ipv4Address.address;

    return bind( socket, (LPSOCKADDR) &address, sizeof( address ) ) != SOCKET_ERROR;
  }

  bool socket_listen( bse::Socket socket )
  {
    return listen( socket, SOMAXCONN ) != SOCKET_ERROR;
  }

  bool socket_accept( bse::Socket socket, bse::Socket* out_socket, bse::Ipv4Address* out_remoteAddress )
  {
    sockaddr_in remoteAddress;
    s32 remoteAddrSize = sizeof( remoteAddress );
    *out_socket = (s32) accept( socket, (sockaddr*) &remoteAddress, &remoteAddrSize );

    if ( *out_socket == SOCKET_ERROR )
    {
      out_remoteAddress->address = bse::IPv4_ADDRESS_INVALID;
      out_remoteAddress->port = bse::PORT_INVALID;
      return false;
    }
    else
    {
      out_remoteAddress->address = remoteAddress.sin_addr.s_addr;
      out_remoteAddress->port = ntohs( remoteAddress.sin_port );
      return true;
    }
  }

  bool socket_send( bse::Socket socket, char const* data, s32 size )
  {
    return send( socket, data, size, 0 ) != SOCKET_ERROR;
  }

  bool socket_receive( bse::Socket socket, char* receiveBuffer, s32 receiveBufferSize, s32* out_bytesReceived )
  {
    *out_bytesReceived = recv( socket, receiveBuffer, receiveBufferSize, 0 );
    return *out_bytesReceived != SOCKET_ERROR;
  }

  void socket_get_last_error_message( char* buffer, s32 bufferSize )
  {
    s32 bytesWritten = FormatMessageA( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 0, WSAGetLastError(), 0, buffer, bufferSize, 0 );
    buffer[min( bytesWritten, bufferSize )] = '\0';
  }

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

    ////////// Memory //////////////////////////////////////////////////////////////////////////////////
    global::platform.allocate_virtual_memory = &allocate_virtual_memory;
    global::platform.free_virtual_memory = &free_virtual_memory;
    global::platform.decommit_virtual_memory = &decommit_virtual_memory;

    ////////// File IO /////////////////////////////////////////////////////////////////////////////////
    global::platform.get_file_info = &get_file_info;
    global::platform.load_file_part = &load_file_part;
    global::platform.write_file = &write_file;
    global::platform.create_directory = &create_directory;
    global::platform.get_precompiled_asset = &get_precompiled_asset;

    ////////// Threading /////////////////////////////////////////////////////////////////////////////////
    global::platform.thread_create = &thread_create;
    global::platform.mutex_create = &mutex_create;
    global::platform.mutex_destroy = &close_handle;
    global::platform.mutex_release = &mutex_release;
    global::platform.semaphore_create = &semaphore_create;
    global::platform.semaphore_destroy = &close_handle;
    global::platform.semaphore_release = &semaphore_release;
    global::platform.wait_for_locking_object = &wait_for_locking_object;

    ////////// Vulkan ////////////////////////////////////////////////////////////////////////////////////
    global::platform.vulkan_create_surface = &vulkan_create_surface;
    global::platform.vulkan_physical_device_supports_presentation = &vulkan_physical_device_supports_presentation;

    ////////// Network /////////////////////////////////////////////////////////////////////////////////
    global::platform.socket_create = &socket_create;
    global::platform.socket_destroy = &socket_destroy;
    global::platform.socket_bind = &socket_bind;
    global::platform.socket_listen = &socket_listen;
    global::platform.socket_accept = &socket_accept;
    global::platform.socket_connect = &socket_connect;
    global::platform.socket_send = &socket_send;
    global::platform.socket_receive = &socket_receive;
    global::platform.socket_get_last_error_message = &socket_get_last_error_message;
    ////////// System //////////////////////////////////////////////////////////////////////////////////
    global::platform.shutdown = &shutdown;

    #if defined(BSE_RENDERER_OPENGL)
    global::platform.opengl_get_process_address = &opengl_get_proc_address;
    #endif
  }
};
