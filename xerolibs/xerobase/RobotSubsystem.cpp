#include "RobotSubsystem.h"
#include "tankdrive/TankDrive.h"

using namespace xero::misc ;

namespace xero {
    namespace base {
        RobotSubsystem::RobotSubsystem(xero::base::Robot &robot, const std::string &name) : Subsystem(robot, name) {
        }

        RobotSubsystem::~RobotSubsystem() {

        }

        bool RobotSubsystem::isDefinedAndTrue(const std::string &name) {
            auto &settings = getRobot().getSettingsParser() ;
            if (!settings.isDefined(name))
                return false ;

            if (settings.getBoolean(name) == false)
                return false ;

            return true ;
        }

        void RobotSubsystem::addTankDrive() {
            //
            // TODO - Most subsystems part the queries file themselves.  This is the
            //        only place where a parent does the query.  Move this code down into
            //        the TankDrive subsystem.
            //

            auto &settings = getRobot().getSettingsParser() ;
            auto &logger = getRobot().getMessageLogger() ;

            const char *l1str = "hw:tankdrive:encoders:left:1" ;
            const char *l2str = "hw:tankdrive:encoders:left:2" ;
            const char *r1str = "hw:tankdrive:encoders:right:1" ;
            const char *r2str = "hw:tankdrive:encoders:right:2" ;
            
            auto tank = std::make_shared<TankDrive>(this, "hw:tankdrive:motors") ;

            if (settings.isDefined(l1str) && settings.isDefined(l2str) && settings.isDefined(r1str) && settings.isDefined(r2str)) {                
                int l1 = settings.getInteger(l1str) ;
                int l2 = settings.getInteger(l2str) ;
                int r1 = settings.getInteger(r1str) ;
                int r2 = settings.getInteger(r2str) ;

                tank->setEncoders(l1, l2, r1, r2) ;
                
            } else {
                logger.startMessage(MessageLogger::MessageType::error) ;
                logger << "encoders not found in robot data file" ;
                logger.endMessage() ;
                db_ = nullptr ;         
            }

            //
            // Check for a solenoid for a shifter
            //
            if (settings.isDefined("hw:tankdrive:shifter")) {
                int sol = settings.getInteger("hw:tankdrive:shifter") ;
                tank->setGearShifter(sol) ;
            }

            //
            // Depending on how the robot is wired, these reverse the motor and/or the
            // encoders such that a positive power on the robot moves the robot forward, and
            // forward motion on the robot results in positive encoder values.
            //
            if (isDefinedAndTrue("hw:tankdrive:invert:motors:left")) {
                tank->invertLeftMotors() ;
            }

            if (isDefinedAndTrue("hw:tankdrive:invert:motors:right")) {
                tank->invertRightMotors() ;
            }

            db_ = tank ;
            addChild(db_) ;
        }
    }
}
