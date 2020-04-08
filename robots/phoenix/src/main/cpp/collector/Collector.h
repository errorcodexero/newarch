#pragma once

#include <Subsystem.h>
#include <singlemotorsubsystem/SingleMotorSubsystem.h>
#include <DebounceBoolean.h>
#include <frc/DigitalInput.h>

namespace xero {
    namespace phoenix {
        class Intake ;
        class Grabber ;

        class Collector : public xero::base::Subsystem {
            friend class CollectCubeAction ;
        public:
            Collector(xero::base::Subsystem *parent) ;
            virtual ~Collector() ;

            virtual bool canAcceptAction(xero::base::ActionPtr action) ;
            virtual void computeMyState() ;

            std::shared_ptr<Grabber> getGrabber() {
                return grabber_ ;
            }

            std::shared_ptr<xero::base::SingleMotorSubsystem> getIntake() {
                return intake_ ;
            }

            bool hasCube() {
                return has_cube_ ;
            }

            bool collectedCube() {
                return collected_cube_ ;
            }

            void createNamedSequences() {
            }

        private:
            void setCollectedCubeState(bool st) {
                collected_cube_ = st ;
            }

        private:
            //
            // This is the raw cube state, does not mean a cube is collected
            //
            bool has_cube_ ;

            //
            // This means we have collected a cube
            //
            bool collected_cube_ ;
            std::shared_ptr<xero::base::SingleMotorSubsystem> intake_ ;
            std::shared_ptr<Grabber> grabber_ ;
            std::shared_ptr<frc::DigitalInput> sensor_ ;
            std::shared_ptr<xero::misc::DebounceBoolean> deb_sensor_ ;
        } ;
    }
}
