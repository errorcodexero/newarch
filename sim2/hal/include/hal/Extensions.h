/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2019 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include "hal/Types.h"

/**
 * @defgroup hal_extensions Simulator Extensions
 * @ingroup hal_capi
 * HAL Simulator Extensions.  These are libraries that provide additional
 * simulator functionality, such as a Gazebo interface, or a more light weight
 * simulation.
 *
 * An extension must expose the HALSIM_InitExtension entry point which is
 * invoked after the library is loaded.
 *
 * The entry point is expected to return < 0 for errors that should stop
 * the HAL completely, 0 for success, and > 0 for a non fatal error.
 * @{
 */
typedef int halsim_extension_init_func_t(void);

extern "C" {
/**
 * Loads a single extension from a direct path.
 *
 * Expected to be called internally, not by users.
 *
 * @param library the library path
 * @return        the succes state of the initialization
 */
int HAL_LoadOneExtension(const char* library);

/**
 * Loads any extra halsim libraries provided in the HALSIM_EXTENSIONS
 * environment variable.
 *
 * @return        the succes state of the initialization
 */
int HAL_LoadExtensions(void);

/**
 * Enables or disables the message saying no HAL extensions were found.
 *
 * Some apps don't care, and the message create clutter. For general team code,
 * we want it.
 *
 * This must be called before HAL_Initialize is called.
 *
 * This defaults to true.
 *
 * @param showMessage true to show message, false to not.
 */
void HAL_SetShowExtensionsNotFoundMessages(HAL_Bool showMessage);
}  // extern "C"
/** @} */
