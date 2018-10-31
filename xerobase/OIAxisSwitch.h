#pragma once

#include "OIItem.h"

namespace xero {
    namespace base {
        class OIAxisSwitch : public OIItem {
        public:
            OIAxisSwitch(int item, int number) : OIItem(item) {
                value_ = 0 ;
                number_ = number ;
            }

            virtual int getValue() const {
                return value_ ;
            }

            void setValue(int value) {
                value_ = value ;
            }

			virtual void setValue(frc::DriverStation &ds, int index) {
				setValue(ds.GetStickAxis(index, getItem())) ;
			}

            void setValue(double value) {
                const double maxvalue = 1.0 ;
                const double minvalue = -1.0 ;
                const double range = maxvalue - minvalue ;
                const double slice = range / number_ ;

                double v = minvalue + slice ;
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