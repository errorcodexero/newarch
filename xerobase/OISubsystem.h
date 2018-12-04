#pragma once

#include "Subsystem.h"
#include "DriverGamepad.h"
#include "ActionSequence.h"

/// \file


namespace xero {
    namespace base {
        /// \brief This is the base class for the OI subsystem
        /// This class manages the OI by calling each of HID Devices that are part
        /// of this subsystem during computeState() and allow them to add actions to
        /// the internal sequence list.  During the run() call each of these actions
        /// are run.
        class OISubsystem : public Subsystem {
        public:
            /// \brief create the OI Subsystem object
            /// \param robot the robot object this subsystem belongs to
            /// \param name the name of the OI subsystem
            /// \param adddriver if true add the driver gamepad to the OI subsystem
            OISubsystem(Robot &robot, const std::string &name, bool adddriver = true) ;

            /// \brief destroy the OI subsystem
            virtual ~OISubsystem() ;

            /// \brief enable the OI subsystem.
            /// If this OI subsystem is enabled any action stored in the internal action
            /// list that are added during the computeState method will be executed.
			void enable() {
				enabled_ = true ;
			}

            /// \brief disable the OI subsystem.
            /// If this OI subsystem is disabled, no actions will be generated as part of 
            /// computing the state of the OI subsystem.
			void disable() {
				enabled_ = false ;
			}            

            /// \brief compute the state of the subsystem
            /// If the OI is enabled, this will generate a series of actions to be executed based on
            /// the state of the OI
            virtual void computeState()  ;

            /// \brief run the action generated in the computeState above
            virtual void run() ;

            /// \brief return the value of the automode selctor.
            /// This method iterates through each of the HID devices and the first HID device that
            /// returns an automode selection that is not -1, this value is returned.
            /// \returns the value of the automodel selection
            int getAutoModeSelector() ;

protected:
            /// \brief Add a HIDDevice to the list of devices managed by the OI
            /// \param dev the HIDDevice to add to the list
            void addHIDDevice(std::shared_ptr<HIDDevice> dev) {
                hiddevices_.push_back(dev) ;
            }

        private:
            bool inited_ ;
            bool enabled_ ;
            std::shared_ptr<ActionSequence> seq_ ;
            std::list<std::shared_ptr<HIDDevice>> hiddevices_ ;
            std::shared_ptr<DriverGamepad> driver_ ;
        } ;
    }
}
