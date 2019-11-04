#pragma once

#include <frc/SimEvent.h>
#include <string>
#include <vector>

namespace xero {
    namespace sim {
        class SimEventManager {
        public:
            SimEventManager() ;
            virtual ~SimEventManager() ;

            void addEvent(const std::string &model, const std::string &event, double time, int value) ;

            bool readEvents(const std::string &evfile) ;

            size_t size() {
                return events_.size() ;
            }

            double getFirstEventTime() const {
                return events_[0].getTime() ;
            }

            const SimEvent &getFirstEvent() {
                return events_[0] ;
            }

            void removeFirstEvent() {
                events_.erase(events_.begin()) ;
            }

        private:
            void split(const std::string &line, std::vector<std::string> &results) ;

        private:
            std::vector<SimEvent> events_ ;
        } ;
    }
}