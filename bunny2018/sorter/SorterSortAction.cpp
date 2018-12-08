#include "SorterSortAction.h"
#include "Robot.h"

using namespace xero::base;
using namespace xero::misc ;

namespace xero {
    namespace bunny2018 {
            SorterSortAction::SorterSortAction(Sorter &sorter, Sorter::BallColor color) : SorterAction(sorter), calc_(5,0.0) {
				auto & settings = sorter.getRobot().getSettingsParser() ;
                color_ = color ;
                state_ = State::Waiting ;
                ctrl_.initFromSettingsExtended(settings, "sorter.pid") ;
                reject_angle_ = settings.getDouble("sorter:reject_angle") ;
                keep_angle_ =settings.getDouble("sorter:keep_angle") ;
                tolerance_ = settings.getDouble("sorter:tolerance") ;
            }

            SorterSortAction::~SorterSortAction() {
            }

            void SorterSortAction::start() {
                state_ = State::Waiting ;
            }

            void SorterSortAction::run() {
                auto &sorter = getSubsystem() ;
                switch(state_){
                    case State::Waiting:
                        if(sorter.hasBall()){
                            if(sorter.getBallColor() == color_){
                                target_ = calc_.calcTargetAngle(sorter.getAngle()+keep_angle_) ;
                                state_ = State::GoToAngle ;
                            }
                            else{
                                target_ = calc_.calcTargetAngle(sorter.getAngle()+reject_angle_) ;
                                state_ = State::GoToAngle ;
                            }
                        }
                        break ;
                    case State::GoToAngle:
                        if(std::fabs(target_ - sorter.getAngle()) < tolerance_){
                            sorter.setSorterMotor(0) ;
                            state_ = State::Waiting ;
                        }
                        else{
                            double dt = sorter.getRobot().getDeltaTime() ;
                            double power = ctrl_.getOutput(target_, sorter.getAngle(), 0, dt) ;
                            sorter.setSorterMotor(power) ;
                        }
                        break ;
                }
            }

            bool SorterSortAction::isDone() {
                return true ;
            }

            void SorterSortAction::cancel() {
            }

            std::string SorterSortAction::toString() {
                std::string result = "SorterSortAction " ;
				if (color_ == Sorter::BallColor::Red)
					result += "red" ;
				else if (color_ == Sorter::BallColor::Blue)
					result += "blue" ;
				else
					result = "all" ;

                return result ;
            }
    }
}
