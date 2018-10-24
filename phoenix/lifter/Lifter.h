#pragma once

#include <Subsystem.h>
#include <memory>
#include <Solenoid.h>
#include "VictorSP.h"
#include "DigitalInput.h"
#include "Encoder.h"
#include "LifterAction.h"

namespace xero {
	namespace phoenix {
		/// \brief this is the lifter subsystem
		class Lifter : public xero::base::Subsystem {
			friend class LifterCalibrateAction;	
			friend class LifterGoToHeightAction;
			friend class LifterSetDutyCycleAction;
			friend class LifterMaintainHeightAction;
			friend class LifterChangeGearAction;
		public:

			enum class LIFTER_GEAR {LOW, HIGH};
			Lifter(xero::base::Robot& robot);
			virtual ~Lifter();
			virtual void run();
			virtual void computeState() ;

			double getCurrentHeight();
			double getCurrentVelocity();
		protected:
			/// \brief Determine if the Lifter subsystem can accept the given action.
            /// For this subsystem the only critera is that the action be derived from
            /// the LifterAction type.
			virtual bool canAcceptAction(xero::base::ActionPtr Action) ;
		
		
		
		private:
			int encoder_ticks_;
			double height_; //Inches
			double previous_height_; //Inches			//Height in the previous robot loop
			double velocity_;
			LIFTER_GEAR current_gear_;
			bool calibrate_flag_;
			enum class CALIBRATED_STATUS {UNCALIBRATED, CALIBRATED, DECALIBRATED};
			CALIBRATED_STATUS calibrated_;
			bool brake_applied_;
			void calibrate();
			void setBrake(bool value);
			void setGear(LIFTER_GEAR value);
			void setMotorsDutyCycle(double value);
			std::vector<std::shared_ptr<frc::VictorSP>> motors_;
			std::shared_ptr<frc::Encoder> encoder_;
			std::shared_ptr<frc::Solenoid> brake_solenoid_, gear_solenoid_;
			std::shared_ptr<frc::DigitalInput> upper_limit_sensor_;
			std::shared_ptr<frc::DigitalInput> lower_limit_sensor_;
		};
	}
}