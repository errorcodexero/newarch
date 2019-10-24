#include "LightSensorSubsystem.h"
#include "Robot.h"
#include <cassert>
#include <string>
#include <iostream>
#include <iomanip>

using namespace xero::misc;
using namespace xero::base;

namespace xero {
    namespace phaser {
        LightSensorSubsystem::LightSensorSubsystem(Robot &robot, const std::string &name, const std::string &base, int sensor_count) : Subsystem(robot,name), ITerminator("LineFollower") {
            SettingsParser settings_parser = robot.getSettingsParser() ;
            for(int i=0; i<sensor_count; i++ ){
                int sensor_address = settings_parser.getInteger(base + std::to_string(i)) ;
                std::shared_ptr <frc::DigitalInput> sensor = std::make_shared <frc::DigitalInput>(sensor_address) ;
                light_sensors_.push_back(sensor) ;
            }

            is_detected_ = false ;
        }

        LightSensorSubsystem::LightSensorSubsystem(Robot &robot, const std::string &name, std::vector<int> sensor_numbers) : Subsystem(robot,name), ITerminator("LineFollower") {
            for (const int &sensorAddress : sensor_numbers){
                std::shared_ptr <frc::DigitalInput> sensor = std::make_shared <frc::DigitalInput>(sensorAddress) ;
                light_sensors_.push_back(sensor) ;
            }

            is_detected_ = false ;
        }        

        void LightSensorSubsystem::computeState() { 
            angle_ = 0 ;
            int sensors_on = 0;

            double midpoint = (light_sensors_.size()-1)/2.0 ;
            uint32_t data = 0 ;
            for(unsigned int i = 0; i < light_sensors_.size(); i++) {
                bool light_sensor = !light_sensors_[i]->Get() ;
                if (light_sensor) {
                    sensors_on++ ;
                    data |= (1 << i) ;
                    angle_ = angle_ + ((i-midpoint)/midpoint) ;
                }
            }
            if(sensors_on > 0)
                angle_= angle_/sensors_on ;

            sensor_data_.push_front(data) ;
            while (sensor_data_.size() > 10)
                sensor_data_.pop_back() ;

            is_detected_ = (sensors_on > 0) ;

            auto &logger = getRobot().getMessageLogger() ;
            logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_LINE_FOLLOWER_VERBOSE) ;
            logger << "LightSensor :" ;
            logger << getName() << " " ;
            logger << sensor_data_.front() ;      
            logger.endMessage() ;            
        }
    }
}