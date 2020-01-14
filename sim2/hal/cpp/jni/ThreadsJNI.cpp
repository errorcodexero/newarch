/*----------------------------------------------------------------------------*/
/* Copyright (c) 2016-2019 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include <jni.h>

#include <cassert>

#include "HALUtil.h"
#include "edu_wpi_first_hal_ThreadsJNI.h"
#include "hal/Threads.h"

using namespace frc;

extern "C" {
/*
 * Class:     edu_wpi_first_hal_ThreadsJNI
 * Method:    getCurrentThreadPriority
 * Signature: ()I
 */
JNIEXPORT jint JNICALL
Java_edu_wpi_first_hal_ThreadsJNI_getCurrentThreadPriority
  (JNIEnv* env, jclass)
{
  int32_t status = 0;
  HAL_Bool isRT = false;
  auto ret = HAL_GetCurrentThreadPriority(&isRT, &status);
  CheckStatus(env, status);
  return (jint)ret;
}

/*
 * Class:     edu_wpi_first_hal_ThreadsJNI
 * Method:    getCurrentThreadIsRealTime
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL
Java_edu_wpi_first_hal_ThreadsJNI_getCurrentThreadIsRealTime
  (JNIEnv* env, jclass)
{
  int32_t status = 0;
  HAL_Bool isRT = false;
  HAL_GetCurrentThreadPriority(&isRT, &status);
  CheckStatus(env, status);
  return (jboolean)isRT;
}

/*
 * Class:     edu_wpi_first_hal_ThreadsJNI
 * Method:    setCurrentThreadPriority
 * Signature: (ZI)Z
 */
JNIEXPORT jboolean JNICALL
Java_edu_wpi_first_hal_ThreadsJNI_setCurrentThreadPriority
  (JNIEnv* env, jclass, jboolean realTime, jint priority)
{
  int32_t status = 0;
  auto ret = HAL_SetCurrentThreadPriority(
      (HAL_Bool)realTime, static_cast<int32_t>(priority), &status);
  CheckStatus(env, status);
  return (jboolean)ret;
}

}  // extern "C"
