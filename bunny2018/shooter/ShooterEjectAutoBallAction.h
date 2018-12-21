#pragma once

#include "ShooterAction.h"
#include "ShooterEjectOneBallAction.h"
#include <DriveBase.h>

namespace xero {
    namespace bunny2018 {

        /// \brief Run the belt at reduced speed until a ball is detected by the sensor, then stop the belt (motor).
        class ShooterEjectAutoBallAction : public ShooterEjectOneBallAction {

        public:
            ShooterEjectAutoBallAction(Shooter& shooter) ;
            virtual ~ShooterEjectAutoBallAction() ;

            virtual void start() ;

            void addCrateLocation(double v) {
                crates_.push_back(v) ;
                std::sort(crates_.begin(), crates_.end()) ;
            }

        protected:
            virtual bool ejectTrigger() ;
            virtual bool repeatEjectOne() ;

			double getClosestCrate(double pos, bool fwd) ;

        private:
            std::shared_ptr<xero::base::DriveBase> db_ ;

            double base_location_ ;
            std::vector<double> crates_ ;
            std::vector<bool> filled_ ;

			double offset_ ;
			double window_ ;
			double crate_width_ ;
        };
    }
}