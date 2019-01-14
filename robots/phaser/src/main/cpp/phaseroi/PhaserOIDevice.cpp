#include "PhaserOIDevice.h"
#include "PhaserOISubsystem.h"
#include "Phaser.h"
#include "phaserrobotsubsystem/PhaserRobotSubsystem.h"
#include <ActionSequence.h>

using namespace xero::base ;

namespace xero {
    namespace phaser {
        PhaserOIDevice::PhaserOIDevice(PhaserOISubsystem &sub, int index) : OIDevice(sub, index) {
            initialize() ;
        }

        PhaserOIDevice::~PhaserOIDevice() {
        }
        
        void PhaserOIDevice::initialize() {
        }

        
        int PhaserOIDevice::getAutoModeSelector() {
            return -1 ;
        }

        void PhaserOIDevice::createActions() {
        }

        void PhaserOIDevice::generateActions(ActionSequence &seq) {
        }
    }
}
