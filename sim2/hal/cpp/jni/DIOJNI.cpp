/*----------------------------------------------------------------------------*/
/* Copyright (c) 2016-2019 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include <jni.h>

#include <cassert>

#include "HALUtil.h"
#include "edu_wpi_first_hal_DIOJNI.h"
#include "hal/DIO.h"
#include "hal/PWM.h"
#include "hal/Ports.h"
#include "hal/handles/HandlesInternal.h"

using namespace frc;

extern "C" {

/*
 * Class:     edu_wpi_first_hal_DIOJNI
 * Method:    initializeDIOPort
 * Signature: (IZ)I
 */
JNIEXPORT jint JNICALL
Java_edu_wpi_first_hal_DIOJNI_initializeDIOPort
  (JNIEnv* env, jclass, jint id, jboolean input)
{
  int32_t status = 0;
  auto dio = HAL_InitializeDIOPort((HAL_PortHandle)id,
                                   static_cast<uint8_t>(input), &status);
  CheckStatusRange(env, status, 0, HAL_GetNumDigitalChannels(),
                   hal::getPortHandleChannel((HAL_PortHandle)id));
  return (jint)dio;
}

/*
 * Class:     edu_wpi_first_hal_DIOJNI
 * Method:    checkDIOChannel
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL
Java_edu_wpi_first_hal_DIOJNI_checkDIOChannel
  (JNIEnv* env, jclass, jint channel)
{
  return HAL_CheckDIOChannel(channel);
}

/*
 * Class:     edu_wpi_first_hal_DIOJNI
 * Method:    freeDIOPort
 * Signature: (I)V
 */
JNIEXPORT void JNICALL
Java_edu_wpi_first_hal_DIOJNI_freeDIOPort
  (JNIEnv* env, jclass, jint id)
{
  HAL_FreeDIOPort((HAL_DigitalHandle)id);
}

/*
 * Class:     edu_wpi_first_hal_DIOJNI
 * Method:    setDIOSimDevice
 * Signature: (II)V
 */
JNIEXPORT void JNICALL
Java_edu_wpi_first_hal_DIOJNI_setDIOSimDevice
  (JNIEnv* env, jclass, jint handle, jint device)
{
  HAL_SetDIOSimDevice((HAL_DigitalHandle)handle, (HAL_SimDeviceHandle)device);
}

/*
 * Class:     edu_wpi_first_hal_DIOJNI
 * Method:    setDIO
 * Signature: (IS)V
 */
JNIEXPORT void JNICALL
Java_edu_wpi_first_hal_DIOJNI_setDIO
  (JNIEnv* env, jclass, jint id, jshort value)
{
  int32_t status = 0;
  HAL_SetDIO((HAL_DigitalHandle)id, value, &status);
  CheckStatus(env, status);
}

/*
 * Class:     edu_wpi_first_hal_DIOJNI
 * Method:    setDIODirection
 * Signature: (IZ)V
 */
JNIEXPORT void JNICALL
Java_edu_wpi_first_hal_DIOJNI_setDIODirection
  (JNIEnv* env, jclass, jint id, jboolean input)
{
  int32_t status = 0;
  HAL_SetDIODirection((HAL_DigitalHandle)id, input, &status);
  CheckStatus(env, status);
}

/*
 * Class:     edu_wpi_first_hal_DIOJNI
 * Method:    getDIO
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL
Java_edu_wpi_first_hal_DIOJNI_getDIO
  (JNIEnv* env, jclass, jint id)
{
  int32_t status = 0;
  jboolean returnValue = HAL_GetDIO((HAL_DigitalHandle)id, &status);
  CheckStatus(env, status);
  return returnValue;
}

/*
 * Class:     edu_wpi_first_hal_DIOJNI
 * Method:    getDIODirection
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL
Java_edu_wpi_first_hal_DIOJNI_getDIODirection
  (JNIEnv* env, jclass, jint id)
{
  int32_t status = 0;
  jboolean returnValue = HAL_GetDIODirection((HAL_DigitalHandle)id, &status);
  CheckStatus(env, status);
  return returnValue;
}

/*
 * Class:     edu_wpi_first_hal_DIOJNI
 * Method:    pulse
 * Signature: (ID)V
 */
JNIEXPORT void JNICALL
Java_edu_wpi_first_hal_DIOJNI_pulse
  (JNIEnv* env, jclass, jint id, jdouble value)
{
  int32_t status = 0;
  HAL_Pulse((HAL_DigitalHandle)id, value, &status);
  CheckStatus(env, status);
}

/*
 * Class:     edu_wpi_first_hal_DIOJNI
 * Method:    isPulsing
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL
Java_edu_wpi_first_hal_DIOJNI_isPulsing
  (JNIEnv* env, jclass, jint id)
{
  int32_t status = 0;
  jboolean returnValue = HAL_IsPulsing((HAL_DigitalHandle)id, &status);
  CheckStatus(env, status);
  return returnValue;
}

/*
 * Class:     edu_wpi_first_hal_DIOJNI
 * Method:    isAnyPulsing
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL
Java_edu_wpi_first_hal_DIOJNI_isAnyPulsing
  (JNIEnv* env, jclass)
{
  int32_t status = 0;
  jboolean returnValue = HAL_IsAnyPulsing(&status);
  CheckStatus(env, status);
  return returnValue;
}

/*
 * Class:     edu_wpi_first_hal_DIOJNI
 * Method:    getLoopTiming
 * Signature: ()S
 */
JNIEXPORT jshort JNICALL
Java_edu_wpi_first_hal_DIOJNI_getLoopTiming
  (JNIEnv* env, jclass)
{
  int32_t status = 0;
  jshort returnValue = HAL_GetPWMLoopTiming(&status);
  CheckStatus(env, status);
  return returnValue;
}

/*
 * Class:     edu_wpi_first_hal_DIOJNI
 * Method:    allocateDigitalPWM
 * Signature: ()I
 */
JNIEXPORT jint JNICALL
Java_edu_wpi_first_hal_DIOJNI_allocateDigitalPWM
  (JNIEnv* env, jclass)
{
  int32_t status = 0;
  auto pwm = HAL_AllocateDigitalPWM(&status);
  CheckStatus(env, status);
  return (jint)pwm;
}

/*
 * Class:     edu_wpi_first_hal_DIOJNI
 * Method:    freeDigitalPWM
 * Signature: (I)V
 */
JNIEXPORT void JNICALL
Java_edu_wpi_first_hal_DIOJNI_freeDigitalPWM
  (JNIEnv* env, jclass, jint id)
{
  int32_t status = 0;
  HAL_FreeDigitalPWM((HAL_DigitalPWMHandle)id, &status);
  CheckStatus(env, status);
}

/*
 * Class:     edu_wpi_first_hal_DIOJNI
 * Method:    setDigitalPWMRate
 * Signature: (D)V
 */
JNIEXPORT void JNICALL
Java_edu_wpi_first_hal_DIOJNI_setDigitalPWMRate
  (JNIEnv* env, jclass, jdouble value)
{
  int32_t status = 0;
  HAL_SetDigitalPWMRate(value, &status);
  CheckStatus(env, status);
}

/*
 * Class:     edu_wpi_first_hal_DIOJNI
 * Method:    setDigitalPWMDutyCycle
 * Signature: (ID)V
 */
JNIEXPORT void JNICALL
Java_edu_wpi_first_hal_DIOJNI_setDigitalPWMDutyCycle
  (JNIEnv* env, jclass, jint id, jdouble value)
{
  int32_t status = 0;
  HAL_SetDigitalPWMDutyCycle((HAL_DigitalPWMHandle)id, value, &status);
  CheckStatus(env, status);
}

/*
 * Class:     edu_wpi_first_hal_DIOJNI
 * Method:    setDigitalPWMOutputChannel
 * Signature: (II)V
 */
JNIEXPORT void JNICALL
Java_edu_wpi_first_hal_DIOJNI_setDigitalPWMOutputChannel
  (JNIEnv* env, jclass, jint id, jint value)
{
  int32_t status = 0;
  HAL_SetDigitalPWMOutputChannel((HAL_DigitalPWMHandle)id,
                                 static_cast<uint32_t>(value), &status);
  CheckStatus(env, status);
}

}  // extern "C"
