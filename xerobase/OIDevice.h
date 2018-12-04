#pragma once

#include "HIDDevice.h"
#include "OIButton.h"
#include "OIAxisScale.h"
#include "OIAxisSwitch.h"
#include <map>
#include <memory>

namespace xero {
    namespace base {
        /// \brief This class is derived from the HIDDevice class and maps OI items to an OI.
        /// It works by reading joystick buttons, axis, and POV and mapping them onto items that
        /// exist on an OI.f
        class OIDevice : public HIDDevice { 
        public:
            /// \brief Create the OIDevice
            /// \param sub the OI subsystem this device is part of
            /// \param index the joystick index for this OI device
            OIDevice(Subsystem &sub, int index) ;

            /// \brief Destroy this device
            virtual ~OIDevice() ;

            /// \brief compute the state of the device by mapping joystick values to OIItems
            /// \param seq the action sequence to store actions
            virtual void computeState(ActionSequence &seq) ;
            
        protected:
            /// \brief map a joystick button to an OIButton of the given type
            /// \param button the joystick button
            /// \param type the type of button
            /// \returns the item ID for this OI button
            /// \sa OIButton
            size_t mapButton(int button, OIButton::ButtonType type) ;

            /// \brief map a joystick axis to set of returned integer (automode selector knob)
            /// \param axis the joystick axis
            /// \param mapping the range values for each of the integer settings for the axis
            /// \returns the item ID for this OI button
            /// \sa OIAxisScale
        	size_t mapAxisScale(int axis, const std::vector<double> &mapping) ;

            /// \brief map a joystick axis to an OISwitch of the given type
            /// \param axis the joystick axis to map
            /// \param positions the number of positions for the switch (2 position switch, 3 position switch)
            /// \returns the item ID for this OI button
            /// \sa OIAxisSwitch
            size_t mapAxisSwitch(int axis, int positions) ;

            /// \brief return the value of a given item
            /// \param which the item id of the item to return the value for
            /// \returns the value for the given item
            int getValue(size_t which) ;  

        private:
            std::vector<std::shared_ptr<OIItem>> items_ ;
        } ;
    }
}