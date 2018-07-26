#include "DriverStation.h"
#include "RobotSimBase.h"

using namespace xero::sim ;

namespace frc {
    DriverStation *DriverStation::theOne ;
    
    DriverStation::DriverStation() {
        RobotSimBase &sim = RobotSimBase::getRobotSimulator() ;
        sim.connect(this) ;
    }
    
    int DriverStation::GetStickAxisCount(int which) {
        std::lock_guard<std::mutex> lock(getLockMutex()) ;
       return 0 ;
    }

    double DriverStation::GetStickAxis(int which, int i) {
        std::lock_guard<std::mutex> lock(getLockMutex()) ;        
        return 0 ;
    }

    int DriverStation::GetStickButtonCount(int which) {
        std::lock_guard<std::mutex> lock(getLockMutex()) ;        
        return 0 ;
    }

    bool DriverStation::GetStickButton(int which, int i) {
        std::lock_guard<std::mutex> lock(getLockMutex()) ;        
        return false ;
    }

	void DriverStation::ReportError(const char *msg) {
		std::cout << msg << std::endl ;
	}
}