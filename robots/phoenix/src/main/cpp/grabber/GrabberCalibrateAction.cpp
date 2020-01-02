#include "GrabberCalibrateAction.h"
#include "Grabber.h"
#include "phoenixids.h"
#include <MessageLogger.h>
#include <Robot.h>
#include <algorithm>
#include <numeric>
#include <cassert>

using namespace xero::misc ;

namespace xero {
    namespace phoenix {

        std::string GrabberCalibrateAction::action_name("GrabberCalibration") ;

        GrabberCalibrateAction::GrabberCalibrateAction(Grabber &g) : GrabberAction(g) {
        }

        GrabberCalibrateAction::~GrabberCalibrateAction() {            
        }

        void GrabberCalibrateAction::start() {
            GrabberAction::start();

            is_calibrated_ = false;
        }

        void GrabberCalibrateAction::run() {
            GrabberAction::run();

            if (!is_calibrated_)
            {
                assert(false) ;
            }
        }

        void GrabberCalibrateAction::cancel() {
            GrabberAction::cancel();
            setDone();
        }
    }
}