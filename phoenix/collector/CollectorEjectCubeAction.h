#pragma once

#include "CollectorAction.h" 

namespace xero {
    namespace phoenix {
        class Collector;
        class CollectorEjectCubeAction : public xero::phoenix::CollectorAction {
            public:
                CollectorEjectCubeAction(Collector &collector);

                virtual ~CollectorEjectCubeAction(); 

                
                /// \brief Start the calibrate action.
                virtual void start() ;

                /// \brief Run the duty cycle action.  This method does nothing.            
                virtual void run() ;

                /// \brief Signals if this action is done, always returs true    
                virtual bool isDone() ;

                /// \brief Canel the current action, stops the motors and returns true
                virtual void cancel() ;

                /// \brief Returns a human readable string for the action
                virtual std::string toString() ;

            protected:
               
            private:
                double start_time_;
                double outrun_time_;
                bool isdone_;

                xero::base::ActionPtr intake_out_;
                xero::base::ActionPtr intake_off_;
               
        };
    }
}