#include "Grabber.h"
#include "GrabberCalibrateAction.h"
#include "phoenixids.h"
#include <Robot.h>
#include <actions/SequenceAction.h>
#include <MessageLogger.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace phoenix {
        Grabber::Grabber(xero::base::Subsystem *parent) : MotorEncoderSubsystem(parent, "grabber", MSG_GROUP_GRABBER) {
            min_angle_ = getRobot().getSettingsParser().getDouble("grabber:angle:minimum") ;
            max_angle_ = getRobot().getSettingsParser().getDouble("grabber:angle:maximum") ;            
        }

        Grabber::~Grabber() {
        }   
    }
}
