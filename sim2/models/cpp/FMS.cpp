#include <FMS.h>
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
        FMS::FMS(SimulatorEngine &engine, const std::string &inst) : SimulationModel(engine, "fms", inst)
        {
            start_time_ = 0.0 ;
            auto_time_ = 0.0 ;
            teleop_time_ = 0.0 ;
            between_time_ = 0.0 ;
        }

        FMS::~FMS()
        {
        }

        double FMS::setProperty(const std::string &propname, const xero::sim2::SimValue &value, double target)
        {
            if (!value.isDouble())
            {
                SimulatorMessages &msg = getEngine().getMessageOutput() ;
                msg.startMessage(SimulatorMessages::MessageType::Warning) ;
                msg << "model " << getModelName() << " instance " << getInstanceName() ;
                msg << " - has property '" << propname << "' but it is not a double" ;
                msg.endMessage(getEngine().getSimulationTime()) ;
                return target ;
            }

            return value.getDouble() ;
        }

        void FMS::processEvent(const std::string &name, const SimValue &value)
        {
            if (name == "autonomous")
                auto_time_ = setProperty("autonomous", value, auto_time_) ;
            else if (name == "start")
                start_time_ = setProperty("start", value, start_time_) ;
            else if (name == "between")
                between_time_ = setProperty("between", value, between_time_) ;
            else if (name == "teleop")
                teleop_time_ = setProperty("teleop", value, teleop_time_) ;
            else if (name == "fms")
            {
                if (!value.isBoolean())
                {
                    SimulatorMessages &msg = getEngine().getMessageOutput() ;
                    msg.startMessage(SimulatorMessages::MessageType::Warning) ;
                    msg << "model " << getModelName() << " instance " << getInstanceName() ;
                    msg << " - has property 'fms' but it is not a boolean" ;
                    msg.endMessage(getEngine().getSimulationTime()) ;                    
                }
                else
                {
                    HALSIM_SetDriverStationFmsAttached(value.getBoolean()) ;
                }
            }
            else if (name == "station")
            {
                if (!value.isString())
                {
                    SimulatorMessages &msg = getEngine().getMessageOutput() ;
                    msg.startMessage(SimulatorMessages::MessageType::Warning) ;
                    msg << "model " << getModelName() << " instance " << getInstanceName() ;
                    msg << " - has property 'station' but it is not a string" ;
                    msg.endMessage(getEngine().getSimulationTime()) ;
                }
                else
                {
                    if (value.getString() == "red1")
                    {
                        HALSIM_SetDriverStationAllianceStationId(HAL_AllianceStationID_kRed1) ;
                    }
                    else if (value.getString() == "red2")
                    {
                        HALSIM_SetDriverStationAllianceStationId(HAL_AllianceStationID_kRed2) ;
                    }   
                    else if (value.getString() == "red3")
                    {
                        HALSIM_SetDriverStationAllianceStationId(HAL_AllianceStationID_kRed3) ;
                    }
                    else if (value.getString() == "blue1")
                    {
                        HALSIM_SetDriverStationAllianceStationId(HAL_AllianceStationID_kBlue1) ;
                    }
                    else if (value.getString() == "blue2")
                    {
                        HALSIM_SetDriverStationAllianceStationId(HAL_AllianceStationID_kBlue2) ;
                    }
                    else if (value.getString() == "blue3")
                    {
                        HALSIM_SetDriverStationAllianceStationId(HAL_AllianceStationID_kBlue2) ;
                    }
                    else
                    {
                        SimulatorMessages &msg = getEngine().getMessageOutput() ;
                        msg.startMessage(SimulatorMessages::MessageType::Warning) ;
                        msg << "model " << getModelName() << " instance " << getInstanceName() ;
                        msg << " - has property 'station' is a string, but is not 'red1', 'red2', 'red3', 'blue1', 'blue2', or 'blue3'" ;
                        msg.endMessage(getEngine().getSimulationTime()) ;                    
                    }                                                                                                                     
                }                
            }
        }

        bool FMS::create()
        {
            int status = 0 ;

            state_ = FMSState::Start ;
            period_start_time_ = HAL_GetFPGATime(&status) / 1e6 ;

            if (hasProperty("start"))
                start_time_ = setProperty("start", getProperty("start"), start_time_) ;

            if (hasProperty("autonomous"))
                auto_time_ = setProperty("autonomous", getProperty("autonomous"), auto_time_) ;

            if (hasProperty("between"))
                between_time_ = setProperty("between", getProperty("between"), between_time_) ;

            if (hasProperty("teleop"))
                teleop_time_ = setProperty("teleop", getProperty("teleop"), teleop_time_) ;                                                

            return true ;
        }

        void FMS::run(uint64_t microdt) 
        {
            int status = 0 ;
            SimulatorMessages &msg = getEngine().getMessageOutput() ;            

            double elapsed = static_cast<double>(HAL_GetFPGATime(&status)) / 1e6 - period_start_time_ ;
            switch(state_)
            {
            case FMSState::Start:
                if (elapsed > start_time_)
                {
                    HALSIM_SetDriverStationAutonomous(true) ;                    
                    HALSIM_SetDriverStationEnabled(true) ;
                    state_ = FMSState::Auto ;
                    period_start_time_ = HAL_GetFPGATime(&status) / 1e6 ;

                    msg.startMessage(SimulatorMessages::MessageType::Debug, 4) ;
                    msg << "model " << getModelName() << " instance " << getInstanceName() ;
                    msg << " - transitioning to AUTOMODE" ;
                    msg.endMessage(getEngine().getSimulationTime()) ;                     
                }
                break ;

            case FMSState::Auto:
                if (elapsed > auto_time_)
                {
                    HALSIM_SetDriverStationEnabled(false) ;
                    state_ = FMSState::Between ;
                    period_start_time_ = HAL_GetFPGATime(&status) / 1e6 ;

                    msg.startMessage(SimulatorMessages::MessageType::Debug, 4) ;
                    msg << "model " << getModelName() << " instance " << getInstanceName() ;
                    msg << " - transitioning to BETWEEN" ;
                    msg.endMessage(getEngine().getSimulationTime()) ;                       
                }            
                break ;

            case FMSState::Between:
                if (elapsed > between_time_)
                {
                    HALSIM_SetDriverStationAutonomous(false) ;                     
                    HALSIM_SetDriverStationEnabled(true) ;
                    state_ = FMSState::Teleop ;
                    period_start_time_ = HAL_GetFPGATime(&status) / 1e6 ;

                    msg.startMessage(SimulatorMessages::MessageType::Debug, 4) ;
                    msg << "model " << getModelName() << " instance " << getInstanceName() ;
                    msg << " - transitioning to TELEOP" ;
                    msg.endMessage(getEngine().getSimulationTime()) ;                                    
                }              
                break ;

            case FMSState::Teleop:
                if (elapsed > teleop_time_)
                {
                    HALSIM_SetDriverStationEnabled(false) ;
                    state_ = FMSState::Done ;
                    period_start_time_ = HAL_GetFPGATime(&status) / 1e6 ;

                    msg.startMessage(SimulatorMessages::MessageType::Debug, 4) ;
                    msg << "model " << getModelName() << " instance " << getInstanceName() ;
                    msg << " - transitioning to DONE" ;
                    msg.endMessage(getEngine().getSimulationTime()) ;                                          
                }               
                break ;

            case FMSState::Done:
                break ;                                              
            }
        }
    }
}
