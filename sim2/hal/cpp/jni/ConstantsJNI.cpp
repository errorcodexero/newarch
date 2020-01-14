/*----------------------------------------------------------------------------*/
/* Copyright (c) 2016-2019 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include <jni.h>

#include <cassert>

#include "HALUtil.h"
#include "edu_wpi_first_hal_ConstantsJNI.h"
#include "hal/Constants.h"

using namespace frc;

extern "C" {
/*
 * Class:     edu_wpi_first_hal_ConstantsJNI
 * Method:    getSystemClockTicksPerMicrosecond
 * Signature: ()I
 */
JNIEXPORT jint JNICALL
Java_edu_wpi_first_hal_ConstantsJNI_getSystemClockTicksPerMicrosecond
  (JNIEnv* env, jclass)
{
  jint value = HAL_GetSystemClockTicksPerMicrosecond();
  return value;
}
}  // extern "C"
