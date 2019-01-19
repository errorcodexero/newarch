#pragma once

#include <list>

#include <Kinematics.h>
#include <DriveBase.h>
#include <Speedometer.h>
#include <AHRS.h>
#include <ctre/Phoenix.h>
#include <frc/Solenoid.h>
#include <frc/Encoder.h>
#include <frc/VictorSP.h>
#include <ctre/Phoenix.h>


/// \file


namespace xero {
    namespace base {
        /// \brief a convience type for a shared pointer to a TalonSRX object
        typedef std::shared_ptr<ctre::phoenix::motorcontrol::can::TalonSRX> TalonPtr;
        
        /// \brief a convience type for a shared pointer to a Victor SP object        
        typedef std::shared_ptr<frc::VictorSP> VictorPtr ;

        /// \brief a tank-style drivebase
        class TankDrive : public DriveBase {
            /// \brief class for tank drive actions
            friend class TankDriveAction;
            /// \brief class for tank drive actions         
            friend class TankDriveDistanceAction;
            /// \brief class for tank drive actions         
            friend class TankDriveVelocityAction;
            /// \brief class for tank drive actions         
            friend class TankDriveCharAction ;
            /// \brief class for tank drive actions         
            friend class TankDrivePowerAction ;
            /// \brief class for tank drive actions         
            friend class TankDriveCharAction ;
            /// \brief class for tank drive actions         
            friend class TankDriveTimedPowerAction ;
            /// \brief class for tank drive actions         
            friend class TankDriveAngleAction ;
            /// \brief class for tank drive actions         
            friend class TankDriveAngleCharAction ;
            /// \brief class for tank drive actions
            friend class TankDriveFollowPathAction ;
            friend class LineFollowAction ;
            friend class LineDetectAction ;

        public:
            /// \brief Create a new tank drive object
            /// \param robot The robot that contains this tank drive subsystem
            /// \param left_motor_ids A list of TalonSRX ids for the left side of the drivebase, the first
            /// being the master and the rest being followers
            /// \param right_motor_ids A list of TalonSRX ids for the right side of the drivebase, the first
            /// being the master and the rest being followers
            TankDrive(Robot& robot, const std::list<int> &left_motor_ids, const std::list<int> &right_motor_ids);

            /// \brief destroy a tank drive object
            virtual ~TankDrive() ;

            /// \brief returns true if the drivebase has a shifter
            /// \returns true if the drivebase has a gear shifter
            bool hasGearShifter() const {
                return gear_ != nullptr ;
            }

            /// \brief set to dump state each cycle
            /// If true, the state of the tankdrive is printed after each iteration of the robot loop
            /// \param state the value of the flag
            void setDumpStateFlag(bool state) {
                dumpstate_ = state ;
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
                return angular_.getDistance() ;
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
            virtual void computeState();

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

        private:
            /// \brief Set the motors to output at the given percentages
            /// \param left_percent the percent output for the left motors
            /// \param right_percent the percent output for the right motors
            void setMotorsToPercents(double left_percent, double right_percent);

            static void initTalonList(const std::list<int>& ids, std::list<TalonPtr>& talons) ;
            static void initVictorList(const std::list<int> &ids, std::list<VictorPtr> &victors) ;

        private:
            std::list<TalonPtr> left_talon_motors_, right_talon_motors_;
            std::list<VictorPtr> left_victor_motors_, right_victor_motors_ ;

            std::shared_ptr<frc::Encoder> left_enc_ ;
            std::shared_ptr<frc::Encoder> right_enc_ ;

            std::shared_ptr<frc::Solenoid> gear_ ;
            
            xero::misc::Speedometer angular_ ;
            xero::misc::Speedometer left_linear_ ;
            xero::misc::Speedometer right_linear_ ;

            int ticks_left_ ;
            int ticks_right_ ;
            
            double dist_l_, dist_r_;

            double left_inches_per_tick_ ;
            double right_inches_per_tick_ ;

            AHRS *navx_ ;

            xero::misc::Kinematics *kin_ ;

            bool dumpstate_ ;
        };
    }
}
