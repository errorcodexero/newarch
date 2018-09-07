#include "CollectorEjectCubeAction.h"
#include "Collector.h"
#include "Robot.h"
#include "intake/IntakeDutyCycleAction.h"
#include "intake/Intake.h"

using namespace xero::base;

namespace xero {
    namespace phoenix {
        
        CollectorEjectCubeAction::CollectorEjectCubeAction(Collector &collector) : CollectorAction(collector){

            Intake& intake = *(getCollector().getIntake());

            outrun_time_ = getCollector().getRobot().getSettingsParser().getDouble("collector:outrun_time");

            intake_out_ = std::make_shared<IntakeDutyCycleAction>(intake, "collector:intake_duty_cycle");
            intake_off_ = std::make_shared<IntakeDutyCycleAction>(intake, 0);

        }
        CollectorEjectCubeAction::~CollectorEjectCubeAction(){

        }
        
           void CollectorEjectCubeAction::start() {
               getCollector().getIntake()->setAction(intake_out_);
               start_time_ =getCollector().getRobot().getTime();

               isdone_ = false;
           }

            void CollectorEjectCubeAction::run() {
                double elapsed_time_ = getCollector().getRobot().getTime() - start_time_;

                if (elapsed_time_ > outrun_time_){
                    getCollector().getIntake()->setAction(intake_off_);
                    isdone_ = true;
                }
            }

            bool CollectorEjectCubeAction::isDone() {
                
                return isdone_;
            }

            void CollectorEjectCubeAction::cancel() {
                isdone_ = true;
            }

            std::string CollectorEjectCubeAction::toString() {

            }


    }
}