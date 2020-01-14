/*----------------------------------------------------------------------------*/
/* Copyright (c) 2016-2019 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include <jni.h>

#include <cassert>

#include <wpi/jni_util.h>

#include "HALUtil.h"
#include "edu_wpi_first_hal_SPIJNI.h"
#include "hal/SPI.h"

using namespace frc;
using namespace wpi::java;

extern "C" {

/*
 * Class:     edu_wpi_first_hal_SPIJNI
 * Method:    spiInitialize
 * Signature: (I)V
 */
JNIEXPORT void JNICALL
Java_edu_wpi_first_hal_SPIJNI_spiInitialize
  (JNIEnv* env, jclass, jint port)
{
  int32_t status = 0;
  HAL_InitializeSPI(static_cast<HAL_SPIPort>(port), &status);
  CheckStatusForceThrow(env, status);
}

/*
 * Class:     edu_wpi_first_hal_SPIJNI
 * Method:    spiTransaction
 * Signature: (ILjava/lang/Object;Ljava/lang/Object;B)I
 */
JNIEXPORT jint JNICALL
Java_edu_wpi_first_hal_SPIJNI_spiTransaction
  (JNIEnv* env, jclass, jint port, jobject dataToSend, jobject dataReceived,
   jbyte size)
{
  uint8_t* dataToSendPtr = nullptr;
  if (dataToSend != 0) {
    dataToSendPtr =
        reinterpret_cast<uint8_t*>(env->GetDirectBufferAddress(dataToSend));
  }
  uint8_t* dataReceivedPtr =
      reinterpret_cast<uint8_t*>(env->GetDirectBufferAddress(dataReceived));
  jint retVal = HAL_TransactionSPI(static_cast<HAL_SPIPort>(port),
                                   dataToSendPtr, dataReceivedPtr, size);
  return retVal;
}

/*
 * Class:     edu_wpi_first_hal_SPIJNI
 * Method:    spiTransactionB
 * Signature: (I[B[BB)I
 */
JNIEXPORT jint JNICALL
Java_edu_wpi_first_hal_SPIJNI_spiTransactionB
  (JNIEnv* env, jclass, jint port, jbyteArray dataToSend,
   jbyteArray dataReceived, jbyte size)
{
  wpi::SmallVector<uint8_t, 128> recvBuf;
  recvBuf.resize(size);
  jint retVal =
      HAL_TransactionSPI(static_cast<HAL_SPIPort>(port),
                         reinterpret_cast<const uint8_t*>(
                             JByteArrayRef(env, dataToSend).array().data()),
                         recvBuf.data(), size);
  env->SetByteArrayRegion(dataReceived, 0, size,
                          reinterpret_cast<const jbyte*>(recvBuf.data()));
  return retVal;
}

/*
 * Class:     edu_wpi_first_hal_SPIJNI
 * Method:    spiWrite
 * Signature: (ILjava/lang/Object;B)I
 */
JNIEXPORT jint JNICALL
Java_edu_wpi_first_hal_SPIJNI_spiWrite
  (JNIEnv* env, jclass, jint port, jobject dataToSend, jbyte size)
{
  uint8_t* dataToSendPtr = nullptr;
  if (dataToSend != 0) {
    dataToSendPtr =
        reinterpret_cast<uint8_t*>(env->GetDirectBufferAddress(dataToSend));
  }
  jint retVal =
      HAL_WriteSPI(static_cast<HAL_SPIPort>(port), dataToSendPtr, size);
  return retVal;
}

/*
 * Class:     edu_wpi_first_hal_SPIJNI
 * Method:    spiWriteB
 * Signature: (I[BB)I
 */
JNIEXPORT jint JNICALL
Java_edu_wpi_first_hal_SPIJNI_spiWriteB
  (JNIEnv* env, jclass, jint port, jbyteArray dataToSend, jbyte size)
{
  jint retVal = HAL_WriteSPI(static_cast<HAL_SPIPort>(port),
                             reinterpret_cast<const uint8_t*>(
                                 JByteArrayRef(env, dataToSend).array().data()),
                             size);
  return retVal;
}

/*
 * Class:     edu_wpi_first_hal_SPIJNI
 * Method:    spiRead
 * Signature: (IZLjava/lang/Object;B)I
 */
JNIEXPORT jint JNICALL
Java_edu_wpi_first_hal_SPIJNI_spiRead
  (JNIEnv* env, jclass, jint port, jboolean initiate, jobject dataReceived,
   jbyte size)
{
  uint8_t* dataReceivedPtr =
      reinterpret_cast<uint8_t*>(env->GetDirectBufferAddress(dataReceived));
  jint retVal;
  if (initiate) {
    wpi::SmallVector<uint8_t, 128> sendBuf;
    sendBuf.resize(size);
    retVal = HAL_TransactionSPI(static_cast<HAL_SPIPort>(port), sendBuf.data(),
                                dataReceivedPtr, size);
  } else {
    retVal = HAL_ReadSPI(static_cast<HAL_SPIPort>(port),
                         reinterpret_cast<uint8_t*>(dataReceivedPtr), size);
  }
  return retVal;
}

/*
 * Class:     edu_wpi_first_hal_SPIJNI
 * Method:    spiReadB
 * Signature: (IZ[BB)I
 */
JNIEXPORT jint JNICALL
Java_edu_wpi_first_hal_SPIJNI_spiReadB
  (JNIEnv* env, jclass, jint port, jboolean initiate, jbyteArray dataReceived,
   jbyte size)
{
  jint retVal;
  wpi::SmallVector<uint8_t, 128> recvBuf;
  recvBuf.resize(size);
  if (initiate) {
    wpi::SmallVector<uint8_t, 128> sendBuf;
    sendBuf.resize(size);
    retVal = HAL_TransactionSPI(static_cast<HAL_SPIPort>(port), sendBuf.data(),
                                recvBuf.data(), size);
  } else {
    retVal = HAL_ReadSPI(static_cast<HAL_SPIPort>(port), recvBuf.data(), size);
  }
  env->SetByteArrayRegion(dataReceived, 0, size,
                          reinterpret_cast<const jbyte*>(recvBuf.data()));
  return retVal;
}

/*
 * Class:     edu_wpi_first_hal_SPIJNI
 * Method:    spiClose
 * Signature: (I)V
 */
JNIEXPORT void JNICALL
Java_edu_wpi_first_hal_SPIJNI_spiClose
  (JNIEnv*, jclass, jint port)
{
  HAL_CloseSPI(static_cast<HAL_SPIPort>(port));
}

/*
 * Class:     edu_wpi_first_hal_SPIJNI
 * Method:    spiSetSpeed
 * Signature: (II)V
 */
JNIEXPORT void JNICALL
Java_edu_wpi_first_hal_SPIJNI_spiSetSpeed
  (JNIEnv*, jclass, jint port, jint speed)
{
  HAL_SetSPISpeed(static_cast<HAL_SPIPort>(port), speed);
}

/*
 * Class:     edu_wpi_first_hal_SPIJNI
 * Method:    spiSetOpts
 * Signature: (IIII)V
 */
JNIEXPORT void JNICALL
Java_edu_wpi_first_hal_SPIJNI_spiSetOpts
  (JNIEnv*, jclass, jint port, jint msb_first, jint sample_on_trailing,
   jint clk_idle_high)
{
  HAL_SetSPIOpts(static_cast<HAL_SPIPort>(port), msb_first, sample_on_trailing,
                 clk_idle_high);
}

/*
 * Class:     edu_wpi_first_hal_SPIJNI
 * Method:    spiSetChipSelectActiveHigh
 * Signature: (I)V
 */
JNIEXPORT void JNICALL
Java_edu_wpi_first_hal_SPIJNI_spiSetChipSelectActiveHigh
  (JNIEnv* env, jclass, jint port)
{
  int32_t status = 0;
  HAL_SetSPIChipSelectActiveHigh(static_cast<HAL_SPIPort>(port), &status);
  CheckStatus(env, status);
}

/*
 * Class:     edu_wpi_first_hal_SPIJNI
 * Method:    spiSetChipSelectActiveLow
 * Signature: (I)V
 */
JNIEXPORT void JNICALL
Java_edu_wpi_first_hal_SPIJNI_spiSetChipSelectActiveLow
  (JNIEnv* env, jclass, jint port)
{
  int32_t status = 0;
  HAL_SetSPIChipSelectActiveLow(static_cast<HAL_SPIPort>(port), &status);
  CheckStatus(env, status);
}

/*
 * Class:     edu_wpi_first_hal_SPIJNI
 * Method:    spiInitAuto
 * Signature: (II)V
 */
JNIEXPORT void JNICALL
Java_edu_wpi_first_hal_SPIJNI_spiInitAuto
  (JNIEnv* env, jclass, jint port, jint bufferSize)
{
  int32_t status = 0;
  HAL_InitSPIAuto(static_cast<HAL_SPIPort>(port), bufferSize, &status);
  CheckStatus(env, status);
}

/*
 * Class:     edu_wpi_first_hal_SPIJNI
 * Method:    spiFreeAuto
 * Signature: (I)V
 */
JNIEXPORT void JNICALL
Java_edu_wpi_first_hal_SPIJNI_spiFreeAuto
  (JNIEnv* env, jclass, jint port)
{
  int32_t status = 0;
  HAL_FreeSPIAuto(static_cast<HAL_SPIPort>(port), &status);
  CheckStatus(env, status);
}

/*
 * Class:     edu_wpi_first_hal_SPIJNI
 * Method:    spiStartAutoRate
 * Signature: (ID)V
 */
JNIEXPORT void JNICALL
Java_edu_wpi_first_hal_SPIJNI_spiStartAutoRate
  (JNIEnv* env, jclass, jint port, jdouble period)
{
  int32_t status = 0;
  HAL_StartSPIAutoRate(static_cast<HAL_SPIPort>(port), period, &status);
  CheckStatus(env, status);
}

/*
 * Class:     edu_wpi_first_hal_SPIJNI
 * Method:    spiStartAutoTrigger
 * Signature: (IIIZZ)V
 */
JNIEXPORT void JNICALL
Java_edu_wpi_first_hal_SPIJNI_spiStartAutoTrigger
  (JNIEnv* env, jclass, jint port, jint digitalSourceHandle,
   jint analogTriggerType, jboolean triggerRising, jboolean triggerFalling)
{
  int32_t status = 0;
  HAL_StartSPIAutoTrigger(static_cast<HAL_SPIPort>(port), digitalSourceHandle,
                          static_cast<HAL_AnalogTriggerType>(analogTriggerType),
                          triggerRising, triggerFalling, &status);
  CheckStatus(env, status);
}

/*
 * Class:     edu_wpi_first_hal_SPIJNI
 * Method:    spiStopAuto
 * Signature: (I)V
 */
JNIEXPORT void JNICALL
Java_edu_wpi_first_hal_SPIJNI_spiStopAuto
  (JNIEnv* env, jclass, jint port)
{
  int32_t status = 0;
  HAL_StopSPIAuto(static_cast<HAL_SPIPort>(port), &status);
  CheckStatus(env, status);
}

/*
 * Class:     edu_wpi_first_hal_SPIJNI
 * Method:    spiSetAutoTransmitData
 * Signature: (I[BI)V
 */
JNIEXPORT void JNICALL
Java_edu_wpi_first_hal_SPIJNI_spiSetAutoTransmitData
  (JNIEnv* env, jclass, jint port, jbyteArray dataToSend, jint zeroSize)
{
  JByteArrayRef jarr(env, dataToSend);
  int32_t status = 0;
  HAL_SetSPIAutoTransmitData(
      static_cast<HAL_SPIPort>(port),
      reinterpret_cast<const uint8_t*>(jarr.array().data()),
      jarr.array().size(), zeroSize, &status);
  CheckStatus(env, status);
}

/*
 * Class:     edu_wpi_first_hal_SPIJNI
 * Method:    spiForceAutoRead
 * Signature: (I)V
 */
JNIEXPORT void JNICALL
Java_edu_wpi_first_hal_SPIJNI_spiForceAutoRead
  (JNIEnv* env, jclass, jint port)
{
  int32_t status = 0;
  HAL_ForceSPIAutoRead(static_cast<HAL_SPIPort>(port), &status);
  CheckStatus(env, status);
}

/*
 * Class:     edu_wpi_first_hal_SPIJNI
 * Method:    spiReadAutoReceivedData
 * Signature: (ILjava/lang/Object;ID)I
 */
JNIEXPORT jint JNICALL
Java_edu_wpi_first_hal_SPIJNI_spiReadAutoReceivedData__ILjava_nio_ByteBuffer_2ID
  (JNIEnv* env, jclass, jint port, jobject buffer, jint numToRead,
   jdouble timeout)
{
  uint32_t* recvBuf =
      reinterpret_cast<uint32_t*>(env->GetDirectBufferAddress(buffer));
  int32_t status = 0;
  jint retval = HAL_ReadSPIAutoReceivedData(
      static_cast<HAL_SPIPort>(port), recvBuf, numToRead, timeout, &status);
  CheckStatus(env, status);
  return retval;
}

/*
 * Class:     edu_wpi_first_hal_SPIJNI
 * Method:    spiReadAutoReceivedData
 * Signature: (I[IID)I
 */
JNIEXPORT jint JNICALL
Java_edu_wpi_first_hal_SPIJNI_spiReadAutoReceivedData__I_3IID
  (JNIEnv* env, jclass, jint port, jintArray buffer, jint numToRead,
   jdouble timeout)
{
  wpi::SmallVector<uint32_t, 128> recvBuf;
  recvBuf.resize(numToRead);
  int32_t status = 0;
  jint retval =
      HAL_ReadSPIAutoReceivedData(static_cast<HAL_SPIPort>(port),
                                  recvBuf.data(), numToRead, timeout, &status);
  if (!CheckStatus(env, status)) return retval;
  if (numToRead > 0) {
    env->SetIntArrayRegion(buffer, 0, numToRead,
                           reinterpret_cast<const jint*>(recvBuf.data()));
  }
  return retval;
}

/*
 * Class:     edu_wpi_first_hal_SPIJNI
 * Method:    spiGetAutoDroppedCount
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL
Java_edu_wpi_first_hal_SPIJNI_spiGetAutoDroppedCount
  (JNIEnv* env, jclass, jint port)
{
  int32_t status = 0;
  auto retval =
      HAL_GetSPIAutoDroppedCount(static_cast<HAL_SPIPort>(port), &status);
  CheckStatus(env, status);
  return retval;
}

}  // extern "C"
