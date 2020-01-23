#include "SimulatedMotor.h"
#include "SimulationModel.h"
#include "CTREManager.h"
#include "REVManager.h"
#include <stdexcept>

namespace xero
{
    namespace sim2
    {
        SimulatedMotor::SimulatedMotor(SimulationModel &model, const std::string &name) : model_(model)
        {
            std::string basename , propname ;
            SimulatorMessages &msg = model.getMessageOutput() ;
            SimValue value ;

            basename = name ;

            propname = basename + ":type" ;
            if (!model.hasProperty(propname))
                basename += ":1" ;
            
            propname = basename + ":type" ;            
            if (!model.hasProperty(propname))
            {
                msg.startMessage(SimulatorMessages::MessageType::Error) ;
                msg << "model " << model.getModelName() << " instance " << model.getInstanceName();
                msg << ": cannot create motor " << name << " - no 'type' field";
                msg.endMessage(model.getSimulationTime()) ;

                std::runtime_error err("cannot create motor") ;
                throw err ;
            }

            propname = basename + ":index" ;
            if (!model.hasProperty(propname))
            {
                msg.startMessage(SimulatorMessages::MessageType::Error) ;
                msg << "model " << model.getModelName() << " instance " << model.getInstanceName();
                msg << ": cannot create motor " << name << " - no 'index' field";
                msg.endMessage(model.getSimulationTime()) ;

                std::runtime_error err("cannot create motor") ;
                throw err ;
            }

            value = model.getProperty(propname) ;
            if (!value.isInteger())
            {
                msg.startMessage(SimulatorMessages::MessageType::Error) ;
                msg << "model " << model.getModelName() << " instance " << model.getInstanceName();
                msg << ": cannot create motor " << name << " - 'index' field is not an integer";
                msg.endMessage(model.getSimulationTime()) ;

                std::runtime_error err("cannot create motor") ;
                throw err ;                
            }

            index_ = value.getInteger() ;

            propname = basename + ":type" ;
            value = model.getProperty(propname) ;

            if (!value.isString())
            {
                msg.startMessage(SimulatorMessages::MessageType::Error) ;
                msg << "model " << model.getModelName() << " instance " << model.getInstanceName();                
                msg << ": cannot create motor " << name << " - 'type' field is not a string" ;
                msg.endMessage(model.getSimulationTime()) ;

                std::runtime_error err("cannot create motor") ;
                throw err ;                
            }            

            if (value.getString() == "talonsrx")
                mt_ = MotorType::TalonSRX ;
            else if (value.getString() == "talonfx")
                mt_ = MotorType::TalonFX ;
            else if (value.getString() == "victorspx")
                mt_ = MotorType::VictorSPX ;
            else if (value.getString() == "victorsp")
                mt_ = MotorType::VictorSP ;
            else if (value.getString() == "sparkmax")
                mt_ = MotorType::SparkMax ;
            else
            {
                msg.startMessage(SimulatorMessages::MessageType::Error) ;
                msg << "model " << model.getModelName() << " instance " << model.getInstanceName();                 
                msg << ": cannot create motor " << name << " - 'type' field does not specify a known motor type" ;
                msg.endMessage(model.getSimulationTime()) ;

                msg.startMessage(SimulatorMessages::MessageType::Error) ;
                msg << "    cannot create motor " << name << "   'type' field has value '" << value.getString() << "'" ;
                msg.endMessage(model.getSimulationTime()) ;                

                msg.startMessage(SimulatorMessages::MessageType::Error) ;
                msg << "    cannot create motor " << name << "   expected 'talonsrx' or 'talonfx' or 'victorspx' or 'victorsp' or 'sparkmax'" ;
                msg.endMessage(model.getSimulationTime()) ;                    

                std::runtime_error err("cannot create motor") ;
                throw err ;                  
            }


        }

        SimulatedMotor::~SimulatedMotor()
        {
        }

        double SimulatedMotor::get()
        {
            double ret = 0 ;

            switch(mt_)
            {
            case MotorType::SparkMax:
                ret = model_.getEngine().getREVManager()->get(index_, ret) ;            
                break ;

            case MotorType::TalonFX:
            case MotorType::TalonSRX:
            case MotorType::VictorSPX:
                ret = model_.getEngine().getCTREManager()->get(index_, ret) ;
                break ;               

            case MotorType::VictorSP:
                ret = model_.getEngine().getPWM(index_) ;
                break ;         
            }

            return ret ;
        }

        void SimulatedMotor::setEncoder(int32_t value)
        {
            switch(mt_)
            {
            case MotorType::SparkMax:
                model_.getEngine().getREVManager()->setEncoder(index_, value) ;
                break ;

            case MotorType::TalonFX:
                model_.getEngine().getCTREManager()->setEncoder(index_, value) ;
                break ;               

            case MotorType::VictorSP:
            case MotorType::TalonSRX:
            case MotorType::VictorSPX:
                {
                    SimulatorMessages &msg = model_.getEngine().getMessageOutput() ;
                    msg.startMessage(SimulatorMessages::MessageType::Error) ;
                    msg << "model " << model_.getModelName() << " instance " << model_.getInstanceName() ;
                    msg << " - trying to set encoder value on motor that does not have encoders" ;
                    msg.endMessage(model_.getEngine().getSimulationTime()) ;

                    std::runtime_error err("cannot set encoder values") ;
                    throw err ;
                }
                break ;         
            }
        }        
    }
}