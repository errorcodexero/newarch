#pragma once

#include "OIItem.h"

namespace xero {
    namespace base {
        class OIButton : public OIItem {
        public:
            enum class ButtonType
            { 
                Level, 
                LevelInv, 
                LowToHigh, 
                HighToLow
            } ;     

        public:
            OIButton(int item, ButtonType btype) : OIItem(item) {
                value_ = 0 ;
                type_ = btype ;
            }

            ButtonType getButtonType() const {
                return type_ ;
            }

            virtual int getValue() const {
                return static_cast<int>(value_) ;
            }

			virtual void setValue(frc::DriverStation &ds, int index) {
				setValue(ds.GetStickButton(index, getItem())) ;
			}

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
