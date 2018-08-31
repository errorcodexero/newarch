#include "CollectorCollectCubeAction.h"
#include "Collector.h"
#include "Robot.h"
#include "intake/Intake.h"
#include "intake/IntakeDutyCycleAction.h"

using namespace xero::base;

namespace xero {
    namespace phoenix {
            CollectorCollectCubeAction::CollectorCollectCubeAction(Collector &collector) : CollectorAction(collector) {

                grab_time_ = getCollector().getRobot().getSettingsParser().getDouble("collector:grab_time");
                state_ = State::reset;

            }

            CollectorCollectCubeAction::~CollectorCollectCubeAction(){

            }

            void CollectorCollectCubeAction::start() {
            }
          
            void CollectorCollectCubeAction::run() {
                if (state_ == State::reset){
                    state_ = State::open_grabber;
                    double intake_speed = getCollector().getRobot().getSettingsParser().getDouble("intake:speed:collect");
                    auto intake_action = std::make_shared<IntakeDutyCycleAction>(getCollector().getIntake()->Get(), intake_speed);
                    intake_->setAction(intake_action);
                }

            }

            bool CollectorCollectCubeAction::isDone() {
                return true;
            }

            void CollectorCollectCubeAction::cancel() {

            }

            std::string CollectorCollectCubeAction::toString() {
                std::string result = "CollectorCollectCubeAction ";
                result += stateToString(state_);
                return result;
            }
    }
}