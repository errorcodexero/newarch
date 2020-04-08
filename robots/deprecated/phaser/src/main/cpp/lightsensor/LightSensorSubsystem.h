#pragma once

#include "Subsystem.h"
#include "ITerminator.h"

#include <Robot.h>
#include <Subsystem.h>
#include <SettingsParser.h>

#include <frc/DigitalInput.h>
#include <vector>
#include <list>

/// \file


namespace xero {
    namespace phaser {
        /// \brief This class is a subsystem that consists of an array of light sensors.
        class LightSensorSubsystem : public xero::base::Subsystem, public xero::base::ITerminator {

        private:
            constexpr static double epsilon = 1e-3 ;

        public:
            /// \brief Create a new subsystem object
            /// \param robot a reference to the robot for this subsystem
            /// \param name the name of this subsystem, must be unique across all subsystems
            /// \param id the message logger id to use for messages from this class            
            /// \sa xero::misc::SettingsParser
            /// \param base sensor name
            /// \param sensor_count number of sensors
            LightSensorSubsystem(xero::base::Robot &robot, const std::string &name, const std::string &base, int sensor_count) ;

            LightSensorSubsystem(xero::base::Robot &robot, const std::string &name, std::vector<int> sensor_numbers) ;

            virtual ~LightSensorSubsystem() {
            }

            virtual bool canAcceptAction(xero::base::ActionPtr action) {
                return false ;
            }

            double guidanceAngle() {
                return angle_;
            }

            virtual void computeMyState() ;


            virtual bool shouldTerminate() {
                return is_detected_ ;
            }
            
            bool getSensorState(uint32_t index) {
                if (sensor_data_.size() == 0)
                    return false ;

                uint32_t data = sensor_data_.front() ;
                return (data & (1 << index)) ? true : false ;
            }

            uint32_t getSensorsState() {
                if (sensor_data_.size() == 0)
                    return 0 ;
                    
                return sensor_data_.front() ;
            }

            size_t getSensorCount() const {
                return light_sensors_.size() ;
            }

            bool detectedObject() {
                return is_detected_ ;
            }

        private:
            std::vector <std::shared_ptr<frc::DigitalInput>> light_sensors_ ;
            double angle_ ;
            std::list<uint32_t> sensor_data_ ;
            int detect_count_ ;
            bool is_detected_ ;
        }  ;
    }
}
