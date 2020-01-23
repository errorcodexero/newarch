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

        private:
            MotorType mt_ ;
            int index_ ;
            SimulationModel &model_ ;
        } ;
    }
}