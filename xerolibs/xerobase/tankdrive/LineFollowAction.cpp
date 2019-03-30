#include "LineFollowAction.h"
#include "lightsensor/LightSensorSubsystem.h"
#include "Robot.h"

using namespace xero::misc ;

namespace xero {
    namespace base {
        LineFollowAction::LineFollowAction(LightSensorSubsystem &ls_subsystem, TankDrive &db_subsystem, double power, double distance, double power_adjust) : TankDriveAction(db_subsystem), ls_subsystem_(ls_subsystem), distance_(distance), power_(power), power_adjust_(power_adjust)  {
            stalled_threshold_ = db_subsystem.getRobot().getSettingsParser().getDouble("linefollower:stalled_threshold") ;
        }

        LineFollowAction::LineFollowAction(LightSensorSubsystem &ls_subsystem, TankDrive &db_subsystem, const std::string &power_name, const std::string &distance_name, const std::string &power_adjust_name) 
                        : TankDriveAction(db_subsystem), ls_subsystem_(ls_subsystem) {
            SettingsParser settings_parser = ls_subsystem.getRobot().getSettingsParser() ;
            power_ = settings_parser.getDouble(power_name) ;
            distance_ = settings_parser.getDouble(distance_name) ;
            power_adjust_ = settings_parser.getDouble(power_adjust_name) ;
            stalled_threshold_ = db_subsystem.getRobot().getSettingsParser().getDouble("linefollower:stalled_threshold") ;            
        }

        LineFollowAction::~LineFollowAction() {
        }

        void LineFollowAction::start() {
            start_distance_ = getTankDrive().getDist() ;
            is_done_ = false ;

            MessageLogger &logger = ls_subsystem_.getRobot().getMessageLogger() ;
            logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_LINE_FOLLOWER) ;            
            logger << "LineFollowAction: " ;
            logger << "start distance " << start_distance_ ;
            logger.endMessage() ;            
        }

        void LineFollowAction::run() {
            MessageLogger &logger = ls_subsystem_.getRobot().getMessageLogger() ;

            distances_.push_front(getTankDrive().getDist()) ;
            if (distances_.size() > 16)
                distances_.pop_back() ;

            if (!is_done_) {
                double traveled = distances_.front() - distances_.back() ;
                double linetraveled = distances_.front() - start_distance_ ;
                if (distances_.size() == 16 && std::fabs(traveled) < stalled_threshold_) {
                    //
                    // Case 1: the drivebase has stalled, must have hit the target
                    //
                    logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_LINE_FOLLOWER) ;
                    logger << "LineFollowAction:" ;
                    logger << " subsystem " << ls_subsystem_.getName() ;  
                    logger << " - action terminates because robot is not moving" ;
                    logger.endMessage() ;                  
                    is_done_ = true ;
                }
                else if (std::fabs(linetraveled) > distance_) {
                    //
                    // Case 2: We have traveled the desired distance after hitting the
                    //         line the first time.
                    //
                    is_done_ = true ;
                    logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_LINE_FOLLOWER) ;
                    logger << "LineFollowAction:" ;
                    logger << " subsystem " << ls_subsystem_.getName() ;  
                    logger << " - distance " ;
                    logger << distance_ << " reached" ;
                    logger.endMessage() ;                        
                } 
                else if (ls_subsystem_.detectedObject()) {
                    //
                    // Case 3: We are still following the line
                    //
                    double left = power_ ;
                    double right = power_ ;

                    uint32_t st = ls_subsystem_.getSensorsState() ;
                    switch(st) {
                        case 0:
                            break ;
                        case 1:
                            left -= power_adjust_ ;
                            right += power_adjust_ ;
                            break ;
                        case 2:
                            left += power_adjust_ / 2.0 ;
                            right -= power_adjust_ / 2.0 ;
                            break ;
                        case 3:
                            break ;                                
                        case 4:
                            left += power_adjust_ ;
                            right -= power_adjust_ ;    
                            break ;                            
                        case 5:
                            break ;   
                        case 6:
                            break ;                              
                        case 7:
                            break ;
                    }
                    setMotorsToPercents(left, right) ;

                    logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_LINE_FOLLOWER) ;
                    logger << "LineFollowAction:" ;
                    logger << " line detected " << ls_subsystem_.getName() ;  
                    logger << " distance " << linetraveled ;
                    logger << " left " << left ;
                    logger << " right " << right ;
                    logger.endMessage() ;

                }
                else {
                    //
                    // Case 4: We have lost the target, keep driving just in case we hit it again.  We
                    //         will stop after the distance is covered or if we hit something
                    //
                }
            }
            else {
                setMotorsToPercents(0, 0) ;
            }
        }

        bool LineFollowAction::isDone() {
            return is_done_ ;
        }

        void LineFollowAction::cancel() {
            is_done_ = true ;
            setMotorsToPercents(0, 0) ;
        }

        std::string LineFollowAction::toString() {
            std::string result = "LineFollowAction " ;
            result += std::to_string(power_) ;
            return result ;
        }
    }
}
