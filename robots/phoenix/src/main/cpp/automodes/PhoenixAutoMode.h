#pragma once

#include "Phoenix.h"
#include <AutoMode.h>

namespace xero {
    namespace phoenix {
        class PhoenixAutoMode : public xero::base::AutoMode {
        public:
            PhoenixAutoMode(Phoenix &robot, const std::string &name) ;
            virtual ~PhoenixAutoMode() ;

        protected:
            void calibrate() ;
            void driveStraight(const std::string &name) ;
            void rotate(const std::string &name) ;
            void rotate(double angle) ;
            void lift(const std::string &name, bool block) ;
            void eject(const std::string &name) ;
            void driveAndCollect(const std::string &name) ;
            void followPath(const std::string &name) ;

            Phoenix &getRobot() {
                return robot_ ;
            }

            void addAction(xero::base::SubsystemPtr sub, xero::base::ActionPtr act, bool block = true) ;
            void addAction(xero::base::ActionPtr act) ;

        private:
            std::list<xero::base::AutoModePtr> stack_ ;
            Phoenix &robot_ ;
        } ;
    }
}
