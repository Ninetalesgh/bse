#pragma once

#include "bse_math.h"
#include "bse_filesystem.h"
#include "bse_debug.h"
#include "bse_thread.h"
#include "bse_string_format.h"
#include "bse_color.h"
//#include "bse_container.h"
#include "bse_common.h"

#if !defined(BSE_BUILD_SKIP_INPUT)
# include "bse_input.h"
#endif

#if !defined(BSE_BUILD_SKIP_GRAPHICS)
# include "bse_image.h"
# include "bse_opengl.h"
#endif

#include "bse_unsorted_definitions.h"