/*----------------------------------------------------------------------------*/
/* Copyright (c) 2008-2019 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "wpi/Demangle.h"

#include <windows.h>  // NOLINT(build/include_order)

#include <dbghelp.h>

#include "wpi/mutex.h"

#pragma comment(lib, "Dbghelp.lib")

namespace wpi {

std::string Demangle(char const* mangledSymbol) {
  static wpi::mutex m;
  std::scoped_lock lock(m);
  char buffer[256];
  DWORD sz = UnDecorateSymbolName(mangledSymbol, buffer, sizeof(buffer),
                                  UNDNAME_COMPLETE);
  if (sz == 0) return mangledSymbol;
  return std::string(buffer, sz);
}

}  // namespace wpi
