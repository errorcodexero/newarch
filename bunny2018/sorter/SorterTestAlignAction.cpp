#include "SorterTestAlignAction.h"
#include "bunnyids.h"
#include <Robot.h>
#include <random>

std::random_device rd;
std::mt19937 mt(rd());
std::uniform_real_distribution<double> dist(0.5, 2.0);

using namespace xero::base;
using namespace xero::misc ;

namespace xero {
    namespace bunny2018 {
            SorterTestAlignAction::SorterTestAlignAction(Sorter &sorter, double delay, int desired) : SorterAlignCapableAction(sorter) {
				desired_ = desired ;
				duration_ = delay ;
				power_ = 0.3 ;
            }

            SorterTestAlignAction::~SorterTestAlignAction() {
            }

            void SorterTestAlignAction::start() {
				dir_ = true ;
				state_ = State::StartPertubate ;
				total_ = 0 ;
				is_done_ = false ;
            }

            void SorterTestAlignAction::run() {
				auto &logger = getSubsystem().getRobot().getMessageLogger() ;

				if (!is_done_) {
					switch(state_) {
					case State::Delay1:
						logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_SORTER) ;
						logger << "SorterTestAlignAction: Delay1" ;	
						logger << ", time " << getSubsystem().getRobot().getTime() ;
						logger << ", actual angle " << getSubsystem().getAngle() ;											
						logger.endMessage() ;

						if (getSubsystem().getRobot().getTime() - start_ > duration_)
							state_ = State::StartPertubate ;
						break ;

					case State::Delay2:
						logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_SORTER) ;
						logger << "SorterTestAlignAction: Delay2" ;	
						logger << ", time " << getSubsystem().getRobot().getTime() ;						
						logger << ", actual angle " << getSubsystem().getAngle() ;											
						logger.endMessage() ;

						if (getSubsystem().getRobot().getTime() - start_ > 1.5) {
							double target = calcTargetAngle() ;	
							setTargetAngle(target) ;													
							state_ = State::Align ;
						}
						break ;						

					case State::StartPertubate:
						logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_SORTER) ;
						logger << "SorterTestAlignAction: StartPertubate" ;
						logger << ", time " << getSubsystem().getRobot().getTime() ;						
						logger.endMessage() ;

						if (dir_) {
							getSubsystem().setSorterMotor(power_) ;							
						}
						else {
							getSubsystem().setSorterMotor(power_) ;									
						}
						start_ = getSubsystem().getRobot().getTime() ;
						dir_ = !dir_ ;
						state_ = State::Pertubate ;
						pertdur_ = dist(mt) ;
						break ;

					case State::Pertubate:
						logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_SORTER) ;
						logger << "SorterTestAlignAction: Pertubate" ;
						logger << ", time " << getSubsystem().getRobot().getTime() ;						
						logger << ", power " << power_ ;
						logger << ", duration " << pertdur_ ;
						logger << ", actual angle " << getSubsystem().getAngle() ;							
						logger.endMessage() ;

						if (getSubsystem().getRobot().getTime() - start_ > pertdur_) {
							getSubsystem().setSorterMotor(0.0) ;							

							start_ = getSubsystem().getRobot().getTime() ;							
							state_ = State::Delay2 ;
						}
						break ;

					case State::Align:
						logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_SORTER) ;
						logger << "SorterTestAlignAction: Align" ;
						logger << ", time " << getSubsystem().getRobot().getTime() ;						
						logger << ", target angle " << getTargetAngle() ;
						logger << ", actual angle " << getSubsystem().getAngle() ;											
						logger.endMessage() ;

						if (alignSorter()) {
							logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_SORTER) ;
							logger << "SorterTestAlignAction: complete" ;
							logger << ", time " << getSubsystem().getRobot().getTime() ;							
							logger << ", target angle " << getTargetAngle() ;
							logger << ", actual angle " << getSubsystem().getAngle() ;
							logger.endMessage() ;

							getSubsystem().setSorterMotor(0.0) ;

							start_ = getSubsystem().getRobot().getTime() ;
							state_ = State::Delay1 ;
						}
						break ;
					}
				}
            }

            bool SorterTestAlignAction::isDone() {
                return is_done_ ;
            }

            void SorterTestAlignAction::cancel() {
            }

            std::string SorterTestAlignAction::toString() {
                std::string result = "SorterTestAlignAction " ;
                return result ;
            }
    }
}
