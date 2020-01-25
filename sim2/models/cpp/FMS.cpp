#include <FMS.h>
#include <SimulatorEngine.h>
#include <SimulatedMotor.h>

#include <mockdata/DriverStationData.h>
#include <hal/HALBase.h>
#include <cassert>

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

        void FMS::setProperty(const std::string &propname, const xero::sim2::SimValue &value, double &target)
        {
            auto prop = getProperty(propname) ;
            if (!prop.isDouble())
            {
                SimulatorMessages &msg = getEngine().getMessageOutput() ;
                msg.startMessage(SimulatorMessages::MessageType::Warning) ;
                msg << "model " << getModelName() << " instance " << getInstanceName() ;
                msg << " - has property '" << propname << "' but it is not a double" ;
                msg.endMessage(getEngine().getSimulationTime()) ;
            }

            target = prop.getDouble() ;
        }

        void FMS::processEvent(const std::string &name, const SimValue &value)
        {
            if (name == "automode")
                setProperty("automode", value, auto_time_) ;
            else if (name == "start")
                setProperty("start", value, start_time_) ;
            else if (name == "between")
                setProperty("between", value, between_time_) ;
            else if (name == "teleop")
                setProperty("teleop", value, teleop_time_) ;
        }

        bool FMS::create()
        {
            int status = 0 ;

            state_ = FMSState::Start ;
            period_start_time_ = HAL_GetFPGATime(&status) / 1e6 ;

            if (hasProperty("start"))
                setProperty("start", getProperty("start"), start_time_) ;

            if (hasProperty("automode"))
                setProperty("automode", getProperty("automode"), auto_time_) ;

            if (hasProperty("between"))
                setProperty("between", getProperty("between"), between_time_) ;

            if (hasProperty("teleop"))
                setProperty("teleop", getProperty("teleop"), teleop_time_) ;                                                

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
                    period_start_time_ = HAL_GetFPGATime(&status) ;
                }
                break ;

            case FMSState::Auto:
                if (elapsed > auto_time_)
                {
                    HALSIM_SetDriverStationEnabled(false) ;
                    state_ = FMSState::Between ;
                    period_start_time_ = HAL_GetFPGATime(&status) ;                    
                }            
                break ;

            case FMSState::Between:
                if (elapsed > between_time_)
                {
                    HALSIM_SetDriverStationAutonomous(false) ;                     
                    HALSIM_SetDriverStationEnabled(true) ;
                    state_ = FMSState::Teleop ;
                    period_start_time_ = HAL_GetFPGATime(&status) ;                    
                }              
                break ;

            case FMSState::Teleop:
                if (elapsed > teleop_time_)
                {
                    HALSIM_SetDriverStationEnabled(false) ;
                    state_ = FMSState::Done ;
                    period_start_time_ = HAL_GetFPGATime(&status) ;                    
                }               
                break ;

            case FMSState::MessageDone:
                msg.startMessage(SimulatorMessages::MessageType::Debug, 1) ;
                msg << "model " << getModelName() << " instance " << getInstanceName() ;
                msg << " - end of match" ;
                msg.endMessage(getEngine().getSimulationTime()) ;            

                state_ = FMSState::Done ;
                break ;                  

            case FMSState::Done:
                break ;                                              
            }
        }
    }
}
