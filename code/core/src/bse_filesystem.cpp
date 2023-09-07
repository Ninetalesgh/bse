#include "bse_core.h"

namespace bse
{

  // struct FileSystem
  // {
  //   constexpr static s32 FS_LL_BLOCK_SIZE = 1024;
  //   constexpr static s32 MAX_LOADED_FILES = 256;
  //   char mountedPaths[FS_LL_BLOCK_SIZE];
  //   File loadedFiles[MAX_LOADED_FILES];
  //   char* writer;
  //   FileSystem* next;
  //   atomic32 fileSlotLock;
  //   s32 mountedPathsCount;
  //   s32 loadedFilesCount;
  // };
  struct FileSystem
  {

  };

  FileSystem* create_filesystem()
  {
    FileSystem* fs = (FileSystem*) allocate_to_zero( platform->default.allocator, sizeof( FileSystem ) );
    // fs->writer = fs->mountedPaths;
    // fs->mountedPathsCount = 0;
    // fs->loadedFilesCount = 0;
    // fs->next = nullptr;
    return fs;
  }


};
