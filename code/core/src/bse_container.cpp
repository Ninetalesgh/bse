#include "core/bse_core.h"



namespace bse
{
  Container::Container( memory::Allocator* _allocator )
  {
    allocator = _allocator ? _allocator : platform->default.allocator;
  }
};
