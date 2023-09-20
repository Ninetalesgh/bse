#include "core/bse_core.h"

//include precompiledassets here?


namespace bse
{
  struct File
  {
    s64 size;
  };



  struct VirtualFileSystem
  {
    Array<String> mountedPaths;
    Array<File*> loadedFiles;
  };

  // [[nodiscard]] VirtualFileSystem* new_virtual_file_system( memory::Allocator* allocator, Array<String> const& mountedPaths );
  // [[nodiscard]] VirtualFileSystem* new_virtual_file_system( memory::Allocator* allocator );
  // void delete_virtual_file_system( VirtualFileSystem* );




};



// //  using get_file_info_fn = bool( char const* filePath, file::Info* out_fileInfo );
// //  using load_file_part_fn = bool( char const* filePath, u64 readOffset, void* targetBuffer, u32 bufferSize );
// //  using write_file_fn = bool( char const* filePath, void const* data, u32 size, file::WriteFlags );
// //  using create_directory_fn = bool( char const* directoryPath );
// //  using get_precompiled_asset_fn = bool( char const* name, void** out_data, u64* out_size );

// namespace bse
// {

//   bool is_absolute_path( char const* path )
//   {
//     return string_match( path + 1, ":/" ) || string_match( path + 1, ":\\" );
//   }

//   s32 path_format_replace_slashes( char* destination, s32 capacity, char const* path )
//   {
//     char const* reader = path;
//     char* writer = destination;
//     char* end = writer + capacity - 1;
//     while ( *reader && writer < end )
//     {
//       if ( *reader != '\\' )
//       {
//         *writer = *reader;
//       }
//       else
//       {
//         *writer = '/';
//       }

//       ++writer;
//       ++reader;
//     }

//     return s32( writer - destination );
//   }

//   void path_remove_node( char* path )
//   {
//     char* reader = path + bs::string_length( path ) - 1;
//     *reader = '\0';
//     while ( *reader != '/' )
//     {
//       *reader-- = '\0';
//     }
//   }

//   bool mount_absolute_path_to_filesystem( VirtualFileSystem* vfs, char const* path )
//   {
//     char stack[BSE_STACK_BUFFER_SMALL] = {};

//     s32 length = path_format_replace_slashes( stack, BSE_STACK_BUFFER_SMALL, path );
//     if ( string_contains( path, ".exe" ) == stack + length - 4 )
//     {
//       path_remove_node( stack );
//     }
//     else if ( stack[length - 1] != '/' )
//     {
//       stack[length] = '/';
//     }

//     vfs->mountedPaths.push( stack );
//     return true;
//   }

//   bool mount_relative_path_to_filesystem( VirtualFileSystem* vfs, char const* path )
//   {
//     if ( !vfs->mountedPaths.count )
//     {
//       //can't mount relative path, relative to what?
//       BREAK;
//       return false;
//     }
//     char stack[BSE_STACK_BUFFER_SMALL] = {};

//     string_format( stack, BSE_STACK_BUFFER_SMALL, vfs->mountedPaths.data );
//     char const* reader = path;

//     while ( *reader != '\0' )
//     {
//       if ( *reader == '/' )
//       {
//         ++reader;
//       }
//       else if ( string_match( reader, "../" ) )
//       {
//         path_remove_node( stack );
//         reader += 3;
//       }
//       else
//       {
//         char* writer = stack + string_length( stack );
//         while ( *reader != '/' && *reader != '\0' )
//         {
//           *writer++ = *reader++;
//         }
//         *writer++ = '/';
//       }
//     }

//     return mount_absolute_path_to_filesystem( vfs, stack );
//   }

//   MountPathID mount_path_to_filesystem( VirtualFileSystem* vfs, char const* path )
//   {
//     bool succeeded = false;
//     if ( is_absolute_path( path ) )
//     {
//       succeeded = mount_absolute_path_to_filesystem( vfs, path );
//     }
//     else
//     {
//       succeeded = mount_relative_path_to_filesystem( vfs, path );
//     }

//     return succeeded ? vfs->nextMountPathID++ : -1;
//   }


//   bool find_first_valid_file_path( VirtualFileSystem* vfs, char* out_validPath, s32 capacity, char const* relativePath, MountPathID* out_mountPathID )
//   {
//     if ( *relativePath == '/' ) ++relativePath;

//     for ( s32 i = 0; i < fs->mountedPathsCount; ++i )
//     {
//       bs::string_format( out_validPath, capacity, get_mounted_path_by_id( fs, i ), relativePath );

//       if ( bsp::platform->get_file_info( out_validPath, nullptr ) )
//       {
//         if ( out_mountPathID )
//         {
//           *out_mountPathID = i;
//         }
//         return true;
//       }
//     }

//     return false;
//   }


//   File* load_file( VirtualFileSystem* vfs, char const* path, MountPathID* out_mountPathID )
//   {
//     if ( !path )
//     {
//       return nullptr;
//     }

//     char actualPath[BSE_STACK_BUFFER_SMALL];
//     if ( !find_first_valid_file_path( fs, actualPath, BSE_STACK_BUFFER_SMALL, path, out_mountPathID ) )
//     {
//       void* pcaData;
//       u64 pcaSize;
//       char const* filename = bs::string_find_last( path, '/' );
//       filename = filename ? filename : path;
//       if ( bsp::platform->get_precompiled_asset( filename, &pcaData, &pcaSize ) )
//       {
//         LOCK_SCOPE( fs->fileSlotLock );
//         File* file = get_next_free_file_slot( fs );
//         file->data = pcaData;
//         file->size = pcaSize;
//         file->type = FileType::PCA;
//         return file;
//       }

//       return nullptr;
//     }

//     u64 size = 0;
//     if ( !bsp::platform->get_file_info( actualPath, &size ) )
//     {
//       return nullptr;
//     }

//     void* data = bs::allocate( bsp::platform->default.generalAllocator, size );
//     if ( data )
//     {
//       if ( bsp::platform->load_file_part( actualPath, 0, data, (u32) size ) )
//       {
//         LOCK_SCOPE( fs->fileSlotLock );
//         File* file = get_next_free_file_slot( fs );
//         file->data = data;
//         file->size = size;
//         file->type = FileType::DISK;
//         return file;
//       }
//       else
//       {
//         bs::free( bsp::platform->default.generalAllocator, data );
//       }
//     }

//     return nullptr;
//   }



//   bool unload_file( VirtualFileSystem* vfs, File* file )
//   {
//     File* begin = fs->loadedFiles;
//     File* end = fs->loadedFiles + FileSystem::MAX_LOADED_FILES;

//     if ( file > begin && file < end )
//     {
//       if ( file->data && file->type != FileType::PCA )
//       {
//         bs::free( bsp::platform->default.generalAllocator, file->data );
//       }

//       interlocked_decrement( (s32 volatile*) &fs->loadedFilesCount );
//       file->type = FileType::INVALID;

//       return true;
//     }
//     else
//     {
//       //it's a precompiled asset or a file that doesn't belong to this filesystem
//       //TODO if fs is linked list
//     }

//     return false;
//   }

//   bool find_file( VirtualFileSystem* vfs, char const* path, MountPathID* out_mountPathID )
//   {
//     char actualPath[1024];

//     if ( !find_first_valid_file_path( fs, actualPath, 1024, path, out_mountPathID ) )
//     {
//       return false;
//     }

//     return true;
//   }

//   bool file_exists( VirtualFileSystem* vfs, char const* path, MountPathID mountPathID )
//   {
//     char actualPath[1024];
//     if ( *path == '/' ) ++path;
//     bs::string_format( actualPath, 1024, get_mounted_path_by_id( fs, mountPathID ), path );
//     return bsp::platform->get_file_info( actualPath, nullptr );
//   }

//   bool write_file( VirtualFileSystem* vfs, char const* path, void const* data, u32 size, MountPathID mountPathID )
//   {
//     char actualPath[1024];

//     if ( *path == '/' ) ++path;
//     bs::string_format( actualPath, 1024, get_mounted_path_by_id( fs, mountPathID ), path );

//     bsp::WriteFileFlags flags = bsp::WriteFileFlags::OVERWRITE_OR_CREATE_NEW;

//     return bsp::platform->write_file( actualPath, data, size, flags );
//   }

//   bool append_file( VirtualFileSystem* vfs, char const* path, void const* data, u32 size, MountPathID mountPathID )
//   {
//     char actualPath[1024];

//     if ( *path == '/' ) ++path;
//     bs::string_format( actualPath, 1024, get_mounted_path_by_id( fs, mountPathID ), path );

//     bsp::WriteFileFlags flags = bsp::WriteFileFlags::APPEND_OR_FAIL;
//     return bsp::platform->write_file( actualPath, data, size, flags );
//   }


























//   // bool mount_absolute_path_to_vfs( VirtualFileSystem* vfs, char const* path )
//   // {

//   // }

//   // bool mount_relative_path_to_vfs( VirtualFileSystem* vfs, char const* path )
//   // {

//   // }

//   // MountPathID mount_path_to_filesystem( VirtualFileSystem* vfs, char const* path )
//   // {
//   //   bool succeeded = false;
//   //   if ( is_absolute_path( path ) )
//   //   {
//   //     succeeded = mount_absolute_path_to_filesystem( fs, path );
//   //   }
//   //   else
//   //   {
//   //     succeeded = mount_relative_path_to_filesystem( fs, path );
//   //   }

//   //   return succeeded ? 1 : -1;
//   // }

//   // VirtualFileSystem* new_virtual_file_system( memory::Allocator* allocator, Array<String> const& mountedPaths )
//   // {
//   //   char stack[BSE_STACK_BUFFER_SIZE];
//   //   s32 bytesWritten = 0;
//   //   for ( String const& path : mountedPaths )
//   //   {
//   //     mount_path_to_filesystem( vfs, path.cstr() );

//   //     bytesWritten += string_format( stack + bytesWritten, BSE_STACK_BUFFER_SIZE - bytesWritten, &path );
//   //   }

//   //   VirtualFileSystem result;
//   //   result.mountedPaths.reserve( bytesWritten );
//   //   memcpy( result.mountedPaths.data, stack, bytesWritten );

//   //   String a = &result.mountedPaths[0];
//   //   String b = &result.mountedPaths[1];
//   //   return nullptr;
//   // }

//   // VirtualFileSystem* new_virtual_file_system( memory::Allocator* allocator )
//   // {
//   //   return new_virtual_file_system( allocator, Array<String>() );
//   // }

//   // void delete_virtual_file_system( VirtualFileSystem* vfs )
//   // {
//   //   vfs->~VirtualFileSystem();
//   //   free( vfs->allocator, vfs, sizeof( VirtualFileSystem ) );
//   // }

// };
