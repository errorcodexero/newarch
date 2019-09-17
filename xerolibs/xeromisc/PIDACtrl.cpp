#include "PIDACtrl.h"
#include "xeromath.h"

namespace xero{
    namespace misc{

        PIDACtrl::PIDACtrl(SettingsParser &parser, const std::string &kv, const std::string &ka, 
                                const std::string &kp, const std::string &kd, bool angle){
            kp_ = parser.getDouble(kp);
            kd_ = parser.getDouble(kd);
            kv_ = parser.getDouble(kv);
            ka_ = parser.getDouble(ka) ;
            last_error_ =  0 ;
            angle_ = angle ;

            equ_ = false ;
        }

        PIDACtrl::PIDACtrl(SettingsParser &parser, const std::string &kv1, const std::string &kv2, const std::string &ka, 
                                const std::string &kp, const std::string &kd, bool angle){
            kp_ = parser.getDouble(kp);
            kd_ = parser.getDouble(kd);
            kv1_ = parser.getDouble(kv1);
            kv2_ = parser.getDouble(kv2);

            ka_ = parser.getDouble(ka) ;
            last_error_ =  0 ;
            angle_ = angle ;

            equ_ = true ;
        }

        PIDACtrl::PIDACtrl(double kv, double ka, double kp, double kd, bool angle):kv_(kv), ka_(ka), kp_(kp), kd_(kd) {
            last_error_ = 0 ;
            angle_ = angle ;

            equ_ = false ;
        }

        double PIDACtrl::getOutput(double a, double v, double dtarget, double dactual, double dt){
            double current_error ;
            
            if (angle_)
                current_error = xero::math::normalizeAngleDegrees(dtarget - dactual) ;
            else
                current_error = dtarget - dactual ;

            if (equ_)
                vpart_ = (v - kv2_) / kv1_ ;
            else
                vpart_ = v * kv_ ;
            apart_ = a * ka_ ;
            ppart_ = current_error * kp_ ;
            dpart_ = ((current_error - last_error_) / dt -v) * kd_ ;

            double output = vpart_ + apart_ + ppart_ + dpart_ ;
            last_error_ = current_error ;
            return output ;
        }
    }
}