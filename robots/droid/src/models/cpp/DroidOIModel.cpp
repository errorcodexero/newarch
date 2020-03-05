#include <DroidOIModel.h>
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
        std::array<double, 10> DroidOIModel::automode_axis_values_ = { -1.0, -0.8, -0.6, -0.4, -0.1, 0.1, 0.3, 0.5, 0.7, 0.9 } ;

        DroidOIModel::DroidOIModel(SimulatorEngine &engine, const std::string &inst) : SimulationModel(engine, "droidoi", inst)
        {
        }

        DroidOIModel::~DroidOIModel()
        {
        }

        void DroidOIModel::processEvent(const std::string &name, const SimValue &value)
        {
            if (name.length() > 6 && name.substr(0, 6) == "button")
            {
                std::string num = name.substr(6) ;
                size_t i ;
                int which ;

                try
                {
                    which = std::stoi(num, &i) ;
                    if (i != num.length())
                    {
                        SimulatorMessages &msg = getEngine().getMessageOutput() ;
                        msg.startMessage(SimulatorMessages::MessageType::Warning) ;
                        msg << "event: model " << getModelName() << " instance " << getInstanceName() ;
                        msg << " - event name '" << name << "' is not valid, expected buttonN where N is an integer" ;
                        msg.endMessage(getEngine().getSimulationTime()) ;                        
                        return ;
                    }
                }
                catch(const std::exception& e)
                {
                    SimulatorMessages &msg = getEngine().getMessageOutput() ;
                    msg.startMessage(SimulatorMessages::MessageType::Warning) ;
                    msg << "event: model " << getModelName() << " instance " << getInstanceName() ;
                    msg << " - event name '" << name << "' is not valid, expected buttonN where N is an integer" ;
                    msg.endMessage(getEngine().getSimulationTime()) ;                        
                    return ;
                }

                if (which >= buttons_.count)
                {
                    SimulatorMessages &msg = getEngine().getMessageOutput() ;
                    msg.startMessage(SimulatorMessages::MessageType::Warning) ;
                    msg << "event: model " << getModelName() << " instance " << getInstanceName() ;
                    msg << " - event name '" << name << "' is not valid, button index exceeded button count" ;
                    msg.endMessage(getEngine().getSimulationTime()) ;                        
                    return ;                    
                }
                
                if (!value.isBoolean())
                {
                    SimulatorMessages &msg = getEngine().getMessageOutput() ;
                    msg.startMessage(SimulatorMessages::MessageType::Warning) ;
                    msg << "event: model " << getModelName() << " instance " << getInstanceName() ;
                    msg << " - event '" << name << "' expected type boolean" ;
                    msg.endMessage(getEngine().getSimulationTime()) ;                        
                    return ;
                }
                
                which-- ;
                if (value.getBoolean())
                    buttons_.buttons |= (1 << which) ;
                else
                    buttons_.buttons &= ~(1 << which) ;

                HALSIM_SetJoystickButtons(index_, &buttons_) ;
            }
            else if (name.length() > 3 && name.substr(0, 3) == "pov")
            {
                std::string num = name.substr(3) ;
                size_t i ;
                int which ;

                try
                {
                    which = std::stoi(num, &i) ;
                    if (i != num.length())
                    {
                        SimulatorMessages &msg = getEngine().getMessageOutput() ;
                        msg.startMessage(SimulatorMessages::MessageType::Warning) ;
                        msg << "event: model " << getModelName() << " instance " << getInstanceName() ;
                        msg << " - event name '" << name << "' is not valid, expected povN where N is an integer" ;
                        msg.endMessage(getEngine().getSimulationTime()) ;                        
                        return ;
                    }
                }
                catch(const std::exception& e)
                {
                    SimulatorMessages &msg = getEngine().getMessageOutput() ;
                    msg.startMessage(SimulatorMessages::MessageType::Warning) ;
                    msg << "event: model " << getModelName() << " instance " << getInstanceName() ;
                    msg << " - event name '" << name << "' is not valid, expected povN where N is an integer" ;
                    msg.endMessage(getEngine().getSimulationTime()) ;                        
                    return ;
                }

                if (which >= povs_.count)
                {
                    SimulatorMessages &msg = getEngine().getMessageOutput() ;
                    msg.startMessage(SimulatorMessages::MessageType::Warning) ;
                    msg << "event: model " << getModelName() << " instance " << getInstanceName() ;
                    msg << " - event name '" << name << "' is not valid, pov index exceeded pov count" ;
                    msg.endMessage(getEngine().getSimulationTime()) ;                        
                    return ;                    
                }                
                
                if (!value.isInteger())
                {
                    SimulatorMessages &msg = getEngine().getMessageOutput() ;
                    msg.startMessage(SimulatorMessages::MessageType::Warning) ;
                    msg << "event: model " << getModelName() << " instance " << getInstanceName() ;
                    msg << " - event '" << name << "' expected type integer" ;
                    msg.endMessage(getEngine().getSimulationTime()) ;                        
                    return ;
                }
                
                povs_.povs[which] = static_cast<int16_t>(value.getInteger()) ;
                HALSIM_SetJoystickPOVs(index_, &povs_) ;
            }
            else if (name.length() > 4 && name.substr(0, 4) == "axis")
            {
                std::string num = name.substr(4) ;
                size_t i ;
                int which ;

                try
                {
                    which = std::stoi(num, &i) ;
                    if (i != num.length())
                    {
                        SimulatorMessages &msg = getEngine().getMessageOutput() ;
                        msg.startMessage(SimulatorMessages::MessageType::Warning) ;
                        msg << "event: model " << getModelName() << " instance " << getInstanceName() ;
                        msg << " - event name '" << name << "' is not valid, expected axisN where N is an integer" ;
                        msg.endMessage(getEngine().getSimulationTime()) ;                        
                        return ;
                    }
                }
                catch(const std::exception& e)
                {
                    SimulatorMessages &msg = getEngine().getMessageOutput() ;
                    msg.startMessage(SimulatorMessages::MessageType::Warning) ;
                    msg << "event: model " << getModelName() << " instance " << getInstanceName() ;
                    msg << " - event name '" << name << "' is not valid, expected axisN where N is an integer" ;
                    msg.endMessage(getEngine().getSimulationTime()) ;                        
                    return ;
                }

                if (which >= axis_.count)
                {
                    SimulatorMessages &msg = getEngine().getMessageOutput() ;
                    msg.startMessage(SimulatorMessages::MessageType::Warning) ;
                    msg << "event: model " << getModelName() << " instance " << getInstanceName() ;
                    msg << " - event name '" << name << "' is not valid, axis index exceeded axis count" ;
                    msg.endMessage(getEngine().getSimulationTime()) ;                        
                    return ;                    
                }                    
                
                if (!value.isDouble())
                {
                    SimulatorMessages &msg = getEngine().getMessageOutput() ;
                    msg.startMessage(SimulatorMessages::MessageType::Warning) ;
                    msg << "event: model " << getModelName() << " instance " << getInstanceName() ;
                    msg << " - event '" << name << "' expected type double" ;
                    msg.endMessage(getEngine().getSimulationTime()) ;                        
                    return ;
                }
                
                axis_.axes[which] = static_cast<float>(value.getDouble()) ;
                HALSIM_SetJoystickAxes(index_, &axis_) ;
            }
            else if (name == "automode")
            {
                if (!value.isInteger())
                {
                    SimulatorMessages &msg = getEngine().getMessageOutput() ;
                    msg.startMessage(SimulatorMessages::MessageType::Warning) ;
                    msg << "event: model " << getModelName() << " instance " << getInstanceName() ;
                    msg << " - event '" << name << "' expected type integer" ;
                    msg.endMessage(getEngine().getSimulationTime()) ;                        
                    return ;
                }

                axis_.axes[automode_axis_] = static_cast<float>(automode_axis_values_[value.getInteger()]) ;
                HALSIM_SetJoystickAxes(index_, &axis_) ;                
            }
        }

        bool DroidOIModel::create()
        {
            memset(&desc_, 0, sizeof(desc_)) ;
            desc_.axisCount = 0 ;
            desc_.buttonCount = 0 ;
            desc_.povCount = 0 ;
            strcpy(desc_.name, "SimulatedJoystick") ;

            if (hasProperty("hw:index"))
            {
                const SimValue &value = getProperty("hw:index") ;
                if (!value.isInteger())
                {
                    SimulatorMessages &msg = getEngine().getMessageOutput() ;
                    msg.startMessage(SimulatorMessages::MessageType::Warning) ;
                    msg << "model " << getModelName() << " instance " << getInstanceName() ;
                    msg << " - has property 'index' but it is not an integer" ;
                    msg.endMessage(getEngine().getSimulationTime()) ;

                    return false ;
                }

                index_ = value.getInteger() ;
            }
            else
            {
                SimulatorMessages &msg = getEngine().getMessageOutput() ;
                msg.startMessage(SimulatorMessages::MessageType::Warning) ;
                msg << "model " << getModelName() << " instance " << getInstanceName() ;
                msg << " - is missing required property 'index', an integer" ;
                msg.endMessage(getEngine().getSimulationTime()) ;                
            }

            if (hasProperty("automode"))
            {
                const SimValue &value = getProperty("automode") ;
                if (!value.isInteger())
                {
                    SimulatorMessages &msg = getEngine().getMessageOutput() ;
                    msg.startMessage(SimulatorMessages::MessageType::Warning) ;
                    msg << "model " << getModelName() << " instance " << getInstanceName() ;
                    msg << " - has property 'automode' but it is not an integer" ;
                    msg.endMessage(getEngine().getSimulationTime()) ;

                    return false ;
                }

                automode_axis_ = value.getInteger() ;
            }
            else
            {
                SimulatorMessages &msg = getEngine().getMessageOutput() ;
                msg.startMessage(SimulatorMessages::MessageType::Warning) ;
                msg << "model " << getModelName() << " instance " << getInstanceName() ;
                msg << " - is missing required property 'automode', an integer" ;
                msg.endMessage(getEngine().getSimulationTime()) ;
            }

            if (hasProperty("xbox"))
            {
                const SimValue &value = getProperty("xbox") ;                
                if (!value.isBoolean())
                {
                    SimulatorMessages &msg = getEngine().getMessageOutput() ;
                    msg.startMessage(SimulatorMessages::MessageType::Warning) ;
                    msg << "model " << getModelName() << " instance " << getInstanceName() ;
                    msg << " - has property 'xbox' but it is not a boolean" ;
                    msg.endMessage(getEngine().getSimulationTime()) ;

                    return false ;
                }

                desc_.isXbox = value.getBoolean() ;
            }

            if (hasProperty("name"))
            {
                const SimValue &value = getProperty("name") ;                
                if (!value.isString())
                {
                    SimulatorMessages &msg = getEngine().getMessageOutput() ;
                    msg.startMessage(SimulatorMessages::MessageType::Warning) ;
                    msg << "model " << getModelName() << " instance " << getInstanceName() ;
                    msg << " - has property 'name' but it is not a boolean" ;
                    msg.endMessage(getEngine().getSimulationTime()) ;

                    return false ;
                }
                strcpy(desc_.name, value.getString().c_str()) ;
            }

            if (hasProperty("axes"))
            {
                const SimValue &value = getProperty("axes") ;
                if (!value.isInteger())
                {
                    SimulatorMessages &msg = getEngine().getMessageOutput() ;
                    msg.startMessage(SimulatorMessages::MessageType::Warning) ;
                    msg << "model " << getModelName() << " instance " << getInstanceName() ;
                    msg << " - has property 'axes' but it is not a integer" ;
                    msg.endMessage(getEngine().getSimulationTime()) ;

                    return false ;
                }

                desc_.axisCount = value.getInteger() ;
                axis_.count = value.getInteger() ;

                for(int i = 0 ; i < axis_.count ; i++)
                    axis_.axes[i] = 0.0 ;

                HALSIM_SetJoystickAxes(index_, &axis_) ;                    
            }

            if (hasProperty("buttons"))
            {
                const SimValue &value = getProperty("buttons") ;
                if (!value.isInteger())
                {
                    SimulatorMessages &msg = getEngine().getMessageOutput() ;
                    msg.startMessage(SimulatorMessages::MessageType::Warning) ;
                    msg << "model " << getModelName() << " instance " << getInstanceName() ;
                    msg << " - has property 'buttons' but it is not a integer" ;
                    msg.endMessage(getEngine().getSimulationTime()) ;

                    return false ;
                }

                desc_.buttonCount = value.getInteger() ;
                buttons_.count = value.getInteger() ;
                buttons_.buttons = 0 ;

                HALSIM_SetJoystickButtons(index_, &buttons_) ;                
            }

            if (hasProperty("povs"))
            {
                const SimValue &value = getProperty("povs") ;
                if (!value.isInteger())
                {
                    SimulatorMessages &msg = getEngine().getMessageOutput() ;
                    msg.startMessage(SimulatorMessages::MessageType::Warning) ;
                    msg << "model " << getModelName() << " instance " << getInstanceName() ;
                    msg << " - has property 'povs' but it is not a integer" ;
                    msg.endMessage(getEngine().getSimulationTime()) ;

                    return false ;
                }

                desc_.povCount = value.getInteger() ;
                povs_.count = value.getInteger() ;
                for(int i = 0 ; i < povs_.count ; i++)
                    povs_.povs[i] = 0 ;

                HALSIM_SetJoystickPOVs(index_, &povs_) ;                       
            }

            HALSIM_SetJoystickDescriptor(index_, &desc_) ;
            return true ;
        }

        void DroidOIModel::run(uint64_t microdt) 
        {
        }
    }
}
