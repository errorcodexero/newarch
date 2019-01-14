#pragma once

#include "OIItem.h"
#include <iostream>

/// \file


namespace xero {
    namespace base {
        /// \brief a switch that gets input from a joystick axis (analog input)
        class OIAxisSwitch : public OIItem {
        public:
            /// \brief create the new switch
            /// \param item the item number for the switch
            /// \param number the number of positions for the switch (generally 2 or 3)
            OIAxisSwitch(int item, int number) : OIItem(item) {
                value_ = 0 ;
                number_ = number ;
            }

            /// \brief return the valiue of the switch
            /// \returns the value of the switch
            virtual int getValue() const {
                return value_ ;
            }

            /// \brief set the value from the drivestation
            /// \param ds the drive station object
            /// \param index the index of the joystick
            virtual void setValue(frc::DriverStation &ds, int index) {
                setValue(ds.GetStickAxis(index, getItem())) ;
            }

private:
            void setValue(double value) {
                const double maxvalue = 1.0 ;
                const double minvalue = -1.0 ;
                const double range = maxvalue - minvalue ;
                const double slice = range / number_ ;

                double v = minvalue + slice ;
                value_ = number_ - 1 ;
                for(int i = 0 ; i < number_ ; i++) {
                    if (value < v) {
                        value_ = i ;
                        break ;
                    }
                    v += slice ;
                }
            }    

        private:
            int value_ ;
            int number_ ;
        } ;             
    }
}