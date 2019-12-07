#include "TankDrive.h"
#include <tankdrive/actions/TankDriveAction.h>
#include "Robot.h"
#include "LoopType.h"
#include <frc/smartdashboard/SmartDashboard.h>
#include <cassert>
#include <cmath>
#include "motors/CTREMotorController.h"
#include "motors/SparkMaxMotorController.h"
#include "motors/MotorFactory.h"

using namespace xero::misc ;

namespace xero {
    namespace base {

        TankDrive::TankDrive(Subsystem *parent, const std::string &motorConfigBase) : 
                        DriveBase(parent, "tankdrive"), angular_(2, true), left_linear_(2), right_linear_(2) {
            
            auto &robot = getRobot();
            auto motorFactory = robot.getMotorFactory();
            left_motors_ = motorFactory->createMotor(motorConfigBase + ":left");
            right_motors_ = motorFactory->createMotor(motorConfigBase + ":right");

            dist_l_ = 0.0 ;
            dist_r_ = 0.0 ;
            last_dist_l_ = 0.0 ;
            last_dist_r_ = 0.0 ;

            navx_ = std::make_shared<AHRS>(frc::SPI::Port::kMXP) ; 
            if (!navx_->IsConnected()) {  
                auto &logger = getRobot().getMessageLogger() ;
                logger.startMessage(MessageLogger::MessageType::error) ;
                logger << "NavX is not connected - cannot perform tankdrive auto functions" ;
                logger .endMessage() ;
                // navx_ = nullptr ;
            }
            else {
                navx_->Reset() ;
            }   

            SettingsParser &settings = robot.getSettingsParser();
            double width = settings.getDouble("tankdrive:width") ;
            double scrub = settings.getDouble("tankdrive:scrub") ;
            kin_ = std::make_shared<xero::misc::Kinematics>(width, scrub) ;
        }

        TankDrive::~TankDrive() {   
            setMotorsToPercents(0, 0);   // Turn motors off     
        }

        void TankDrive::reset() {
            Subsystem::reset() ;

            left_motors_->setNeutralMode(MotorController::NeutralMode::Coast);
            right_motors_->setNeutralMode(MotorController::NeutralMode::Coast);
            setMotorsToPercents(0, 0);   // Turn motors off
        }

        void TankDrive::init(LoopType ltype) {
            Subsystem::init(ltype) ;

            if (ltype == LoopType::Autonomous)
            {
                left_motors_->setNeutralMode(MotorController::NeutralMode::Brake);
                right_motors_->setNeutralMode(MotorController::NeutralMode::Brake);
            }
            else
            {
                left_motors_->setNeutralMode(MotorController::NeutralMode::Coast);
                right_motors_->setNeutralMode(MotorController::NeutralMode::Coast);                
            }
        }
        
        void TankDrive::lowGear() {
            if (gear_ != nullptr) {
                gear_->Set(true) ;
                auto &logger = getRobot().getMessageLogger() ;
                logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_TANKDRIVE) ;
                logger << "Shifted tankdrive to low gear" ;
                logger.endMessage() ;    
                frc::SmartDashboard::PutBoolean("HighGear", false) ;
            } else {
                auto &logger = getRobot().getMessageLogger() ;
                logger.startMessage(MessageLogger::MessageType::warning) ;
                logger.endMessage() ;
            }
        }

        void TankDrive::highGear() {
            if (gear_ != nullptr) {
                gear_->Set(false) ;
                auto &logger = getRobot().getMessageLogger() ;
                logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_TANKDRIVE) ;
                logger << "Shifted tankdrive to high gear" ;
                logger.endMessage() ;  
                frc::SmartDashboard::PutBoolean("HighGear", true) ;
            } else {
                auto &logger = getRobot().getMessageLogger() ;
                logger.startMessage(MessageLogger::MessageType::warning) ;
                logger.endMessage() ;               
            }
        }       

        bool TankDrive::canAcceptAction(ActionPtr ptr) {
            auto act = std::dynamic_pointer_cast<TankDriveAction>(ptr) ;
            return act != nullptr ;
        }

        void TankDrive::run() {
            Subsystem::run() ;
        }

        void TankDrive::setEncoders(int l1, int l2, int r1, int r2) {
            left_enc_ = std::make_shared<frc::Encoder>(l1, l2) ;
            right_enc_ = std::make_shared<frc::Encoder>(r1, r2) ;

            SettingsParser &parser = getRobot().getSettingsParser() ;
            if (parser.isDefined("tankdrive:inches_per_tick")) {
                left_inches_per_tick_ = getRobot().getSettingsParser().getDouble("tankdrive:inches_per_tick") ;
                right_inches_per_tick_ = left_inches_per_tick_ ;
            }
            else {
                left_inches_per_tick_ = getRobot().getSettingsParser().getDouble("tankdrive:left_inches_per_tick") ;
                right_inches_per_tick_ = getRobot().getSettingsParser().getDouble("tankdrive:right_inches_per_tick") ;                
            }

            left_enc_->Reset() ;
            right_enc_->Reset() ;
        }

        void TankDrive::setGearShifter(int index) {
            gear_ = std::make_shared<frc::Solenoid>(index) ;
            highGear() ;
        }

        void TankDrive::invertLeftMotors() {
            left_motors_->setInverted(true);
        }

        void TankDrive::invertRightMotors() {
            right_motors_->setInverted(true);
        }       

        void TankDrive::computeState() {
            double angle = 0.0 ;

            if (left_enc_ != nullptr) {
                assert(right_enc_ != nullptr) ;

                ticks_left_ = left_enc_->Get() ;
                ticks_right_ = right_enc_->Get() ;

                dist_l_ = ticks_left_ * left_inches_per_tick_ ;
                dist_r_ = ticks_right_ * right_inches_per_tick_ ;
            }

            if (navx_ != nullptr) {
                angle = -navx_->GetYaw() ;
                angular_.update(getRobot().getDeltaTime(), angle) ;
                total_angle_ = navx_->GetAngle() ;    
            }

            left_linear_.update(getRobot().getDeltaTime(), getLeftDistance()) ;
            right_linear_.update(getRobot().getDeltaTime(), getRightDistance()) ;

            frc::SmartDashboard::PutNumber("dbleft", left_linear_.getDistance()) ;
            frc::SmartDashboard::PutNumber("dbright", right_linear_.getDistance()) ;
            frc::SmartDashboard::PutNumber("dbyaw", getAngle()) ;

            auto &logger = getRobot().getMessageLogger() ;
            logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_TANKDRIVE_VERBOSE);
            logger << "TankDrive: " ;
            logger << " linear dist " << getDist() ;
            logger << ", velocity " << getVelocity() ;
            logger << ", accel " << getAcceleration() ;
            logger << ", angle dist " << getAngle() ;
            logger << ", velocity " << getAngularVelocity() ;
            logger << ", accel " << getAngularAcceleration() ;
            logger << ", ticks " << ticks_left_ << " " << ticks_right_ ;
            logger << ", dist " << dist_l_ << " " << dist_r_ ;
            logger.endMessage();    

            kin_->move(dist_r_ - last_dist_r_, dist_l_ - last_dist_l_, angle) ;

            last_dist_l_ = dist_l_ ;
            last_dist_r_ = dist_r_ ;

#ifdef USE_NAVX
            if (navx_ != nullptr) {
                double vx = navx_->GetVelocityX() ;
                double vy = navx_->GetVelocityY() ;
                double vz = navx_->GetVelocityZ() ;
                xyz_velocity_ = std::sqrt(vx * vx + vy * vy + vz * vz) * 39.3701 ; 
            }
            else {
                xyz_velocity_ = 0.0 ;
            }
#else
            xyz_velocity_ = 0.0 ;
#endif
        }

        void TankDrive::setMotorsToPercents(double left_percent, double right_percent) {
            left_motors_->set(left_percent);
            right_motors_->set(right_percent);
        }
    }
}
