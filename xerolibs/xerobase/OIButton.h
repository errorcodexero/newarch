#pragma once

#include "OIItem.h"
#include <iostream>

/// \file


namespace xero {
    namespace base {
        /// \brief A button item on a HIDDevices
        /// \sa HIDDevice
        class OIButton : public OIItem {
        public:
            /// \brief The type of a button
            enum class ButtonType
            { 
                Level,          ///< The button value will track the input value
                LevelInv,       ///< The button value will be the inverse of the input value
                LowToHigh,      ///< The button value will be high for a single cycle after a low to high transition
                HighToLow       ///< The button value will be low for a single cycle after a high to low transition
            } ;     

        public:
            /// \brief Create a new OI button
            /// \param item the item number for the button
            /// \param btype the button type for the button
            /// \sa ButtonType
            OIButton(int item, ButtonType btype) : OIItem(item) {
                value_ = 0 ;
                type_ = btype ;
            }

            /// \brief return the button type for the button
            /// \returns the button type for the button
            ButtonType getButtonType() const {
                return type_ ;
            }

            /// \brief return the value for this butt
            /// \returns the value for the button
            virtual int getValue() const {
                return static_cast<int>(value_) ;
            }

            /// \brief set the value for the button
            /// \param ds the driver station class to get button values
            /// \param index the index of the joystick
            virtual void setValue(frc::DriverStation &ds, int index) {
                setValue(ds.GetStickButton(index, getItem())) ;
            }

private:
            void setValue(bool value) {
                switch(type_) {
                case ButtonType::Level:
                    value_ = value ;
                    break ;

                case ButtonType::LevelInv:
                    value_ = !value ;
                    break ;

                case ButtonType::LowToHigh:

                    if (!prev_ && value)
                        value_ = true ;
                    else
                        value_ = false ;

                    //if (getItem() == 5)
                        //std::cout << "CHECKING " << prev_ << " " << value << " " << value_ << std::endl ;
                    break ;

                case ButtonType::HighToLow:
                    if (prev_ && !value)
                        value_ = true ;
                    else
                        value_ = false ;
                    break ;                 
                }

                prev_ = value ;
            }

        private:
            bool value_ ;
            bool prev_ ;
            ButtonType type_ ;
        } ;
    }
}
