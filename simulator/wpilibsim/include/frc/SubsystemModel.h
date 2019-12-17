#pragma once

#include <ctre/Phoenix.h>
#include <rev/CANSparkMax.h>
#include "Encoder.h"
#include "PowerDistributionPanel.h"
#include "AnalogInput.h"
#include "Counter.h"
#include "DigitalInput.h"
#include "DriverStation.h"
#include "Solenoid.h"
#include "VictorSP.h"
#include "Timer.h"
#include "AHRS.h"
#include "Relay.h"
#include <MessageLogger.h>
#include <mutex>
#include <map>
#include <vector>
#include <list>
#include <string>

namespace xero {
    namespace sim {
        class RobotSimBase ;

        class SubsystemModel {
        public:
            SubsystemModel(RobotSimBase &simbase, const std::string &name) ;
            virtual ~SubsystemModel() ;

            virtual void run(double dt) = 0 ;
            virtual void init() {                
            }
            virtual void inputChanged(SimulatedObject *obj) = 0 ;
            virtual std::string toString() = 0 ;

            virtual void addDevice(ctre::phoenix::motorcontrol::can::TalonSRX *talon)  {                
            }

            virtual void addDevice(rev::CANSparkMax *spark)  {                
            }            

            virtual void addDevice(frc::Encoder *encoder) {                
            }

            virtual void addDevice(frc::PowerDistributionPanel *pdp) {                
            }            

            virtual void addDevice(frc::AnalogInput *input) {                
            }            

            virtual void addDevice(frc::Counter *input) {                
            }            

            virtual void addDevice(frc::DigitalInput *input) {                
            }

            virtual void addDevice(frc::DriverStation *station) {
            }

            virtual void addDevice(frc::Solenoid *sol) {
            }

            virtual void addDevice(frc::VictorSP *victor) {
            }

            virtual void addDevice(ctre::phoenix::motorcontrol::can::VictorSPX *victor) {                
            }

            virtual void addDevice(AHRS *navx) {                
            }

            virtual void addDevice(frc::Relay *relay) {                
            }

            virtual bool processEvent(const std::string &name, int value) {          
                return false ;      
            }

            std::mutex &getLockMutex() ;

            const std::string &getName() const {
                return name_ ;
            }

            RobotSimBase &getSimulator() {
                return simbase_ ;
            }

            xero::misc::MessageLogger &getRobotMessageLogger()  ;

            virtual void generateDisplayInformation(std::list<std::string> &lines) ;

        protected:

        private:
            std::string name_ ;
            RobotSimBase &simbase_ ;
        } ;
    }
}
