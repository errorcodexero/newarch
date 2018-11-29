#pragma once

#include <Subsystem.h>
#include <tankdrive/TankDrive.h>
#include <MessageListener/MessageListener.h>

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
            std::shared_ptr<xero::base::MessageListener> getMessageListener() {
                return ml_ ;
            }

        private:
            std::shared_ptr<xero::base::TankDrive> db_ ;
            std::shared_ptr<BunnyOISubsystem> oi_ ;
            std::shared_ptr<xero::base::MessageListener> ml_ ;
        } ;
    }
}
