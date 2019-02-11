#pragma once

#include "phaseroi/PhaserOISubsystem.h"
#include "hatchholder/HatchHolder.h"
#include "cargoholder/CargoHolder.h"
// #include "hatchintake/HatchIntake.h"
#include "cargointake/CargoIntake.h"
#include "turntable/TurnTable.h"
#include <lifter/Lifter.h>
#include <Subsystem.h>

namespace xero {
    namespace phaser {

        class GamePieceManipulator : public xero::base::Subsystem {
        public:
            GamePieceManipulator(xero::base::Robot &robot) ;
            virtual ~GamePieceManipulator() ;

            std::shared_ptr<HatchHolder> getHatchHolder() {
                return hatch_holder_ ;
            }

            // std::shared_ptr<HatchIntake> getHatchIntake() {
            // return hatch_intake_ ;
            // }

            std::shared_ptr<CargoHolder> getCargoHolder(){
                return cargo_holder_ ;
            } 

            std::shared_ptr<CargoIntake> getCargoIntake() {
                return cargo_intake_ ;
            }

            std::shared_ptr<xero::base::Lifter> getLifter() {
                return lifter_ ;
            }

            std::shared_ptr<Turntable> getTurntable() {
                return turntable_ ;
            }

        private:
            std::shared_ptr<HatchHolder> hatch_holder_ ;
            // std::shared_ptr<HatchIntake> hatch_intake_ ;
            std::shared_ptr<CargoHolder> cargo_holder_ ;
            std::shared_ptr<CargoIntake> cargo_intake_ ;
            std::shared_ptr<xero::base::Lifter> lifter_ ;
            std::shared_ptr<Turntable> turntable_ ;
        } ;
    }
}