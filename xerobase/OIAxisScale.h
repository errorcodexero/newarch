#pragma once

#include "OIItem.h"
#include <vector>
#include <cstdlib>

namespace xero {
    namespace base {
        class OIAxisScale : public OIItem {
        public:
            OIAxisScale(int item, const std::vector<double> &mapping) : OIItem(item) {
                value_ = 0 ;
                mapping_ = mapping ;
            }

            virtual int getValue() const {
                return value_ ;
            }

			virtual void setValue(frc::DriverStation &ds, int index) {
				setValue(ds.GetStickAxis(index, getItem())) ;
			}

            void setValue(int value) {
                value_ = value ;
            }

            void setValue(double value) {
                value_ = mapping_.size() ;
                for(size_t i = 0 ; i < mapping_.size() ; i++) {
                    if (value <= mapping_[i]) {
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