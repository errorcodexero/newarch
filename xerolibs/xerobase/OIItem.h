#pragma once

#include <frc/DriverStation.h>

/// \file


namespace xero {
    namespace base {
        /// \brief the base class for any switch, button, axis, etc. that is part of an OIDevice
        /// \sa OIDevice
        class OIItem {
        public:
            /// \brief create a new OIItem with an item number
            /// \param item the item number for this item
            OIItem(int item) {
                item_ = item ;
            }

            /// \brief return the value for this item
            /// \returns item value for this item
            virtual int getValue() const = 0 ;

            /// \brief set the item value for this item
            /// \param ds the drive station class which contains method for getting the value from a joystick device
            /// \param index the index of the value to retreive
            virtual void setValue(frc::DriverStation &ds, int index) = 0 ;

            /// \brief returns the item number for this item
            /// \returns the item number for this item
            virtual int getItem() const {
                return item_ ;
            }

        private:
            int item_ ;
        } ;
    }
}
