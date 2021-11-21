#pragma once

#ifdef __GNUG__
  #define GET_CURRENT_FUNCTION_NAME() __func__
#else
  #define GET_CURRENT_FUNCTION_NAME() ""
#endif