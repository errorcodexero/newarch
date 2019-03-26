#include "WaitForHatch.h"
#include "carloshatch/CarlosHatch.h"

using namespace xero::base ;

namespace xero {
    namespace phaser {
        WaitForHatch::WaitForHatch(GamePieceManipulator &subsystem, bool present):GamePieceAction(subsystem)
        {
            present_ = present ;
        }

        WaitForHatch::~WaitForHatch(){
        }

        void WaitForHatch::start() {
        }

        void WaitForHatch::run() {
        }

        bool WaitForHatch::isDone() {
            auto hatchholder = getGamePiece().getHatchHolder() ;
            return hatchholder->hasHatch() == present_ ;
        }

        void WaitForHatch::cancel() {
        }

        std::string WaitForHatch::toString() {
            std::string ret("WaitForHatch") ;
            return ret ;
        }
    }
}