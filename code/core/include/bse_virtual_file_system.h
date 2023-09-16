#pragma once

#include "bse_file.h"
#include "bse_container.h"

namespace bse
{
  struct VirtualFileSystem;

  [[nodiscard]] VirtualFileSystem* new_virtual_file_system( memory::Allocator* allocator );
  [[nodiscard]] VirtualFileSystem* new_virtual_file_system( memory::Allocator* allocator, Array<String> const& mountedPaths );
  void delete_virtual_file_system( VirtualFileSystem* );







  using MountPathID = s32;

  // void destroy_filesystem( FileSystem* );

  // //keep the return value if you need the mounted path for writing, for reading it doesn't matter
  // //path can be absolute or relative to the initial mounted path
  // MountPathID mount_path_to_filesystem( FileSystem*, char const* path );

  // char const* get_mounted_path_by_id( FileSystem* fs, MountPathID mountPathID );

  // bool unmount_path_from_filesystem( FileSystem*, char const* path ) { return false; } //TODO

  // [[nodiscard]]
  // File* load_file( FileSystem*, char const* path, MountPathID* out_mountPathID = nullptr );

  // //returns false if the file wasn't loaded by this file system
  // bool unload_file( FileSystem*, File* );

  // //returns true if the file exists anywhere in the filesystem
  // //out_mountPathID is the mounted path at which the file was found
  // bool find_file( FileSystem*, char const* path, MountPathID* out_mountPathID = nullptr );

  // //returns true if the file exists at the specified mount path
  // bool file_exists( FileSystem*, char const* path, MountPathID mountPathID = 0 );

  // bool write_file( FileSystem*, char const* path, void const* data, u32 size, MountPathID mountPathID = 0 );
  // bool append_file( FileSystem*, char const* path, void const* data, u32 size, MountPathID mountPathID = 0 );

};
