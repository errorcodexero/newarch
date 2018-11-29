#include "SorterDutyCycleAction.h"
#include "Robot.h"

using namespace xero::base;
using namespace xero::misc ;

namespace xero {
    namespace bunny2018 {
            SorterDutyCycleAction::SorterDutyCycleAction(Sorter &sorter, Which w, double v) : SorterAction(sorter) {
                duty_cycle_ = v ;
				which_ = w ;
            }

            SorterDutyCycleAction::SorterDutyCycleAction(Sorter &sorter, Which w, const std::string &name) : SorterAction(sorter) {
                duty_cycle_ = sorter.getRobot().getSettingsParser().getDouble(name) ;
				which_ = w ;
            }

            SorterDutyCycleAction::~SorterDutyCycleAction() {

            }
            void SorterDutyCycleAction::start() {
                switch(which_) {
                case Which::IntakeMotor:
                    getSubsystem().setIntakeMotor(duty_cycle_) ;
                    break ;

                case Which::OuttakeMotor:
                    getSubsystem().setOuttakeMotor(duty_cycle_) ;                
                    break ;

                case Which::SortMotor:
                    getSubsystem().setSorterMotor(duty_cycle_) ;
                    break ;
            
                default:
                    {
                        auto &logger = getSubsystem().getRobot().getMessageLogger() ;
                        logger.startMessage(MessageLogger::MessageType::error) ;
                        logger << "Invalid motor type given in SorterDutyCycleAction" ;
                        logger.endMessage() ;
                    }
                }
            }

            void SorterDutyCycleAction::run() {
            }

            bool SorterDutyCycleAction::isDone() {
                return true ;
            }

            void SorterDutyCycleAction::cancel() {
            }

            std::string SorterDutyCycleAction::toString() {
                std::string result = "SorterDutyCycleAction " ;
                result += std::to_string(duty_cycle_) ;
                return result ;
            }
    }
}
