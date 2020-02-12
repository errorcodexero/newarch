#pragma once

#include "NeutralMode.h"
#include "InvertType.h"
#include "ControlMode.h"
#include <frc/GPG3MotorController.h>
#include <cassert>


namespace ctre
{
    namespace phoenix
    {
        namespace motorcontrol
        {
            enum StatusFrameEnhanced
            {
                Status_1_General_,
                Status_2_Feedback0_,
                Status_4_AinTempVbat_,
                Status_6_Misc_,
                Status_7_CommStatus_,
                Status_9_MotProfBuffer_,
                Status_10_MotionMagic_,
                Status_10_Targets_,
                Status_12_Feedback1_,
                Status_13_Base_PIDF0_,
                Status_14_Turn_PIDF1_,
                Status_15_FirmareApiStatus_,
                Status_17_Targets1_ 
            } ;

            enum StatusFrame
            {
                Status_1_General,
                Status_2_Feedback0,
                Status_4_AinTempVbat,
                Status_6_Misc,
                Status_7_CommStatus,
                Status_9_MotProfBuffer,
                Status_10_MotionMagic,
                Status_10_Targets,
                Status_12_Feedback1,
                Status_13_Base_PIDF0,
                Status_14_Turn_PIDF1,
                Status_15_FirmareApiStatus,
                Status_17_Targets1,
                Status_3_Quadrature,
                Status_8_PulseWidth,
                Status_11_UartGadgeteer,
                Status_Brushless_Current 
            } ;

            class IMotorController : public frc::GPG3MotorController
            {
            public:
                IMotorController(int id)  : frc::GPG3MotorController(id)  {
                    isInverted_ = false;
                }

                virtual ~IMotorController() {
                }

                void Follow(IMotorController &talon) {      
                }                

                void SetNeutralMode(NeutralMode mode) {                    
                }

                void SetInverted(InvertType type) {                    
                }

                void SetInverted(bool b) {
                    isInverted_ = b;
                }

                void Set(ControlMode mode, double value) {
                    double motorPower = value;
                    assert(mode == ControlMode::PercentOutput) ;
                    if (isInverted_) {
                        motorPower *= -1;
                    }
                    frc::GPG3MotorController::Set(motorPower) ;
                }

                void ConfigVoltageCompSaturation(double voltage, int timeout) {
                }

                void EnableVoltageCompensation(bool b) {                    
                }

            private:
                bool isInverted_;
            } ;
        }
    }
}