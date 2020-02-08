#include <LimeLightModel.h>
#include <SimulatorEngine.h>
#include <SimulatedMotor.h>
#include <hal/HALBase.h>
#include <cassert>
#include <iostream>

using namespace xero::sim2;

namespace xero
{
    namespace models
    {
        LimeLightModel::LimeLightModel(SimulatorEngine &engine, const std::string &inst) : SimulationModel(engine, "limelight", inst)
        {
        }

        LimeLightModel::~LimeLightModel()
        {
        }

        void LimeLightModel::processEvent(const std::string &name, const SimValue &value)
        {
            if (name == "tv")
            {
                if (!value.isDouble())
                {
                    SimulatorMessages &msg = getEngine().getMessageOutput() ;
                    msg.startMessage(SimulatorMessages::MessageType::Warning) ;
                    msg << "event: model " << getModelName() << " instance " << getInstanceName() ;
                    msg << " - event name '" << name << "' value is not a double" ;
                    msg.endMessage(getEngine().getSimulationTime()) ;                        
                    return ;
                }

                table_->PutNumber(tvkey, value.getDouble()) ;
            }
            else if (name == "tx")
            {
                if (!value.isDouble())
                {
                    SimulatorMessages &msg = getEngine().getMessageOutput() ;
                    msg.startMessage(SimulatorMessages::MessageType::Warning) ;
                    msg << "event: model " << getModelName() << " instance " << getInstanceName() ;
                    msg << " - event name '" << name << "' value is not a double" ;
                    msg.endMessage(getEngine().getSimulationTime()) ;                        
                    return ;
                }

                table_->PutNumber(txkey, value.getDouble()) ;
            }
            else if (name == "ty")
            {
                if (!value.isDouble())
                {
                    SimulatorMessages &msg = getEngine().getMessageOutput() ;
                    msg.startMessage(SimulatorMessages::MessageType::Warning) ;
                    msg << "event: model " << getModelName() << " instance " << getInstanceName() ;
                    msg << " - event name '" << name << "' value is not a double" ;
                    msg.endMessage(getEngine().getSimulationTime()) ;                        
                    return ;
                }

                table_->PutNumber(tykey, value.getDouble()) ;
            }
        }

        bool LimeLightModel::create()
        {
            return true ;
        }

        void LimeLightModel::run(uint64_t microdt) 
        {
        }
    }
}
