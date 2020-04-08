#pragma once

#include "motors/MotorController.h"
#include <Kinematics.h>
#include <DriveBase.h>
#include <Speedometer.h>
#include <frc/Solenoid.h>
#include <frc/Encoder.h>
#include <list>
#include <map>
#include <string>
#include <AHRS.h>       

/// \file


namespace xero {
    namespace base {
        /// \brief a tank-style drivebase
        class TankDrive : public DriveBase {
            /// \brief class for tank drive actions
            friend class TankDriveAction;      

        public:
            /// \brief Create a new tank drive object
            /// \param parent The parent subsystem for the drivebase
            /// \param motorConfigBase The configuration ID for the motors.
            /// If motorConfigBase is "hw:tankdrive", TankDrive will look for
            /// motor groups named "hw:tankdrive:left" and "hw:tankdrive:right".
            TankDrive(Subsystem *parent, const std::string &motorConfigBase);

            /// \brief destroy a tank drive object
            virtual ~TankDrive() ;

            /// \brief set the default motor mode for auto mode
            void setAutoNeutralMode(MotorController::NeutralMode m) {
                automode_neutral_ = m ;
            }

            /// \brief set the default motor mode for auto mode
            void setTeleopNeutralMode(MotorController::NeutralMode m) {
                teleop_neutral_ = m ;
            }        

            /// \brief set the default motor mode for auto mode
            void setResetNeutralMode(MotorController::NeutralMode m) {
                reset_neutral_ = m ;
            }                    

            /// \brief return the width of the robot
            /// \returns the width of the robot
            double getWidth() const {
                return kin_->getWidth() ;
            }

            /// \brief returns true if the drivebase has a shifter
            /// \returns true if the drivebase has a gear shifter
            bool hasGearShifter() const {
                return gear_ != nullptr ;
            }

            /// \brief Create encoders for the tank drive that are not attached to the TalonSRX controllers
            /// \param l1 first digital IO for left encoder
            /// \param l2 second digital IO for left encoder
            /// \param r1 first digital IO for right encoder
            /// \param r2 second digital IO for right encoder                       
            void setEncoders(int l1, int l2, int r1, int r2) ;

            /// \brief set the gear shifter for the drivebase
            /// \param index the gear to shift into
            void setGearShifter(int index) ;

            /// \brief Return the current angle of the robot relative to its starting angle
            /// \returns The current angle of the robot
            virtual double getAngle() const {
                return angular_.getDistance() - angle_offset_;
            }

            /// \brief Resets the current angle of the robot.
            /// This can be used e.g. at the start of an automode when our starting angle is known.
            virtual void setAngle(double angle) {
                angle_offset_ = angular_.getDistance() - angle;
                kin_->set(kin_->getX(), kin_->getY(), angle);
            }

            /// \brief Return the net distance travelled in inches by the left side of the drivebase.
            /// If the robot travels forward and then back by the same distance, the net distance
            /// is zero and zero will be returned.
            /// \returns The distance traveled by the left side in inches
            double getLeftDistance() const {
                return dist_l_ ;
            }

            /// \brief Return the net distance travled in inches by the right side of the drivebase.
            /// If the robot travels forward and then back by the same distance, the net distance
            /// is zero and zero will be returned.
            /// \returns The distance traveled by the right side in inches
            double getRightDistance() const {
                return dist_r_ ;
            }

            /// \brief Return the average net distanced travelled by the two sides of the drivebase.
            /// \returns The average net distance travelled by the two sides to the drivebase
            virtual double getDist() const {
                return (dist_r_ + dist_l_) / 2.0;
            }

            /// \brief returns the left encoder tick count
            /// \returns left encoder tick count
            int getLeftTickCount() {
                return ticks_left_ ;
            }

            /// \brief returns the right encoder tick count
            /// \returns right encoder tick count
            int getRightTickCount() {
                return ticks_right_ ;
            }

            /// \brief Return the velocity of the drive base
            /// \returns the linear velocity of the drive base
            virtual double getVelocity() const {
                return (left_linear_.getVelocity() + right_linear_.getVelocity()) / 2.0 ;
            }

            /// \brief Return the velocity of the left side of the drive base
            /// \returns the linear velocity of the left side of the drive base
            virtual double getLeftVelocity() const {
                return left_linear_.getVelocity() ;
            }

            /// \brief Return the velocity of the right side of the drive base
            /// \returns the linear velocity of the right side of the drive base
            virtual double getRightVelocity() const {
                return right_linear_.getVelocity() ;
            }           

            /// \brief Return the angular velocity of the drive base
            /// \returns the angular velocity of the drive base
            double getAngularVelocity() const {
                return angular_.getVelocity() ;
            }

            /// \brief Return the acceleration of the drive base
            /// \returns the linear acceleration of the drive base
            double getAcceleration() const {
                return (left_linear_.getAcceleration() + right_linear_.getAcceleration()) / 2.0 ;
            }

            /// \brief Return the angular acceleration of the drive base
            /// \returns the angular acceleration of the drive base
            double getAngularAcceleration() const {
                return angular_.getAcceleration() ;
            }

            /// \brief Return the total angle of the drive base.
            /// The total angle is the total change in angle since the navx was reset.  The
            /// total angle is not normalized to be between 180 and -180.
            /// \returns the total angle of the robot
            double getTotalAngle() const {
                return total_angle_ ;
            }

            /// \brief Invert the output of the left motors
            void invertLeftMotors() ;

            /// \brief Invert the output of the left motors
            void invertRightMotors() ;  

            /// \brief Invert the left encoders
            void invertLeftEncoder() {
                left_enc_->SetReverseDirection(true) ;
            }

            /// \brief Invert the right encoders
            void invertRightEncoder() {
                right_enc_->SetReverseDirection(true) ;
            }

            /// \brief Compute the current state of the drivebase.
            /// This method generally reads the input sensors associated with the drivebase and
            /// calculates the current state of the drivebase, including the distance traveled,
            /// speed and acceleration of the robot.  This method is generally called once per
            /// robot loop before any robot controller gets a chance to run.
            virtual void computeMyState();

            /// \brief Run the subsystem
            virtual void run() ;

            /// \brief This method returns true if the tankdrive subsystem can execute the given action
            /// \param action the section to test to see if it can be executed
            /// \returns true if the action can be executed, false otherwise
            virtual bool canAcceptAction(xero::base::ActionPtr action) ;

            /// \brief set the drive base to low gear
            void lowGear() ;

            /// \brief set the drive base to high gear
            void highGear()  ;

            /// \brief reset the tank drive after leaving a mode
            virtual void reset() ;

            /// \brief init the subsystem for the enabled mode
            /// \param ltype the type of loop being enabled (e.g. teleop, auto, test)
            virtual void init(LoopType ltype) ;

            /// \brief start a trip timer for the drive base
            /// \param name name of the trip timer
            void startTrip(const std::string &name) ;

            /// \brief return the current value for a trip timer
            /// \param name name of the trip timer
            double getTripDistance(const std::string &name) ;

            std::shared_ptr<xero::misc::Kinematics> getKinematics() const {
                return kin_;
            }


        private:
            /// \brief Set the motors to output at the given percentages
            /// \param left_percent the percent output for the left motors
            /// \param right_percent the percent output for the right motors
            void setMotorsToPercents(double left_percent, double right_percent);

        private:
            MotorController::NeutralMode automode_neutral_ ;
            MotorController::NeutralMode teleop_neutral_ ;
            MotorController::NeutralMode reset_neutral_ ;

            std::shared_ptr<MotorController> left_motors_;
            std::shared_ptr<MotorController> right_motors_;

            std::vector<double> left_power_ ;
            std::vector<double> right_power_ ;

            std::shared_ptr<frc::Encoder> left_enc_ ;
            std::shared_ptr<frc::Encoder> right_enc_ ;

            std::shared_ptr<frc::Solenoid> gear_ ;

            std::shared_ptr<AHRS> navx_ ;
            double total_angle_ ;

            double angle_offset_;
            xero::misc::Speedometer angular_ ;
            xero::misc::Speedometer left_linear_ ;
            xero::misc::Speedometer right_linear_ ;

            int ticks_left_ ;
            int ticks_right_ ;
            
            double dist_l_, dist_r_;
            double last_dist_l_, last_dist_r_ ;

            double left_inches_per_tick_ ;
            double right_inches_per_tick_ ;

            std::map<std::string, double> trip_start_ ;

            std::shared_ptr<xero::misc::Kinematics> kin_ ;

            double xyz_velocity_ ;
        };
    }
}
