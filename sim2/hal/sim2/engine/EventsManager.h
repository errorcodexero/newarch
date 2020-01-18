#pragma once

#include <string>

namespace xero
{
    namespace sim2
    {
        class EventsManager
        {
        public:
            EventsManager() ;
            virtual ~EventsManager() ;

            bool loadEvents(const std::string &simfile) ;

        private:
        } ;
    }
}