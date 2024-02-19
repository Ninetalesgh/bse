#pragma once

#include "bse_common.h"
#include "bse_container.h"

namespace bse
{
  namespace file
  {
    struct Info
    {
      u64 size;
    };

    enum class WriteFlags : u32
    {
      //Overwrite an existing file or create a new file if none exists
      OverwriteOrCreateNew = 0x0,
      //Append an existing file or fail if none exists
      AppendOrFail = 0x1,
      //Overwrite an existing file or fail if none exists
      OverwriteOrFail = 0x2,
    };

    [[nodiscard]]
    Buffer load( char const* filePath );
    void unload( Buffer& buffer );
  };
};