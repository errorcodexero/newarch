#include "ranseurids.h"
#include "TubManipulatorSubsystem.h"
#include <Robot.h>
#include <list>
#include <TubIntake/Collector.h>
#include "TubManipulatorSubsystem.h"
#include "Action.h"

//using namespace xero::ranseur::Collector ;
using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace ranseur {
        TubManipulatorSubsystem::TubManipulatorSubsystem(Robot &robot) : RobotSubsystem(robot, "ranseur") {

        collector_ = std::make_shared<Collector>(robot) ;
            addChild(collector_) ;                    
        }

        bool TubManipulatorSubsystem::canAcceptAction(ActionPtr act) {
            return false;
        }

        void TubManipulatorSubsystem::reset() {

        }

/*
        void TubManipulatorSubsystem::init(LoopType ltype) {           //loop type to be specified
            
        }
*/
        void TubManipulatorSubsystem::run() {
            
        }

        void TubManipulatorSubsystem::cancelAction() {
            
        }
   
        bool TubManipulatorSubsystem::setAction(xero::base::ActionPtr action, bool force) {      //action to be specified
            return false ;      
        }

        void TubManipulatorSubsystem::computeState() {
            
        }

        std::string TubManipulatorSubsystem::toString() {
            return " " ;
        }

    }
}
