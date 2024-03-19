#pragma once
#include "core/bse_core.h"
#include <vulkan/vulkan.h>

#define BSE_VK_CHECK(term) do{VkResult bseVkCheckResult = term; if (bseVkCheckResult != VK_SUCCESS) { log_error(#term, " failed with error code: ", s32(bseVkCheckResult));} } while (0)
