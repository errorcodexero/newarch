#include <frc/DriverStation.h>
#include <frc/RobotSimBase.h>

using namespace xero::sim ;

namespace frc {
    DriverStation *DriverStation::theOne ;

    DriverStation::DriverStation() : sticks_(3) {
        RobotSimBase &sim = RobotSimBase::getRobotSimulator() ;
        sim.connect(this) ;
    }

    DriverStation::~DriverStation() {       
    }
    
    int DriverStation::GetStickAxisCount(int which) {
        std::lock_guard<std::mutex> lock(getLockMutex()) ;
        return sticks_[which].getAxisCount() ;
    }

    double DriverStation::GetStickAxis(int which, int i) {
        std::lock_guard<std::mutex> lock(getLockMutex()) ;        
        return sticks_[which].getAxisValue(i) ;
    }

    int DriverStation::GetStickButtonCount(int which) {
        std::lock_guard<std::mutex> lock(getLockMutex()) ;        
        return sticks_[which].getButtonCount() ;
    }

    bool DriverStation::GetStickButton(int which, int i) {
        std::lock_guard<std::mutex> lock(getLockMutex()) ;        
        return sticks_[which].getButtonValue(i) ;
    }

    int DriverStation::GetStickPOVCount(int which) {
        std::lock_guard<std::mutex> lock(getLockMutex()) ;        
        return sticks_[which].getPOVCount() ;
    }   

    int DriverStation::GetStickPOV(int which, int i) {
        std::lock_guard<std::mutex> lock(getLockMutex()) ;
        return sticks_[which].getPOVValue(i) ;
    }         

    void DriverStation::ReportError(const char *msg) {
        std::cout << msg << std::endl ;
    }

    std::string DriverStation::GetGameSpecificMessage() const {
        return game_data_ ;
    }
}
