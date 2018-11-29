#pragma once

#include "bunnysubsystem/BunnySubsystem.h" 
#include "sorterin/SorterIn.h"
#include "sorterout/SorterOut.h"

namespace xero {
    namespace bunny2018 {
		
        class Sorter : public xero::base::Subsystem {

        public:
            Sorter(xero::base::Robot & robot);

            virtual ~Sorter(); 

        private:
            std::shared_ptr<SorterIn> sorter_in_;

            std::shared_ptr<SorterOut> sorter_out_;
            
            std::shared_ptr<frc::VictorSP> motor_;
            std::shared_ptr<frc::Encoder> encoder_;
            
            double min_angle_;
            double max_angle_;
            double degrees_per_tick_;
            double encoder_ticks_;
            double angle_;

            bool calibrated_;


        };
    }
}