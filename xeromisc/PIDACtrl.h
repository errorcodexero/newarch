#pragma once
#include "SettingsParser.h"


namespace xero {
    namespace misc{

        class PIDACtrl{

            public:

            PIDACtrl(SettingsParser &parser, const std::string &kv, const std::string &ka, const std::string &kp, const std::string &kd, double dt);

            PIDACtrl(double kv, double ka, double kp, double kd, double dt);

            double getOutput(double a, double v, double dtarget, double dactual);

            private:

            double kv_;
            double ka_;
            double kp_;
            double kd_;
            double last_error_;
            double dt_;



        };

    }

}