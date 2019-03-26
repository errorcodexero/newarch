#pragma once

#include <AutoMode.h>

namespace xero {
    namespace phaser {
        class PhaserAutoModeBase : public xero::base::AutoMode
        {
        public:
            PhaserAutoModeBase(xero::base::Robot &robot, const std::string &name, const std::string &desc) ;
            virtual ~PhaserAutoModeBase() ;


        protected:
            void insertAutoModeLeg(const std::string &height, const std::string &angle, const std::string &path, 
                                    bool rear, bool hashatch, double delay, xero::base::ActionPtr finish, double vdist = 60.0) ;

        private:
        } ;
    }
}