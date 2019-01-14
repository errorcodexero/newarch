#include "PIDACtrl.h"

namespace xero{
    namespace misc{

        PIDACtrl::PIDACtrl(SettingsParser &parser, const std::string &kv, const std::string &ka, const std::string &kp, const std::string &kd){
            kp_ = parser.getDouble(kp);
            kd_ = parser.getDouble(kd);
            kv_ = parser.getDouble(kv);
            ka_ = parser.getDouble(ka) ;
            last_error_ =  0 ;
        }

        PIDACtrl::PIDACtrl(double kv, double ka, double kp, double kd):kv_(kv), ka_(ka), kp_(kp), kd_(kd) {
            last_error_ = 0 ;
        }

        double PIDACtrl::getOutput(double a, double v, double dtarget, double dactual, double dt){
            double current_error = dtarget - dactual ;
            double output = v * kv_ + a * ka_ + last_error_ * kp_ + ((current_error - last_error_) / dt -v) * kd_ ;
            last_error_ = current_error ;
            return output ;
        }
    }
}