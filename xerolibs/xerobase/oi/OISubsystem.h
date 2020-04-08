#pragma once

#include "Subsystem.h"
#include "oi/DriverGamepad.h"
#include "actions/SequenceAction.h"

/// \file


namespace xero {
    namespace base {
        /// \brief This is the base class for the OI subsystem
        /// This class manages the OI by calling each of HID Devices that are part
        /// of this subsystem during computeMyState() and allow them to add actions to
        /// the internal sequence list.  During the run() call each of these actions
        /// are run.
        class OISubsystem : public Subsystem {
        public:
            /// \brief create the OI Subsystem object
            /// \param robot the robot object this subsystem belongs to
            /// \param name the name of the OI subsystem
            /// \param adddriver if true add the driver gamepad to the OI subsystem
            OISubsystem(Subsystem *parent, const std::string &name, bool adddriver = true) ;

            /// \brief destroy the OI subsystem
            virtual ~OISubsystem() ;

            /// \brief compute the state of the subsystem
            /// If the OI is enabled, this will generate a series of actions to be executed based on
            /// the state of the OI
            virtual void computeMyState()  ;

            /// \brief generate actions from the OI
            /// \param seq the action sequenced used to store the generated actions
            virtual void generateActions(SequenceActionPtr seq) ;

            /// \brief run the action generated in the computeMyState above
            virtual void run() ;

            virtual bool canAcceptAction(ActionPtr act) ;

            /// \brief return the value of the automode selctor.
            /// This method iterates through each of the HID devices and the first HID device that
            /// returns an automode selection that is not -1, this value is returned.
            /// \returns the value of the automodel selection
            int getAutoModeSelector() ;

            void init(xero::base::LoopType ltype) override;

            /// \brief return the driver gamepad
            std::shared_ptr<DriverGamepad> getDriverGamepad() {
                return driver_ ;
            }

            virtual void postHWInit() ;

protected:
            /// \brief Add a HIDDevice to the list of devices managed by the OI
            /// \param dev the HIDDevice to add to the list
            void addHIDDevice(std::shared_ptr<HIDDevice> dev) {
                hiddevices_.push_back(dev) ;
            }

        private:
            bool inited_ ;
            std::list<std::shared_ptr<HIDDevice>> hiddevices_ ;
            std::shared_ptr<DriverGamepad> driver_ ;
        } ;
    }
}
