#include "CollectorCollectCubeAction.h"
#include "Collector.h"
#include <Robot.h>

using namespace xero::base;

namespace xero {
    namespace phoenix {
            CollectorCollectCubeAction::CollectorCollectCubeAction(Collector &collector) : CollectorAction(collector) {

            grab_time_ = collector.getRobot().getSettingsParser().getDouble("collector:grab_time");
            state_ = State::reset;

            }

            CollectorCollectCubeAction::~CollectorCollectCubeAction(){

            }

            void CollectorCollectCubeAction::start() {
            }
          
            void CollectorCollectCubeAction::run() {

            }

            bool CollectorCollectCubeAction::isDone() {
                return true;
            }

            void CollectorCollectCubeAction::cancel() {

            }

            std::string CollectorCollectCubeAction::toString() {

            }
    }
}