#pragma once

#include <string>

namespace xero
{
    namespace sim2
    {
        class SimulationModel ;
        
        class SimulatedMotor
        {
        public:
            enum class MotorType
            {
                TalonSRX,
                TalonFX,
                VictorSPX,
                VictorSP,
                SparkMax
            } ;

        public:
            SimulatedMotor(SimulationModel &model, const std::string &name) ;
            virtual ~SimulatedMotor() ;

            double get() ;
            void setEncoder(int32_t value) ;

            bool hasEncoder() {
                return mt_ == MotorType::TalonFX || mt_ == MotorType::SparkMax ;
            }

            int ticksPerRev() {
                int ret = 0 ;

                if (mt_ == MotorType::TalonFX)
                    ret = 2048 ;
                else if (mt_ == MotorType::SparkMax)
                    ret = 42 ;

                return ret ;                
            }

        private:
            MotorType mt_ ;
            int index_ ;
            SimulationModel &model_ ;
        } ;
    }
}