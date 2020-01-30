#pragma once

#include <optional>

#include <frc/DigitalInput.h>
#include <SettingsParser.h>
#include <singlemotorsubsystem/SingleMotorSubsystem.h>
#include <motors/MotorFactory.h>

namespace xero {
    namespace droid {
        class Conveyor : public xero::base::Subsystem {
            friend class ConveyorAction;
        public:
            Conveyor(xero::base::Subsystem *parent);
            virtual ~Conveyor() {}

            void postHWInit() override;

            int getBallCount() { return ballCount_; }

            static const int MAX_BALLS = 5;

            enum class Direction {
                TowardsShooter,
                TowardsIntake,
            };

            typedef std::shared_ptr<frc::DigitalInput> SensorPtr;

            virtual void computeState() ;

            bool getSensor1() {
                return sensor1_state_ ;
            }

            bool getSensor2() {
                return sensor2_state_ ;
            }

            bool getSensor3() {
                return sensor3_state_ ;
            }

            virtual bool canAcceptAction(xero::base::ActionPtr action) ;

        private:
            // Runs the motors in the specified direction, or stops them if direction is null.
            void setMotor(double v);

            SensorPtr createSensor(std::string configName);

        private:
            SensorPtr sensor1_ ;
            SensorPtr sensor2_ ;
            SensorPtr sensor3_ ;

            bool sensor1_state_ ;
            bool sensor2_state_ ;
            bool sensor3_state_ ;

            int ballCount_ ;

            xero::base::MotorFactory::MotorPtr conveyor_belts_ ;
            xero::base::MotorFactory::MotorPtr turret_conveyor_ ; 
        };
    }
}