#pragma once

#include <ctre/phoenix/MotorControl/IMotorController.h>
#include <SimulatorEngine.h>
#include <CTREManager.h>

namespace ctre {
    namespace phoenix {
        namespace motorcontrol {
            enum StatusFrame
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

            enum StatusFrameEnhanced
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

            struct SupplyCurrentLimitConfiguration
            {
                bool enable ;
                double currentLimit ;
                double triggerThresholdCurrent ;
                double triggerThresholdTime ;

                SupplyCurrentLimitConfiguration()
                {
                    enable = false ;
                    currentLimit = 0.0 ;
                    triggerThresholdCurrent = 0.0 ;
                    triggerThresholdTime = 0.0 ;
                }

 	            SupplyCurrentLimitConfiguration (bool e, double cl, double tc, double tt)
                {
                    enable = e ;
                    currentLimit = cl ;
                    triggerThresholdCurrent = tc ;
                    triggerThresholdTime = tt ;
                }
            } ;
            
            namespace can {
                class TalonFX : public IMotorController
                {
                public:
                    TalonFX(int id) : IMotorController(id) 
                    {
                        auto &engine = xero::sim2::SimulatorEngine::getEngine() ;
                        auto &msg = engine.getMessageOutput();

                        msg.startMessage(xero::sim2::SimulatorMessages::MessageType::Debug, 5) ;
                        msg << "TalonFX: creating motor controller, index " << id;
                        msg.endMessage(engine.getSimulationTime());
                    }

                    virtual ~TalonFX() {                        
                    }

                    void SetStatusFramePeriod(StatusFrame frame, int time, int timeout = 0) {                        
                    }


                    void SetStatusFramePeriod(StatusFrameEnhanced frame, int time, int timeout = 0) {                        
                    }

                    void ConfigSupplyCurrentLimit(const ctre::phoenix::motorcontrol::SupplyCurrentLimitConfiguration &limits, int timeout = 50) {
                    }

                };
            }
        }
    }
}