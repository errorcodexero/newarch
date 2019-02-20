#pragma once

#include <string>

namespace xero {
    namespace sim {
        class SimEvent {
        public:
            SimEvent(const std::string &model, const std::string &name, double time, int value) {
                model_ = model ;
                name_ = name ;
                time_ = time ;
                value_ = value ;
            }

            const std::string &getModel() const {
                return model_ ;
            }

            const std::string &getName() const {
                return name_ ;
            }

            double getTime() const {
                return time_ ;
            }

            int getValue() const {
                return value_ ;
            }

        private:
            std::string model_ ;
            std::string name_ ;
            double time_ ;
            int value_ ;
        } ;
    }
}