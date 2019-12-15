#pragma once

#include <frc/SubsystemModel.h>
#include <vector>

namespace xero
{
    namespace sim
    {
        class RobotSimBase ;

        class TankDriveModel : public SubsystemModel
        {
        public:
            enum class MotorType
            {
                MTTalon,
                MTSpark,
                MTVictor
            } ;

            enum class Gear
            {
                LowGear,
                HighGear,
            } ;
        private:
            static constexpr double PI = 3.14159265359;

        public:
            TankDriveModel(RobotSimBase &borobt, MotorType mt);
            virtual ~TankDriveModel();

            virtual bool processEvent(const std::string &name, int value) ;             

            virtual void run(double dt);
            virtual void inputChanged(SimulatedObject *obj);
            virtual std::string toString()  ;

            virtual void addDevice(ctre::phoenix::motorcontrol::can::TalonSRX *motor);
            virtual void addDevice(rev::CANSparkMax *motor) ;
            virtual void addDevice(frc::Encoder *encoder);
            virtual void addDevice(AHRS *navx);
            virtual void addDevice(frc::Solenoid *sol) ;

            double getXPos() { 
                return xpos_ ;
            }

            double getYPos() {
                return ypos_ ;
            }

            double getAngle() {
                return angle_ ;
            }

            double getSpeed() {
                return speed_ ;
            }

            double getWidth() {
                return width_ ;
            }

            double getLength() {
                return length_ ;
            }

            void generateDisplayInformation(std::list<std::string> &lines) ;

        private:
            void updatePosition(double dx, double dy, double angle) ;

            double capValue(double prev, double desired, double maxchange) ;

            void lowGear() ;
            void highGear() ;

            void calcLowLevelParams(RobotSimBase &simbase) ;

            void getMotorParams(RobotSimBase &simbase) ;

        private:
            // If true, we are in low gear.  Otherwise high gear
            Gear gear_ ;

            // The distance the left and right wheels have traveled
            double left_;
            double right_;

            // The current angle of the robot
            double angle_;

            // The navx offset, used to return the correct navx angle if the
            // navx has been reset to zero yaw at an arbitrary robot angle
            double navx_offset_ ;

            // The number of encoder ticks per revolution of the wheels
            double ticks_per_rev_;

            // The diameter of the wheels of the robot
            double diameter_;

            // The current power assigned to the robot
            double left_power_;
            double right_power_;

            // The scrub and width of the driverbase, used in the kinematics to
            // track the location of the driverbase
            double scrub_;
            double width_;
            double length_ ;

            // High gear and low gear revolutions per second per power
            double high_rps_per_power_per_time_;
            double low_rps_per_power_per_time_;

            // The maximum revolution per second change
            double high_max_change_ ;
            double low_max_change_ ;

            // The current maximum revs per second change, will be either the high or low
            double current_max_change_ ;

            // The percent error versus ideal (random, not tested well)
            double left_right_error_ ;

            // The left side revs per second per power
            double right_rps_per_power_per_time_;
            double left_rps_per_power_per_time_;                     

            // Current linear speed of the robot
            double speed_ ;

            // Current left RPS based on gear and power applied
            double current_left_rps_ ;
            double current_right_rps_ ;

            // The X and Y position of the robot
            double xpos_ ;
            double ypos_ ;

            // The previous X and Y positon of the robot.   Used to calculate the
            // distance the robot has moved since the last time the model was run.  THis
            // if used in the kinematics to track robot position and heading.
            double last_xpos_ ;
            double last_ypos_ ;

            // The left and right encoder values
            int left_enc_value_ ;
            int right_enc_value_ ;

            // The set of motors for the left and right side of the robot
            std::vector<ctre::phoenix::motorcontrol::can::TalonSRX *> left_talon_motors_;
            std::vector<ctre::phoenix::motorcontrol::can::TalonSRX *> right_talon_motors_;

            std::vector<rev::CANSparkMax *> left_spark_motors_;
            std::vector<rev::CANSparkMax *> right_spark_motors_;         

            // The left and right encoers for the robot
            frc::Encoder *left_enc_;
            frc::Encoder *right_enc_;

            // The solenoid that engages the shifer
            frc::Solenoid *shifter_ ;

            // The NAVX that supplies the robot angle
            AHRS *navx_;

            // The type of motors
            MotorType mt_ ;

            std::vector<int> left_motors_ ;
            std::vector<int> right_motors_ ;
            double left_motor_mult_ ;
            double right_motor_mult_ ;

            int left_encoder_1_ ;
            int left_encoder_2_ ;
            int left_encoder_mult_  ;
            int right_encoder_1_ ;
            int right_encoder_2_ ;
            int right_encoder_mult_ ;
            int shifter_channel_ ;

        };
    } // namespace sim
} // namespace xero
