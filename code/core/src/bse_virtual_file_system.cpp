#include "core/bse_core.h"

//include precompiledassets here?




//  using get_file_info_fn = bool( char const* filePath, file::Info* out_fileInfo );
//  using load_file_part_fn = bool( char const* filePath, u64 readOffset, void* targetBuffer, u32 bufferSize );
//  using write_file_fn = bool( char const* filePath, void const* data, u32 size, file::WriteFlags );
//  using create_directory_fn = bool( char const* directoryPath );
//  using get_precompiled_asset_fn = bool( char const* name, void** out_data, u64* out_size );

namespace bse
{
  struct VirtualFileSystem
  {
    memory::Allocator* allocator;
    Array<char> mountedPaths;
  };

  bool is_absolute_path( char const* path )
  {
    return string_match( path + 1, ":/" ) || string_match( path + 1, ":\\" );
  }

  //s32 format_mount_path()

  VirtualFileSystem* new_virtual_file_system( memory::Allocator* allocator, Array<String> const& mountedPaths )
  {
    char stack[BSE_STACK_BUFFER_SIZE];
    s32 bytesWritten = 0;
    for ( String const& path : mountedPaths )
    {
      bytesWritten += string_format( stack + bytesWritten, BSE_STACK_BUFFER_SIZE - bytesWritten, &path );
    }

    VirtualFileSystem result;
    result.mountedPaths.reserve( bytesWritten );
    //    memcpy( result.mountedPaths.data(), stack, bytesWritten );

    String a = &result.mountedPaths[0];
    String b = &result.mountedPaths[1];
    return nullptr;
  }

  VirtualFileSystem* new_virtual_file_system( memory::Allocator* allocator )
  {
    return new_virtual_file_system( allocator, Array<String>() );
  }

  void delete_virtual_file_system( VirtualFileSystem* vfs )
  {
    vfs->~VirtualFileSystem();
    free( vfs->allocator, vfs, sizeof( VirtualFileSystem ) );
  }

};
