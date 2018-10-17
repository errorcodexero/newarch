#include "CollectorCollectCubeAction.h"
#include "Collector.h"
#include "Robot.h"
#include "intake/Intake.h"
#include "intake/IntakeDutyCycleAction.h"
#include "grabber/Grabber.h"
#include "grabber/GrabberToAngleAction.h"
#include "grabber/GrabberHoldCubeAction.h"

using namespace xero::base;

namespace xero {
    namespace phoenix {
            CollectorCollectCubeAction::CollectorCollectCubeAction(Collector &collector) : CollectorAction(collector) {
                Intake& intake = *(getCollector().getIntake());
                Grabber& grabber = *(getCollector().getGrabber());

                grab_time_ = getCollector().getRobot().getSettingsParser().getDouble("collector:grab_time");
                state_ = State::reset;

                grabber_open_ = std::make_shared<GrabberToAngleAction>(grabber, "collector:collect_angle");
                intake_in_ = std::make_shared<IntakeDutyCycleAction>(intake, "collector:intake_duty_cycle");
                hold_cube_ = std::make_shared<GrabberHoldCubeAction>(grabber);
                intake_off_ = std::make_shared<IntakeDutyCycleAction>(intake, 0);
            }

            CollectorCollectCubeAction::~CollectorCollectCubeAction(){

            }
            std::string CollectorCollectCubeAction::stateToString(CollectorCollectCubeAction::State state){
                switch(state){
                    case State::reset: return "reset";
                    case State::wait_for_cube: return "wait for cube";
                    case State::secure_cube: return "secure cube";
                    case State::hold: return "hold";
                    default: return "";
                }
            }

            void CollectorCollectCubeAction::start() {
                getCollector().getIntake()->setAction(intake_in_);
                getCollector().getGrabber()->setAction(grabber_open_);
                state_=State::wait_for_cube;
            }
          
            void CollectorCollectCubeAction::run() {
                // Get references to subsystems
                Intake& intake = *(getCollector().getIntake());
                Grabber& grabber = *(getCollector().getGrabber());
                switch(state_){
                    case State::reset:
                        start() ;
                        break ;
                    case State::wait_for_cube:
                        if(getCollector().hasCube()){
                            grabber.setAction(hold_cube_);
                            start_grab_=getCollector().getRobot().getTime();
                            state_=State::secure_cube;
                        }
                    break;
                    case State::secure_cube:
                        if(!getCollector().hasCube()){
                            start();
                        }
                        else if(getCollector().getRobot().getTime()-start_grab_ > grab_time_){
                            intake.setAction(intake_off_);
                            state_ = State::hold;
                        }
                    break;

                    case State::hold:
                        if (!getCollector().hasCube()) {
                            start() ;
                        }
                    break ;
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