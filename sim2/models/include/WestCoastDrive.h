#pragma once
#include <SimulatedMotor.h>
#include <SimulationModel.h>
#include <memory>

namespace xero
{
    namespace models
    {
        class WestCoastDrive : public xero::sim2::SimulationModel
        {
        public:
            WestCoastDrive(xero::sim2::SimulatorEngine &engine, const std::string &inst);
            virtual ~WestCoastDrive();

            virtual bool create() ;
            virtual void run();

        private:
            std::shared_ptr<xero::sim2::SimulatedMotor> left_motor_ ;
            std::shared_ptr<xero::sim2::SimulatedMotor> right_motor_ ;
            int left_enc_[2] ;
            int right_enc_[2] ;
        };
    }
}