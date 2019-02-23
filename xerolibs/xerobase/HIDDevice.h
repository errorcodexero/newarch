#pragma once

/// \file


namespace xero {
    namespace base {
        class ActionSequence ;
        class OISubsystem ;

        /// \brief A basic HID device that may be part of the OISubsystem
        class HIDDevice  {
        public:
            /// \brief Create a new HID Devices
            /// \param sub the oi subsystem this device belogs to
            /// \param index the joystick index for this device
            HIDDevice(OISubsystem &sub, int index) : subsystem_(sub) {
                index_ = index ;
                enabled_ = true ;
            }

            /// \brief destroy this device
            virtual ~HIDDevice() {                
            }

            /// \brief return the OI subsystem for this device
            /// \returns the OI subsystem for this device
            OISubsystem &getSubsystem() {
                return subsystem_ ;
            }

            /// \brief return the joystick index for this HID device
            /// \returns the joystick index for this HID device
            int getIndex() const {
                return index_ ;
            }

            /// \brief compute the state for this HID device
            virtual void computeState() = 0 ;

            /// \brief generation the actions for the HID device
            /// \param seq the action sequence for generated actions
            virtual void generateActions(ActionSequence &seq) = 0 ;

            /// \brief return the value of the automode selector
            /// \returns the value of the auto mode selector
            virtual int getAutoModeSelector() {
                return -1 ;
            }

            virtual void enable(bool b) {
                enabled_ = b ;
            }

            virtual bool isEnabled() const {
                return enabled_ ;
            }

            /// \brief This is called after the hardware is initialization
            virtual void init() {
            }

        private:
            OISubsystem &subsystem_ ;
            bool enabled_ ;
            int index_ ;
        } ;
    }
}