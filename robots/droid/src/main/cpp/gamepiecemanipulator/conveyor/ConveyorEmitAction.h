#pragma once

#include "ConveyorStateAction.h"
#include <vector>
#include <string>

namespace xero {
    namespace droid {
        /// An action which emits a single ball.
        class ConveyorEmitAction : public ConveyorStateAction {
        public:
            ConveyorEmitAction(Conveyor &subsystem);

            void start() override ;

            // Stops the action after the current shot completes.
            void stopFiring() { shouldStopFiring_ = true; }

        protected:
            void conveyorActionStarted() override ;
            void conveyorActionFinished() override ;
            void conveyorActionRun() override ;

        private:
            bool shouldStopFiring_;
            int plot_id_ ;

            static std::vector<std::string> columns_ ;
        } ;
    }
}