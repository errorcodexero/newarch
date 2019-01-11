#include "TankDrive.h"
#include "TankDriveAction.h"
#include "Robot.h"
#include "LoopType.h"
#include <frc/smartdashboard/SmartDashboard.h>
#include <cassert>
#include <cmath>

#ifdef SIMULATOR
#include <SensorCollection.h>
#else
#include <ctre/phoenix/MotorControl/SensorCollection.h>
#endif

using namespace xero::misc ;

namespace xero {
    namespace base {

        TankDrive::TankDrive(Robot& robot, const std::list<int> &left_motor_ids, const std::list<int> &right_motor_ids) : 
                        DriveBase(robot, "tankdrive"), angular_(4, true), left_linear_(4), right_linear_(4) {
            //The two sides should always have the same number of motors and at least one motor each
            assert((left_motor_ids.size() == right_motor_ids.size()) && (left_motor_ids.size() > 0));

            SettingsParser &settings = robot.getSettingsParser() ;
            if (settings.isDefined("hw:tankdrive:motortype") && settings.getString("hw:tankdrive:motortype") == "victor")
            {
                initVictorList(left_motor_ids, left_victor_motors_) ;
                initVictorList(right_motor_ids, right_victor_motors_) ;
            }
            else
            {
                initTalonList(left_motor_ids, left_talon_motors_);
                initTalonList(right_motor_ids, right_talon_motors_);
            }

            dist_l_ = 0.0 ;
            dist_r_ = 0.0 ;
            dumpstate_ = false ;

#ifdef GOPIGO
            navx_ = new AHRS(frc::SerialPort::Port::Port_0) ;
#else
            navx_ = new AHRS(frc::SPI::Port::kMXP) ;        
#endif

            if (!navx_->IsConnected()) {
                auto &logger = getRobot().getMessageLogger() ;
                logger.startMessage(MessageLogger::MessageType::error) ;
                logger << "NavX is not connected - cannot perform tankdrive auto functions" ;
                logger .endMessage() ;
                navx_ = nullptr ;
            }
        }

        TankDrive::~TankDrive() {   
            setMotorsToPercents(0, 0);   // Turn motors off     
        }

        void TankDrive::reset() {
            Subsystem::reset() ;

            if (left_talon_motors_.size()) {
                for(auto &talon : left_talon_motors_) {
                    talon->SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Coast) ;
                }

                for(auto &talon : right_talon_motors_) {
                    talon->SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Coast) ;
                }           
            }
            setMotorsToPercents(0, 0);   // Turn motors off
        }

        void TankDrive::init(LoopType ltype) {
            Subsystem::init(ltype) ;

            if (left_talon_motors_.size()) {
                for(auto &talon : left_talon_motors_) {
                    talon->SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake) ;
                }

                for(auto &talon : right_talon_motors_) {
                    talon->SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake) ;
                }           
            }
        }


        /// \brief set the drive base to low gear
        void TankDrive::lowGear() {
            if (gear_ != nullptr)
                gear_->Set(true) ;
            else {
                auto &logger = getRobot().getMessageLogger() ;
                logger.startMessage(MessageLogger::MessageType::warning) ;
                logger << "tankdrive: attempting to shift to low gear when the tank drive does not have a gear box" ;
                logger.endMessage() ;
            }
        }

        /// \brief set the drive base to high gear
        void TankDrive::highGear() {
            if (gear_ != nullptr)
                gear_->Set(false) ;
            else {
                auto &logger = getRobot().getMessageLogger() ;
                logger.startMessage(MessageLogger::MessageType::warning) ;
                logger << "tankdrive: attempting to shift to high gear when the tank drive does not have a gear box" ;
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

            inches_per_tick_ = getRobot().getSettingsParser().getDouble("tankdrive:inches_per_tick") ;

            left_enc_->Reset() ;
            right_enc_->Reset() ;
        }

        void TankDrive::setGearShifter(int index) {
            gear_ = std::make_shared<frc::Solenoid>(index) ;
        }

        void TankDrive::initTalonList(const std::list<int>& ids, std::list<TalonPtr>& talons) {
            //Assuming first id will be master
            for(int id : ids) {
                auto talon = std::make_shared<TalonSRX>(id) ;
                talon->ConfigVoltageCompSaturation(12.0, 10) ;
                talon->EnableVoltageCompensation(true) ;
                talons.push_back(talon);
                if(talons.size() > 1)
                    talons.back()->Follow(*talons.front());
            }
        }       

        void TankDrive::initVictorList(const std::list<int> &ids, std::list<VictorPtr> &victors) {
            for(int id : ids) {
                auto victor = std::make_shared<VictorSP>(id) ;
                victors.push_back(victor) ;
            }
        }

        void TankDrive::invertLeftMotors() {
            if (left_talon_motors_.size() > 0) {
                for(TalonPtr talon : left_talon_motors_)
                    talon->SetInverted(true);
            }
            else {
                for(VictorPtr victor : left_victor_motors_)
                    victor->SetInverted(true) ;
            }
        }

        void TankDrive::invertRightMotors() {
            if (right_talon_motors_.size() > 0) {
                for(TalonPtr talon : right_talon_motors_) 
                    talon->SetInverted(true);
            }
            else {
                for(VictorPtr victor : right_victor_motors_)
                    victor->SetInverted(true) ;
            }
        }       

        void TankDrive::computeState() {

            if (left_enc_ != nullptr) {
                assert(right_enc_ != nullptr) ;

                ticks_left_ = left_enc_->Get() ;
                ticks_right_ = right_enc_->Get() ;

                dist_l_ = ticks_left_ * inches_per_tick_ ;
                dist_r_ = ticks_right_ * inches_per_tick_ ;
            }

            if (navx_ != nullptr) {
                double angle = navx_->GetYaw() ;
                angular_.update(getRobot().getDeltaTime(), angle) ;
            }

            left_linear_.update(getRobot().getDeltaTime(), getLeftDistance()) ;
            right_linear_.update(getRobot().getDeltaTime(), getRightDistance()) ;

            auto &logger = getRobot().getMessageLogger() ;
            logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_TANKDRIVE_VERBOSE);
            logger << "time " << getRobot().getTime() ;
            logger << ", linear dist " << getDist() ;
            logger << ", velocity " << getVelocity() ;
            logger << ", accel " << getAcceleration() ;
            logger << ", angle dist " << getAngle() ;
            logger << ", velocity " << getAngularVelocity() ;
            logger << ", accel " << getAngularAcceleration() ;
            logger << ", ticks " << ticks_left_ << " " << ticks_right_ ;
            logger << ", dist " << dist_l_ << " " << dist_r_ ;
            logger.endMessage();    
        }

        void TankDrive::setMotorsToPercents(double left_percent, double right_percent) {
            if (left_talon_motors_.size() > 0) {
                left_talon_motors_.front()->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, left_percent);
                right_talon_motors_.front()->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, right_percent);
            }
            else {
                for(VictorPtr victor : left_victor_motors_)
                    victor->Set(left_percent) ;

                for(VictorPtr victor : right_victor_motors_)
                    victor->Set(right_percent) ;
            }
        }
    }
}
