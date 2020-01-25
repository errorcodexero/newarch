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
        FMS::FMS(SimulatorEngine &engine, const std::string &inst) : SimulationModel(engine, "FMS", inst)
        {
            start_time_ = 1000000 ;
            auto_time_ = 15000000 ;
            between_time_ = 2000000 ;
            teleop_time_ = 135000000 ;
        }

        FMS::~FMS()
        {
        }

        bool FMS::create()
        {
            int status = 0 ;

            state_ = FMSState::Start ;
            period_start_time_ = HAL_GetFPGATime(&status) ;

            return true ;
        }

        void FMS::run(uint64_t microdt) 
        {
            int status = 0 ;
            SimulatorMessages &msg = getEngine().getMessageOutput() ;            

            uint64_t elapsed = HAL_GetFPGATime(&status) - period_start_time_ ;

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

            case FMSState::Done:
                msg.startMessage(SimulatorMessages::MessageType::Debug, 1) ;
                msg << "model " << getModelName() << " instance " << getInstanceName() ;
                msg << " - end of match" ;
                msg.endMessage(getEngine().getSimulationTime()) ;            
                break ;                                                                
            }
        }
    }
}
