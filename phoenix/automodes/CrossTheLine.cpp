#include "CrossTheLine.h"

namespace xero {
    namespace phoenix {
        CrossTheLine::CrossTheLine(Phoenix &robot) : PhoenixAutoMode(robot, "CrossTheLine") {
            create() ;
        }

        CrossTheLine::~CrossTheLine() {
        }

        void CrossTheLine::create() {
            calibrate() ;
            driveStraight("auto:crossline:segment1") ;
        }
    }
}