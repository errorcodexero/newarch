#pragma once

#include <Subsystem.h>
#include <memory>
#include <Solenoid.h>
#include "VictorSP.h"
#include "DigitalInput.h"
#include "Encoder.h"
#include "LiftingCollectorAction.h"
#include "Collector.h"
#include "Lifter.h"

namespace xero {
	namespace phoenix {
		/// \brief this is the LiftingCollector subsystem
		class LiftingCollector : public xero::base::Subsystem {
			friend class LiftingCollectorCalibrateAction;	
			friend class LiftingCollectorGoToHeightAction;
			friend class LiftingCollectorSetDutyCycleAction;
			friend class LiftingCollectorMaintainHeightAction;
			friend class LiftingCollectorChangeGearAction;
		public:
			enum class LiftingCollector_GEAR {LOW, HIGH};
			LiftingCollector(xero::base::Robot& robot);
			virtual ~LiftingCollector();
			virtual void run();
			double getCurrentHeight();
			double getCurrentVelocity();
			virtual void computeState() ;

			std::shared_ptr<Lifter> getLifter(){
                return lifter_;
            }
			std::shared_ptr<Collector> getCollector(){
                return collector_;
            }
		protected:
			/// \brief Determine if the LiftingCollector subsystem can accept the given action.
            /// For this subsystem the only critera is that the action be derived from
            /// the LiftingCollectorAction type.
			virtual bool canAcceptAction(xero::base::ActionPtr Action) ;
		
		
		
		private:
			int encoder_ticks_;
			double height_; //Inches
			double previous_height_; //Inches			//Height in the previous robot loop
			double velocity_;
			LiftingCollector_GEAR current_gear_;
			bool calibrate_flag_;
			enum class CALIBRATED_STATUS {UNCALIBRATED, CALIBRATED, DECALIBRATED};
			CALIBRATED_STATUS calibrated_;
			bool brake_applied_;
			void calibrate();
			void setBrake(bool value);
			void setMotorsDutyCycle(double value);
			std::vector<std::shared_ptr<frc::VictorSP>> motors_;
			std::shared_ptr<frc::Encoder> encoder_;
			std::shared_ptr<frc::Solenoid> brake_solenoid_, gear_solenoid_;
			std::shared_ptr<frc::DigitalInput> upper_limit_sensor_;
			std::shared_ptr<frc::DigitalInput> lower_limit_sensor_;
			std::shared_ptr<Lifter> lifter_;
			std::shared_ptr<Collector> collector_;
		};
	}
}