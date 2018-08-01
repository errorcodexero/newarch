#pragma once

#include "GrabberAction.h" 
#include <list>


namespace xero {
    namespace phoenix {
         class GrabberCalibrateAction : public xero::base::GrabberAction {
         public:
            GrabberCalibrateAction(Grabber &grabber);
            virtual ~GrabberCalibrateActon(); 

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

        private:
            double calibratepower_;
            int count_;
            int diff_;
            std::list<int> ticks_;
        };
    }
}