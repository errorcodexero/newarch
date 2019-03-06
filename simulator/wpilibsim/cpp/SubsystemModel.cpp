#include <frc/SubsystemModel.h>
#include <frc/RobotSimBase.h>
#include <cctype>

using namespace xero::misc ;

namespace xero {
    namespace sim {
        SubsystemModel::SubsystemModel(RobotSimBase &simbase, const std::string &name) : simbase_(simbase) {
            name_ = name ;
        }

        SubsystemModel::~SubsystemModel() {
        }

        void SubsystemModel::generateDisplayInformation(std::list<std::string> &lines) {
            lines.push_back("  No Data") ;            
        }

        std::mutex &SubsystemModel::getLockMutex() {
            RobotSimBase &sim = RobotSimBase::getRobotSimulator() ;
            return sim.getLockMutex() ;            
        }

        MessageLogger &SubsystemModel::getRobotMessageLogger() {
            return simbase_.getRobotMessageLogger() ;
        }
    }
}
