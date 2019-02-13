#include "LineFollowAction.h"
#include "lightsensor/LightSensorSubsystem.h"
#include "Robot.h"

using namespace xero::misc ;

namespace xero {
    namespace base {
        std::list<std::string> LineFollowAction::plot_columns_ = {
            "time", 
            "angle", "left", "right",
            "distance"
        } ;

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
            if (getTankDrive().hasGearShifter())
                getTankDrive().lowGear() ;                
            start_distance_ = getTankDrive().getDist() ;
            is_done_ = false ;
            plotid_ = getTankDrive().getRobot().startPlot(toString(), plot_columns_) ;
            start_time_ = getTankDrive().getRobot().getTime() ;
            index_ = 0 ;
        }

        void LineFollowAction::run() {
            MessageLogger &logger = ls_subsystem_.getRobot().getMessageLogger() ;                

            Robot &rb = getTankDrive().getRobot() ;        

            if (!is_done_) {
                bool is_detected = ls_subsystem_.detectedObject() ;                
                if(is_detected == true){

                    double guidance_angle = ls_subsystem_.guidanceAngle() ;
                    double left_m = power_ + guidance_angle*power_adjust_ ;
                    double right_m = power_ - guidance_angle*power_adjust_ ;

                    setMotorsToPercents(left_m, right_m) ;

                    is_done_ = getTankDrive().getDist() - start_distance_ >= distance_ ;

                    logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_LINE_FOLLOWER) ;
                    logger << "LineFollowAction:" ;
                    logger << " sensor_values " << ls_subsystem_.getSensorsState() ;
                    logger << " guidance_angle " << guidance_angle ;
                    logger << " left " << left_m ;
                    logger << " right " << right_m ;
                    logger << " dist " << getTankDrive().getDist() - start_distance_ ;
                    logger << " target " << distance_ ;
                    logger.endMessage() ;


                    rb.addPlotData(plotid_, index_, 0, rb.getTime() - start_time_) ;

                    // Left side
                    rb.addPlotData(plotid_, index_, 1, guidance_angle) ;
                    rb.addPlotData(plotid_, index_, 2, left_m) ;
                    rb.addPlotData(plotid_, index_, 3, right_m) ;
                    rb.addPlotData(plotid_, index_, 4, getTankDrive().getDist() - start_distance_) ;

                    if (is_done_)
                        rb.endPlot(plotid_) ;
                }
            }
            else {
                setMotorsToPercents(0, 0) ;
            }
            index_++ ;
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
