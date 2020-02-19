#pragma once

#include "ClimberAction.h"

namespace xero
{
    namespace droid
    {
        class ClimberTraverseAction : public ClimberAction
        {
        public:
            ClimberTraverseAction(Climber &subsystem, double p) ;
            ClimberTraverseAction(Climber &subsystem, const std::string &name) ;
            virtual ~ClimberTraverseAction() ;

            virtual void start() ;

            virtual void run() ;

            virtual std::string toString() ;

        private:
            double power_ ;

        } ;
    }
}
