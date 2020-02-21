#include <ShooterModel.h>
#include <SimulatorEngine.h>
#include <SimulatedMotor.h>
#include <mockdata/DriverStationData.h>
#include <hal/HALBase.h>
#include <cassert>
#include <iostream>

using namespace xero::sim2;

namespace xero
{
    namespace models
    {
        ShooterModel::ShooterModel(SimulatorEngine &engine, const std::string &inst) : SimulationModel(engine, "shooter", inst)
        {
            kv_ = 0.000130571 ;
        }

        ShooterModel::~ShooterModel()
        {
        }

        void ShooterModel::processEvent(const std::string &name, const SimValue &value)
        {
        }

        bool ShooterModel::create()
        {
            shooter_ = std::make_shared<SimulatedMotor>(*this, "hw:shooter:motor") ;
            current_speed_rpm_ = 0.0 ;
            encoder_count_ = 0.0 ;

            if (!hasProperty(KVPropName))
            {
                SimulatorMessages &msg = getEngine().getMessageOutput() ;
                msg.startMessage(SimulatorMessages::MessageType::Error) ;
                msg << "model " << getModelName() << " instance " << getInstanceName() ;
                msg << " - is missing property '" << KVPropName << "'" ;
                msg.endMessage(getEngine().getSimulationTime()) ;
                return false ;   
            }

            if (!hasProperty(ChangePropName))
            {
                SimulatorMessages &msg = getEngine().getMessageOutput() ;
                msg.startMessage(SimulatorMessages::MessageType::Error) ;
                msg << "model " << getModelName() << " instance " << getInstanceName() ;
                msg << " - is missing property '" << ChangePropName << "'" ;
                msg.endMessage(getEngine().getSimulationTime()) ;
                return false ;   
            }

            auto &kvprop = getProperty(KVPropName) ;
            if (!kvprop.isDouble())
            {
                SimulatorMessages &msg = getEngine().getMessageOutput() ;
                msg.startMessage(SimulatorMessages::MessageType::Error) ;
                msg << "model " << getModelName() << " instance " << getInstanceName() ;
                msg << " - has property '" << KVPropName << "' but type is not double" ;
                msg.endMessage(getEngine().getSimulationTime()) ;
                return false ;                
            }
            kv_ = kvprop.getDouble() ;

            auto &chprop = getProperty(ChangePropName) ;
            if (!chprop.isDouble())
            {
                SimulatorMessages &msg = getEngine().getMessageOutput() ;
                msg.startMessage(SimulatorMessages::MessageType::Error) ;
                msg << "model " << getModelName() << " instance " << getInstanceName() ;
                msg << " - has property '" << ChangePropName << "' but type is not double" ;
                msg.endMessage(getEngine().getSimulationTime()) ;
                return false ; 
            }

            rpm_change_per_second_ = chprop.getDouble() ;
            return true ;
        }

        void ShooterModel::run(uint64_t microdt) 
        {
            double calc_speed = shooter_->get() / kv_ ;
            double delta = calc_speed - current_speed_rpm_ ;

            if (std::fabs(delta) > rpm_change_per_second_ * microdt / 1e6)
            {
                if (delta < 0.0)
                    delta = -rpm_change_per_second_ * microdt / 1e6 ;
                else
                    delta = rpm_change_per_second_ * microdt / 1e6 ;                    
            }

            current_speed_rpm_ += delta ;
            double revs = current_speed_rpm_ * microdt / 1e6 / 60 ;
            encoder_count_ += revs * shooter_->ticksPerRev() ;
        }
    }
}
