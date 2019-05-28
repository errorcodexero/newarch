#include "ServoGoToAngle.h"
#include "ServoSubsystem.h"

namespace xero {
    namespace gopigo3 {
        ServoGoToAngle::ServoGoToAngle(ServoSubsystem &sub, double angle) : subsystem_(sub) {
            angle_ = angle ;
        }

        ServoGoToAngle::~ServoGoToAngle() {         
        }

        /// \brief Start the duty cycle action.  This method actually sets the
        /// motor duty cycles.
        void ServoGoToAngle::start() {
            subsystem_.servo_->setPosition(angle_) ;
        }

        /// \brief Run the duty cycle action.  This method does nothing.            
        void ServoGoToAngle::run() {            
        }

        /// \brief Signals if this action is done, always returs true    
        bool ServoGoToAngle::isDone() {
            return true ;
        }

        /// \brief Canel the current action, stops the motors and returns true
        void ServoGoToAngle::cancel() {         
        }

        /// \brief Returns a human readable string for the action
        std::string ServoGoToAngle::toString() {
            std::string ret = "ServoGoToAngle " + std::to_string(angle_) ;
            return ret ;
        }
    }
}
