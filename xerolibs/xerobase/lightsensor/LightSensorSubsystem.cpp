#include "LightSensorSubsystem.h"
#include "Robot.h"
#include <cassert>
#include <String>
#include <iostream>
#include <iomanip>

using namespace xero::misc;

namespace xero {
    namespace base {
        LightSensorSubsystem::LightSensorSubsystem(Robot &robot, const std::string &name, const std::string &base, int sensor_count) : Subsystem(robot,name) {
            SettingsParser settings_parser = robot.getSettingsParser() ;
            for(int i=0; i<sensor_count; i++ ){
                int sensor_address = settings_parser.getInteger(base + std::to_string(i)) ;
                std::shared_ptr <frc::DigitalInput> sensor = std::make_shared <frc::DigitalInput>(sensor_address) ;
                light_sensors_.push_back(sensor) ;
            }
        }

        LightSensorSubsystem::LightSensorSubsystem(Robot &robot, const std::string &name, std::vector<int> sensor_numbers) : Subsystem(robot,name) {
            for (const int &sensorAddress : sensor_numbers){
                std::shared_ptr <frc::DigitalInput> sensor = std::make_shared <frc::DigitalInput>(sensorAddress) ;
                light_sensors_.push_back(sensor) ;
            }
        }        

        void LightSensorSubsystem::computeState() { 
            angle_ = 0 ;

            double midpoint = light_sensors_.size()/2 ;
            uint32_t data = 0 ;
            for(unsigned int i = 0; i<light_sensors_.size(); i++) {
                bool light_sensor = light_sensors_[i]->Get() ;
                if (!light_sensor) {
                    data |= (1 << i) ;
                    angle_ += (i-midpoint)/light_sensors_.size() ;
                }
            }

            bool adddata = true ;

            if (light_sensors_.size() == 3 && sensor_data_.size() > 0 && data == 0 && sensor_data_.front() == 2)
                adddata = false ;

            if (adddata)
            {
                sensor_data_.push_front(data) ;

                while (sensor_data_.size() > 4)
                    sensor_data_.pop_back() ;
            }
        }
    }
}