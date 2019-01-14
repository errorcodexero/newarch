#pragma once

#include "OIItem.h"
#include <vector>
#include <cstdlib>

/// \file


namespace xero {
    namespace base {
        /// \brief An OI item that maps a joystick axis to a set of integer values based on a mapping array
        class OIAxisScale : public OIItem {
        public:
            /// \brief create the new object
            /// The mapping array provides the mapping of the axis value to the integer
            /// value.  The returned value is the index where the axis value is less than
            /// the mapping value in the slow.  If the mapping array is -1.0, 0.0, 1.0 and the
            /// axis value is -0.5, then 1 is returned because index 1 in the mapper array, value
            /// 0.0 is the place where the -0.5 axis value is less than the mapping value.  If the
            /// axis value is greater than all mapping values, then N  is returned where N is the
            /// number of entries in the mapping array.
            /// \param item the item number for the item
            /// \param mapping the mapping of axis values to integer values
            OIAxisScale(int item, const std::vector<double> &mapping) : OIItem(item) {
                value_ = 0 ;
                mapping_ = mapping ;
            }

            /// \brief return the value of the OI item
            /// \returns the value of this item
            virtual int getValue() const {
                return value_ ;
            }

            /// \brief set the value of the item
            /// \param ds the drive station object to get joystick values
            /// \param index the index of the joystick to read
            virtual void setValue(frc::DriverStation &ds, int index) {
                setValue(ds.GetStickAxis(index, getItem())) ;
            }

        private:
            void setValue(double v) {
                value_ = mapping_.size() ;
                for(size_t i = 0 ; i < mapping_.size() ; i++) {
                    if (v <= mapping_[i]) {
                        value_ = static_cast<int>(i) ;
                        break ;
                    }
                }
            }

        private:
            int value_ ;
            std::vector<double> mapping_ ;
        } ;       
    }
}