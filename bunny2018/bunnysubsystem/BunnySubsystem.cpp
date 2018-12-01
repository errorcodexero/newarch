#include "BunnySubsystem.h"
#include "bunnyoi/BunnyOISubsystem.h"
#include <Robot.h>
#include <list>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace bunny2018 {
        BunnySubsystem::BunnySubsystem(Robot &robot) : Subsystem(robot, "bunny2018") {
            auto &settings = robot.getSettingsParser() ;
            auto &logger = robot.getMessageLogger() ;
            std::list<int> left, right ;
            bool dberror = false ;
            int index = 1 ;

            // Register message listener *before* any sub-system like the drive base which needs vision information.
            // The vision info received will only be present during 1 robot loop so ML's computeState()
            // must be called before other dependent sub-systems call their computeState() to access vision results.
            int socket_port_number = settings.getInteger("messagelistener:port") ;
            ml_ = std::make_shared<MessageListener>(robot, socket_port_number) ;
            addChild(ml_) ;

            if (!settings.isDefined("hw:tankdrive:disable")) {

                while (true) {
                    std::string motstr = "hw:tankdrive:leftmotor" + std::to_string(index) ;
                    if (!settings.isDefined(motstr))
                        break ;
                    left.push_back(settings.getInteger(motstr)) ;

                    motstr = "hw:tankdrive:rightmotor" + std::to_string(index) ;
                    if (!settings.isDefined(motstr))
                        break ;
                    right.push_back(settings.getInteger(motstr)) ;
                    index++;
                }

                if (left.size() != right.size()) {
                    logger.startMessage(MessageLogger::MessageType::error) ;
                    logger << "differ motor count for drive base on left and right sides" ;
                    logger << ", left " << left.size() ;
                    logger << ", right " << right.size() ;
                    logger.endMessage() ;
                    dberror = true ;
                }
                
                db_ = std::make_shared<TankDrive>(robot, left, right) ;

                if (settings.isDefined("hw:tankdrive:leftencoder1") && settings.isDefined("hw:tankdrive:leftencoder2") &&
                    settings.isDefined("hw:tankdrive:rightencoder1") && settings.isDefined("hw:tankdrive:rightencoder2")) {
                    
                    int l1 = settings.getInteger("hw:tankdrive:leftencoder1") ;
                    int l2 = settings.getInteger("hw:tankdrive:leftencoder2") ;
                    int r1 = settings.getInteger("hw:tankdrive:rightencoder1") ;
                    int r2 = settings.getInteger("hw:tankdrive:rightencoder2") ;

                    db_->setEncoders(l1, l2, r1, r2) ;
                }

                if (settings.isDefined("hw:tankdrive:invertleft")) {
                    db_->invertLeftMotors() ;
                    db_->invertLeftEncoder() ;
                }
                else if (settings.isDefined("hw:tankdrive:invertright")) {
                    db_->invertRightMotors() ;
                    db_->invertRightEncoder() ;                
                }

                db_->setEncoders(0, 1, 2, 3) ;

                addChild(db_) ;
            }

            oi_ = std::make_shared<BunnyOISubsystem>(robot) ;
            addChild(oi_) ;

            if (!settings.isDefined("hw:collector:disabled")) {
                collector_ = std::make_shared<xero::base::SingleMotorSubsystem>(robot, "Collector", "hw:collector:motor") ;
                addChild(collector_) ;
            }

            if (!settings.isDefined("hw:hopper:disabled")) {
                hopper_ = std::make_shared<xero::base::SingleMotorSubsystem>(robot, "Hopper", "hw:hopper:motor") ;
                addChild(hopper_) ;
            }

            if (!settings.isDefined("hw:sorter:disabled")) {
                sorter_ = std::make_shared<Sorter>(robot) ;
                addChild(sorter_) ;     
            }
        }

        BunnySubsystem::~BunnySubsystem() {
        }
    }
}
