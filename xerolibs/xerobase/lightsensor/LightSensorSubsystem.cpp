#include "LightSensorSubsystem.h"
#include "Robot.h"
#include <cassert>

using namespace xero::misc;

namespace xero {
    namespace base {
        LightSensorSubsystem::LightSensorSubsystem(Robot &robot, const std::string &name, const std::string &base, int sensor_count) : Subsystem(robot,name) {
            SettingsParser settings_parser = robot.getSettingsParser() ;
            for(int i=0; i<sensor_count; i++ ){
                int sensor_address = settings_parser.getInteger(base + std::to_string(i)) ;
                std::shared_ptr <frc::DigitalInput> sensor = std::make_shared <frc::DigitalInput>(sensor_address) ;
                light_sensors_.push_back(sensor) ;
                sensor_data_.push_back(false) ;
            }
        }

        LightSensorSubsystem::LightSensorSubsystem(Robot &robot, const std::string &name, std::vector<int> sensor_numbers) : Subsystem(robot,name) {
            for (const int &sensorAddress : sensor_numbers){
                std::shared_ptr <frc::DigitalInput> sensor = std::make_shared <frc::DigitalInput>(sensorAddress) ;
                light_sensors_.push_back(sensor) ;
                sensor_data_.push_back(false) ;                
            }
        }        

        void LightSensorSubsystem::computeState(){
            assert(sensor_data_.size() == light_sensors_.size()) ;
            something_detected_ = false ;
            angle_ = 0 ;

            double midpoint = light_sensors_.size()/2 ;
            for(unsigned int i = 0; i<light_sensors_.size(); i++) {

                bool light_sensor = light_sensors_[i]->Get() ;
                sensor_data_[i] = light_sensor ;
                something_detected_ |= light_sensor ;
                if(light_sensor == true) {
                    angle_+= (i-midpoint/light_sensors_.size()) ;
                } 
            }
        }
    }
}