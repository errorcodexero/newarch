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
                    insertBallFromIntake() ;
                }
            }
            else if (name == "print")
            {
                if (value.isBoolean() && value.getBoolean())
                {
                    printBalls() ;
                }                
            }
        }

        bool ConveyorModel::create()
        {
            belt_ = std::make_shared<SimulatedMotor>(*this, "hw:belt") ;
            turret_ = std::make_shared<SimulatedMotor>(*this, "hw:turret") ;

            sensor1_ = getInteger("hw:sensor1") ;
            sensor2_ = getInteger("hw:sensor2") ;
            sensor3_ = getInteger("hw:sensor3") ;

            HALSIM_SetDIOValue(sensor1_, false) ;
            HALSIM_SetDIOValue(sensor2_, false) ;
            HALSIM_SetDIOValue(sensor3_, false) ;     

            for(int i = 0 ; i < MaxBalls ; i++)
                balls_[i].present_ = false ;

            dist_per_second_per_volt_ = getDouble("distance_per_second_per_inch") ;

            return true ;
        }

        void ConveyorModel::run(uint64_t microdt) 
        {
            double tm = microdt / 1.0e6 ;
            double dist = belt_->get() * dist_per_second_per_volt_ * tm ;

            if (dist > 0)
            {
                for(int i = MaxBalls - 1 ; i >= 0 ; i--)
                {
                    if (balls_[i].present_)
                    {
                        double newloc = balls_[i].position_ + dist ;

                        if (i == MaxBalls - 1)
                        {
                            if (newloc > 31.5)
                                deleteBallFromShooter();
                        }
                        else
                        {
                            if (balls_[i + 1].present_ && newloc > balls_[i + 1].position_ - BallDiameter / 2.0)
                                newloc =- balls_[i + 1].position_ - BallDiameter / 2.0 ;
                        }

                        balls_[i].position_ = newloc ;
                    }
                }
            }
            else
            {
                for(int i = 0 ; i < MaxBalls ; i++)
                {
                    if (balls_[i].present_)
                    {
                        double newloc = balls_[i].position_ + dist ;

                        if (i == 0)
                        {
                            if (newloc < -5.0)
                            {
                                deleteBallFromIntake() ;
                                break ;
                            }
                        }
                        else
                        {
                            if (balls_[i - 1].present_ && newloc < balls_[i - 1].position_ + BallDiameter / 2.0)
                                newloc = balls_[i - 1].position_ + BallDiameter / 2.0 ;
                        }

                        balls_[i].position_ = newloc ;
                    }
                }
            }

            // Update sensors
            HALSIM_SetDIOValue(sensor1_, false) ;
            HALSIM_SetDIOValue(sensor2_, false) ;
            HALSIM_SetDIOValue(sensor3_, false) ;
            for (int i = 0; i < MaxBalls; i++) {
                if (!balls_[i].present_) continue;
                double pos = balls_[i].position_;

                if (pos < -4.0)
                    HALSIM_SetDIOValue(sensor1_, true) ;

                if (pos > 1.0 && pos < 3.0)
                    HALSIM_SetDIOValue(sensor2_, true) ;

                if (pos > 29.0 && pos < 31.0)
                    HALSIM_SetDIOValue(sensor3_, true) ;
            }
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

        void ConveyorModel::insertBallFromIntake()
        {

            if (countBalls() < MaxBalls)
            {
                int i = MaxBalls - 1 ;
                do
                {
                    balls_[i] = balls_[i - 1] ;
                } while (--i > 0) ;

                balls_[0].present_ = true ;
                balls_[0].position_ = -5.0 ;
            }                
        }

        void ConveyorModel::printBalls()
        {
            SimulatorMessages &msg = getEngine().getMessageOutput() ;
            double beltSpeed = belt_->get();
            msg.startMessage(SimulatorMessages::MessageType::Info) ;
            msg << "Conveyor [" << countBalls() << "]:" ;
            for(int i = 0 ; i < MaxBalls - 1 ; i++)
            {
                if (balls_[i].present_)
                    msg << " (*)" ;
                else
                    msg << " ( )" ;
                    
                msg << balls_[i].position_ ;
            }
            msg << " belt: " << beltSpeed;
            msg.endMessage(getEngine().getSimulationTime()) ;   
        }
    }
}
