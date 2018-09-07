#pragma once

#include "SimulatedObject.h"
#include <iostream>
#include <vector>

namespace frc
{
    class DriverStation : public xero::sim::SimulatedObject {
        friend class xero::sim::RobotSimBase ;

	public:
		class Stick {
		public:
			Stick() : axis_(10, 0.0), buttons_(10, false), pov_(10, -1) {
			}

			virtual ~Stick() {
			}

			int getAxisCount() {
				return axis_.size() ;
			}

			double getAxisValue(int which) {
				return axis_[which] ;
			}
			
			void setAxisValue(int which, double value) {
				axis_[which] = value ;
			}

			int getButtonCount() {
				return buttons_.size() ;
			}

			bool getButtonValue(int which) {
				return buttons_[which] ;
			}

			void setButtonValue(int which, bool value) {
				buttons_[which] = value ;
			}

			int getPOVCount() {
				return pov_.size() ;
			}

			int getPOVValue(int which) {
				return pov_[which] ;
			}

			void setPOVValue(int which, int value) {
				pov_[which] = value ;
			}

		private:
			std::vector<double> axis_ ;
			std::vector<bool> buttons_ ;
			std::vector<int> pov_ ;			
		} ;

    public:


        static DriverStation &GetInstance() {
            if (theOne == nullptr)
                theOne = new DriverStation() ;

            return *theOne ;
        }

        int GetStickAxisCount(int which) ;
        double GetStickAxis(int which, int i) ;
        int GetStickButtonCount(int which) ;
        bool GetStickButton(int which, int i) ;
        int GetStickPOVCount(int which) ;
        int GetStickPOV(int which, int i) ;
		void ReportError(const char *msg) ;
		std::string GetGameSpecificMessage() const ;

		Stick &getStick(int which) {
			return sticks_[which] ;
		}

		size_t getStickCount() {
			return sticks_.size() ;
		}

		double GetBatteryVoltage() const {
			return 12.0;
		}

    private:
        DriverStation() ;
		virtual ~DriverStation() ;
		
    private:
        static DriverStation *theOne ;

	private:
		std::vector<Stick> sticks_ ;
    } ;
}
