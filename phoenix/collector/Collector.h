#pragma once

#include <cassert>
#include <memory>
#include "DigitalInput.h"
#include "Subsystem.h" 
#include "intake/Intake.h"
#include "grabber/Grabber.h"
#include "DebounceBoolean.h"

namespace xero {
    namespace phoenix {
		/// \brief This subsystem represents the collector on the robot.
		/// The collector consists of the grabber, the intake, and the
		/// cube detect sensor.
        class Collector : public xero::base::Subsystem {
            friend class CollectorEjectCubeAction;
            friend class CollectorGetCubeAction;

        public:
			/// \brief create the new collector subsystem.
			/// This in turn creates the grabber subsystem and the intake subsystem
			/// \param robot the robot this subsystem belong to
            Collector(xero::base::Robot & robot);

			/// \brief destroys the collector subsystem
            virtual ~Collector(); 

			/// \brief computes the state of the collector subsystem.
			/// Noteable this determines if the collector has a cube by reading the 
			/// cube detect sensor.
            virtual void computeState();

			/// \brief determines if this subsystem can accept the action given
			/// \param action the action to check
			/// \returns true if the subsystem can accept the action, otherwise false
            virtual bool canAcceptAction(xero::base::ActionPtr action) ;

			/// \brief returns true if the collector currently has a cube
			/// \returns true if the collector has a cube.
            bool hasCube(){
                return has_cube_;
            }

			/// \brief returns a shared pointer to the intake subsystem
			/// \returns a shared pointer to the intake subsystem
            std::shared_ptr<Intake> getIntake(){
                return intake_;
            }

			/// \brief returns a shared pointer to the grabber subsystem
			/// \returns a shared pointer to the grabber subsystem
            std::shared_ptr<Grabber> getGrabber(){
                return grabber_;
            }

        private:
			// The intake subsystem
            std::shared_ptr<Intake> intake_;

			// The grabber subsystem
            std::shared_ptr<Grabber> grabber_;

			// The input with a connection to the cube detect sensor
            std::shared_ptr<frc::DigitalInput> sensor_;

			// A debounced version of the cube detect sensor
            std::shared_ptr<xero::misc::DebounceBoolean> debounce_;

			// If true, the collector is holding a cube
            bool has_cube_;
        };
    }
}