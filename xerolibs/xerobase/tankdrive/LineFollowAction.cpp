#include "LineFollowAction.h"
#include "lightsensor/LightSensorSubsystem.h"
#include "Robot.h"

using namespace xero::misc ;

namespace xero {
    namespace base {
            LineFollowAction::LineFollowAction(LightSensorSubsystem &ls_subsystem, TankDrive &db_subsystem, double power, double distance, double power_adjust) : TankDriveAction(db_subsystem), ls_subsystem_(ls_subsystem), distance_(distance), power_(power), power_adjust_(power_adjust)  {
             


            }

            LineFollowAction::LineFollowAction(LightSensorSubsystem &ls_subsystem, TankDrive &db_subsystem, const std::string &power_name, const std::string &distance_name, const std::string &power_adjust_name) : TankDriveAction(db_subsystem), ls_subsystem_(ls_subsystem) {
                
                SettingsParser settings_parser = ls_subsystem.getRobot().getSettingsParser() ;
                power_ = settings_parser.getDouble(power_name) ;
                distance_ = settings_parser.getDouble(distance_name) ;
                power_adjust_ = settings_parser.getDouble(power_adjust_name) ;

            }

            LineFollowAction::~LineFollowAction() {
            }

            void LineFollowAction::start() {
                
                start_distance_ = getTankDrive().getDist() ;
                is_done_ = false ;

            }

            void LineFollowAction::run() {
                bool is_detected = ls_subsystem_.detectedObject() ;

                std::vector<bool> sensor_data_ = ls_subsystem_.sensorData() ;

                getTankDrive().getRobot().getMessageLogger().startMessage(MessageLogger::MessageType::debug, MSG_GROUP_TANKDRIVE) ;
                getTankDrive().getRobot().getMessageLogger() << "light sensor values"<<std::to_string(sensor_data_[0])<<" "<< std::to_string(sensor_data_[1]) <<" "<< std::to_string(sensor_data_[2])<<" "<< std::to_string(is_detected)  ;
                getTankDrive().getRobot().getMessageLogger().endMessage() ;

                if(is_detected == true){

                    double guidance_angle = ls_subsystem_.guidanceAngle() ;
                    double left_m = power_ + guidance_angle*power_adjust_ ;
                    double right_m = power_ - guidance_angle*power_adjust_ ;

                    getTankDrive().setMotorsToPercents(left_m, right_m) ;

                    is_done_ = getTankDrive().getDist() - start_distance_ >= distance_ ;


                }
                else{
                    is_done_ = true ;
                    MessageLogger logger = ls_subsystem_.getRobot().getMessageLogger() ;
                    logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_TANKDRIVE) ;
                    logger<<"LineFollowAction:no line was detected" ;
                    logger.endMessage() ;
                }

                if(is_done_ == true){
                    getTankDrive().setMotorsToPercents(0, 0) ;
                }
                
            }

            bool LineFollowAction::isDone() {
                return is_done_ ;
            }

            void LineFollowAction::cancel() {
            }

            std::string LineFollowAction::toString() {
                std::string result = "LineFollowAction " ;
                result += std::to_string(power_) ;
                return result ;
            }
    }
}
