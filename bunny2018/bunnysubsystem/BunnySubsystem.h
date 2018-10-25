#pragma once

#include <Subsystem.h>
#include <tankdrive/TankDrive.h>

namespace xero {
    namespace bunny2018 {

        class BunnyOISubsystem ;

        class BunnySubsystem : public xero::base::Subsystem {
        public:
            BunnySubsystem(xero::base::Robot &robot) ;
            virtual ~BunnySubsystem() ;

            std::shared_ptr<xero::base::TankDrive> getDriveBase() {
                return db_ ;
            }

            std::shared_ptr<BunnyOISubsystem> getOI() {
                return oi_ ;
            }

        private:
            std::shared_ptr<xero::base::TankDrive> db_ ;
            std::shared_ptr<BunnyOISubsystem> oi_ ;
        } ;
    }
}