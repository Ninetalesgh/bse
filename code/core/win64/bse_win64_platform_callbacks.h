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

  void* allocate_virtual_memory( void* address, s64 size );
  void free_virtual_memory( void* allocationToFree );
  void decommit_virtual_memory( void* committedMemory, s64 size );

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// File IO ///////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  bool get_file_info( char const* filePath, bse::file::Info* out_fileInfo );
  bool load_file_part( char const* filePath, u64 readOffset, void* targetBuffer, u32 bufferSize );
  bool write_file( char const* filePath, void const* data, u32 size, bse::file::WriteFlags flags );
  bool create_directory( char const* directoryPath );
  bool get_precompiled_asset( char const* name, void** out_data, u64* out_size );

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
  void register_platform_callbacks()
  {
    ////////// Debug /////////////////////////////////////////////////////////////////////////////////////
    global::platform.debug_log = &debug_log;

    ////////// Memory ////////////////////////////////////////////////////////////////////////////////////
    global::platform.allocate_virtual_memory = &allocate_virtual_memory;
    global::platform.free_virtual_memory = &free_virtual_memory;
    global::platform.decommit_virtual_memory = &decommit_virtual_memory;

    ////////// File IO ///////////////////////////////////////////////////////////////////////////////////
    global::platform.get_file_info = &get_file_info;
    global::platform.load_file_part = &load_file_part;
    global::platform.write_file = &write_file;
    global::platform.create_directory = &create_directory;
    global::platform.get_precompiled_asset = &get_precompiled_asset;

    ////////// System ////////////////////////////////////////////////////////////////////////////////////
    global::platform.shutdown = &shutdown;
    global::platform.opengl_get_process_address = &opengl_get_proc_address;
  }

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

    // char const* prefix = parameters.severity == bse::debug::LogSeverity::INFO ? infoPrefix :
     //  parameters.severity == bse::debug::LogSeverity::WARNING ? warningPrefix : errorPrefix;

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

  void* allocate_virtual_memory( void* address, s64 size )
  {
    debug_log_info( "Reserving ", size, " Bytes of virtual Memory." );
    // return malloc( size );
    void* result = VirtualAlloc( address, (s64) size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE );
    if ( !result )
    {
      u32 errormsg = GetLastError();
      log_warning( errormsg );
      BREAK;
    }
    return result;
  }

  void decommit_virtual_memory( void* committedMemory, s64 size )
  {
    if ( VirtualFree( committedMemory, size, MEM_DECOMMIT ) )
    {
      u32 errormsg = GetLastError();
      log_warning( errormsg );
      BREAK;
    }
  }

  void free_virtual_memory( void* allocationToFree )
  {
    //free( allocationToFree );
    if ( !VirtualFree( allocationToFree, 0, MEM_RELEASE ) )
    {
      u32 errormsg = GetLastError();
      log_error( errormsg );
      BREAK;
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

  bool load_file_part( char const* filePath, u64 readOffset, void* targetBuffer, u32 bufferSize )
  {
    if ( readOffset )
    {
      BREAK; //TODO
    }

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
        if ( ReadFile( fileHandle, targetBuffer, bufferSize, &bytesRead, 0 ) )
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
  ////////// System ////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  void shutdown()
  {
    global::running = false;
  }

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Networking ////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////



  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Audio /////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

};
