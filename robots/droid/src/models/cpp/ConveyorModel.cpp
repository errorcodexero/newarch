#include <ConveyorModel.h>
#include <SimulatorEngine.h>
#include <SimulatedMotor.h>
#include <mockdata/DIOData.h>
#include <hal/HALBase.h>
#include <cassert>
#include <iostream>

using namespace xero::sim2;

namespace xero
{
    namespace models
    {
        ConveyorModel::ConveyorModel(SimulatorEngine &engine, const std::string &inst) : SimulationModel(engine, "conveyor", inst)
        {
            printall_ = false ;
        }

        ConveyorModel::~ConveyorModel()
        {
        }

        void ConveyorModel::processEvent(const std::string &name, const SimValue &value)
        {
            if (name == "ball")
            {
                if (value.isBoolean() && value.getBoolean())
                {
                    insertBallFromIntake(false) ;
                }
            }
            if (name == "true")
            {
                if (value.isBoolean() && value.getBoolean())
                {
                    insertBallFromIntake(true) ;
                }
            }            
            else if (name == "print")
            {
                if (value.isBoolean() && value.getBoolean())
                {
                    printBalls() ;
                }                
            }
            else if (name == "printall")
            {
                if (value.isBoolean() && value.getBoolean())
                {
                    printall_ = true ;
                }
                else
                {
                    printall_ = false ;
                }
            }            
        }

        bool ConveyorModel::create()
        {
            belt_ = std::make_shared<SimulatedMotor>(*this, "hw:belt") ;
            turret_ = std::make_shared<SimulatedMotor>(*this, "hw:turret") ;

            sensorA_ = getInteger("hw:sensorA") ;
            sensorB_ = getInteger("hw:sensorB") ;
            sensorC_ = getInteger("hw:sensorC") ;
            sensorD_ = getInteger("hw:sensorD") ;

            sensorAPos_ = getDouble("positionA") ;
            sensorBPos_ = getDouble("positionB") ;
            sensorCPos_ = getDouble("positionC") ;
            sensorDPos_ = getDouble("positionD") ;            

            HALSIM_SetDIOValue(sensorA_, true) ;
            HALSIM_SetDIOValue(sensorB_, true) ;
            HALSIM_SetDIOValue(sensorC_, true) ;
            HALSIM_SetDIOValue(sensorD_, true) ;

            for(int i = 0 ; i < MaxBalls ; i++)
                balls_[i].present_ = false ;

            dist_per_second_per_volt_ = getDouble("distance_per_second_per_inch") ;

            return true ;
        }

        void ConveyorModel::run(uint64_t microdt) 
        {
            double tm = microdt / 1.0e6 ;
            double dist = belt_->get() * dist_per_second_per_volt_ * tm ;
            double dist2 = turret_->get() * dist_per_second_per_volt_ * tm ;

            if (dist > 0)
            {
                for(int i = MaxBalls - 1 ; i >= 0 ; i--)
                {
                    if (balls_[i].present_)
                    {
                        if (balls_[i].position_ > 15.0)
                            balls_[i].position_ += dist2 ;
                        else
                            balls_[i].position_ += dist ;
                    }
                }

                int cnt = countBalls() ;
                if (cnt > 0)
                {
                    if (balls_[cnt].position_ > 31.0)
                        deleteBallFromShooter() ;
                    if (balls_[0].position_ > 0.0 && balls_[0].drop_)
                        deleteBallFromIntake() ;
                }
            }
            else
            {
                for(int i = 0 ; i < MaxBalls ; i++)
                {
                    if (balls_[i].present_)
                    {
                        double newloc = balls_[i].position_ + dist ;
                        balls_[i].position_ = newloc ;
                    }
                }

                int cnt = countBalls() ;
                if (cnt > 0)
                {
                    if (balls_[0].position_ < -7.0)
                        deleteBallFromIntake() ;
                }                
            }

            // Update sensors


            bool sa = true ;
            bool sb = true ;
            bool sc = true ;
            bool sd = true ;

            for (int i = 0; i < MaxBalls; i++) {
                if (!balls_[i].present_) 
                    continue;

                double pos = balls_[i].position_;

                if (pos > sensorAPos_ - BallDiameter / 2.0 && pos < sensorAPos_ + BallDiameter / 2.0)
                    sa = false ;

                if (pos > sensorBPos_ - BallDiameter / 2.0 && pos < sensorBPos_ + BallDiameter / 2.0)
                    sb = false ;

                if (pos > sensorCPos_ - BallDiameter / 2.0 && pos < sensorCPos_ + BallDiameter / 2.0)
                    sc = false ;

                if (pos > sensorDPos_ - BallDiameter / 2.0 && pos < sensorDPos_ + BallDiameter / 2.0)
                    sd = false ;
            }

            HALSIM_SetDIOValue(sensorA_, sa) ;
            HALSIM_SetDIOValue(sensorB_, sb) ;
            HALSIM_SetDIOValue(sensorC_, sc) ;
            HALSIM_SetDIOValue(sensorD_, sd) ;

            if (printall_)
                printBalls() ;
        }

        int ConveyorModel::countBalls()
        {
            int cnt = 0 ;

            for(int i = 0 ; i < MaxBalls ; i++)
            {
                if (balls_[i].present_)
                    cnt++ ;
            }

            return cnt ;
        }

        void ConveyorModel::deleteBallFromIntake()
        {
            int i = 0 ; 

            while (i < MaxBalls - 1)
            {
                balls_[i] = balls_[i + 1] ;
                i++ ;
            }
            balls_[MaxBalls - 1].present_ = false ;
        }

        void ConveyorModel::deleteBallFromShooter()
        {
            int i = MaxBalls - 1 ;

            while (i >= 0)
            {
                if (balls_[i].present_)
                {
                    if (balls_[i].position_ > 31.0)
                    {
                        balls_[i].present_ = false ;
                    }
                    break ;
                }
            }
        }

        void ConveyorModel::insertBallFromIntake(bool drop)
        {
            if (countBalls() < MaxBalls)
            {
                int i = MaxBalls - 1 ;
                do
                {
                    balls_[i] = balls_[i - 1] ;

                } while (--i > 0) ;

                balls_[0].present_ = true ;
                balls_[0].position_ = -3.25 ;
                balls_[0].drop_ = drop ;
            }                
        }

        void ConveyorModel::printBalls()
        {
            SimulatorMessages &msg = getEngine().getMessageOutput() ;
            double beltpower = belt_->get() ;
            double turretpower = turret_->get() ;

            msg.startMessage(SimulatorMessages::MessageType::Info) ;
            msg << "Conveyor [" << countBalls() << "]:" ;
            for(int i = 0 ; i < MaxBalls ; i++)
            {
                if (balls_[i].present_)
                    msg << " (*)" ;
                else
                    msg << " ( )" ;
                    
                msg << balls_[i].position_ ;
            }
            msg << " belt: " << beltpower ;
            msg << " turret: " << turretpower ;
            msg.endMessage(getEngine().getSimulationTime()) ;   
        }
    }
}
