#include "SmartDashboardAction.h"
#include <frc/smartdashboard/SmartDashboard.h>

namespace xero {
    namespace base {
        void SmartDashboardAction::start() {
            switch(dt_)
            {
            case DataType::Boolean:
                frc::SmartDashboard::PutBoolean(name_, boolvalue_) ;
                break ;

            case DataType::Number:
                frc::SmartDashboard::PutNumber(name_, number_) ;
                break ;            

            case DataType::String:
                frc::SmartDashboard::PutString(name_, str_) ;
                break ;             
            }
        }

        void SmartDashboardAction::run() {            
        }

        void SmartDashboardAction::cancel() {            
        }

        bool SmartDashboardAction::isDone() {
            return true ;
        }

        std::string SmartDashboardAction::toString() {
            std::string ret = "SmartDashboardAction " ;
            ret += name_ = " = " ;
            switch(dt_)
            {
            case DataType::Boolean:
                ret += "bool (" ;
                ret += (boolvalue_ ? "true" : "false") ;
                ret += ")" ;
                break ;

            case DataType::Number:
                ret += "number (" ;
                ret += std::to_string(number_) ;
                ret += ")" ;
                break ;            

            case DataType::String:
                ret += "string (" ;
                ret += str_ ;
                ret += ")" ;            
                break ;             
            }

            return ret ;
        }
    }
}