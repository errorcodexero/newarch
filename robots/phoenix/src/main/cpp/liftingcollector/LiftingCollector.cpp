#include "LiftingCollector.h"
#include "phlifter/Lifter.h"
#include "collector/Collector.h"
#include <actions/SequenceAction.h>
#include <Robot.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace phoenix {
        LiftingCollector::LiftingCollector(Subsystem *parent) : Subsystem(parent, "liftingcollector") {
            lifter_ = std::make_shared<Lifter>(parent) ;
            addChild(lifter_) ;
            
            collector_ = std::make_shared<Collector>(parent) ;
            addChild(collector_) ;
        }

        LiftingCollector::~LiftingCollector() {
        }

        bool LiftingCollector::canAcceptAction(ActionPtr action) {
            return true ;
        }

        void LiftingCollector::computeMyState() {
            lifter_->computeMyState() ;
            collector_->computeMyState() ;
        }
    }
}