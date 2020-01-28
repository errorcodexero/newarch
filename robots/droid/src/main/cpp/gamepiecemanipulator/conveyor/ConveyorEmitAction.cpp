#include "ConveyorEmitAction.h"
#include "Conveyor.h"

using namespace xero::misc;

namespace xero {
    namespace droid {
        // void ConveyorEmitAction::start() {
        //     if (getSubsystem().ballCount_ == 0) {
        //         setDone();
        //     }
            
        //     if (/* sensor C is clear*/false) {
        //         auto &logger = getMessageLogger();

        //         logger.startMessage(MessageLogger::MessageType::warning, getSubsystem().getMsgID());
        //         logger << "ConveyorEmitAction started when no ball is detected by shooter-side sensor";
        //         logger << "Perhaps ConveyorPrepareToEmitAction didn't get run?";
        //         logger.endMessage();

        //         setDone();
        //     }
        // }

        // void ConveyorEmitAction::run() {
        //     getSubsystem().setMotor(1); // emit
        //     if (/* sensor C is clear */true) {
        //         setDone();
        //         getSubsystem().ballCount_ -= 1;
        //     }
        // }
    }
} 