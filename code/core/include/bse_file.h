#pragma once

#include "bse_common.h"

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
      OVERWRITE_OR_CREATE_NEW = 0x0,
      APPEND_OR_FAIL = 0x1,
      OVERWRITE_OR_FAIL = 0x2,
    };
  };
};