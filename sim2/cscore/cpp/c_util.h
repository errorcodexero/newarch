/*----------------------------------------------------------------------------*/
/* Copyright (c) 2016-2019 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#ifndef CSCORE_C_UTIL_H_
#define CSCORE_C_UTIL_H_

#include <cstdlib>
#include <cstring>

#include <wpi/MemAlloc.h>
#include <wpi/StringRef.h>

namespace cs {

inline char* ConvertToC(wpi::StringRef in) {
  char* out = static_cast<char*>(wpi::safe_malloc(in.size() + 1));
  std::memmove(out, in.data(), in.size());
  out[in.size()] = '\0';
  return out;
}

}  // namespace cs

#endif  // CSCORE_C_UTIL_H_
