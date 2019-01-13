#include "PhoenixRobotSubsystem.h"
#include "liftingcollector/LiftingCollector.h"
#include "wings/Wings.h"
#include "phoenixoi/PhoenixOISubsystem.h"
#include "PhoenixRobotSubsystemAction.h"
#include "phoenixgroups.h"
#include <tankdrive/TankDrive.h>
#include <Robot.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace phoenix {
        PhoenixRobotSubsystem::PhoenixRobotSubsystem(Robot &robot) : Subsystem(robot, "PhoenixRobotSubsystem") {
            auto &settings = robot.getSettingsParser() ;
            auto &logger = robot.getMessageLogger() ;
            std::list<int> left, right ;
            int index = 1 ;

            //
            // Create the wings subsystem
            //
            wings_ = std::make_shared<Wings>(robot) ;
            addChild(wings_) ;

            //
            // Create the lifting collector subsystem
            //
            lifting_collector_ = std::make_shared<LiftingCollector>(robot) ;
            addChild(lifting_collector_) ;
            lifting_collector_->createNamedSequences() ;

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

                addChild(db_) ;
            }

            //
            // Create the Phoenix OI subsystem.  It is created last as it may
            // refer to the other subsystems
            //
            oi_ = std::make_shared<PhoenixOISubsystem>(robot) ;
            addChild(oi_) ;         
        }

        PhoenixRobotSubsystem::~PhoenixRobotSubsystem() {
            oi_ = nullptr ;
            wings_ = nullptr ;
            lifting_collector_ = nullptr ;
            db_ = nullptr ;
        }

        bool PhoenixRobotSubsystem::canAcceptAction(xero::base::ActionPtr action) {
            auto ptr = std::dynamic_pointer_cast<PhoenixRobotSubsystemAction>(action) ;
            return ptr != nullptr ;
        }
    }
}
