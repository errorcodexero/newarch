#include "CollectorCollectCubeAction.h"
#include "Collector.h"
#include "Robot.h"
#include "intake/Intake.h"
#include "intake/IntakeDutyCycleAction.h"
#include "grabber/Grabber.h"
#include "grabber/GrabberToAngleAction.h"

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
                // Get references to subsystems
                Intake& intake = *(getCollector().getIntake());
                Grabber& grabber = *(getCollector().getGrabber());

                if (state_ == State::reset){
                    state_ = State::open_grabber;

                    // Set action to run intake
                    double intake_speed = getCollector().getRobot().getSettingsParser().getDouble("intake:speed:collect");
                    auto intake_action = std::make_shared<IntakeDutyCycleAction>(intake, intake_speed);
                    intake.setAction(intake_action);

                    // Set action to open grabber
                    double grabber_angle = getCollector().getRobot().getSettingsParser().getDouble("grabber:angle:collect");
                    auto grabber_action = std::make_shared<GrabberToAngleAction>(grabber, grabber_angle);
                    grabber.setAction(grabber_action);
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