#pragma once

#include <string>
#include <list>
#include <memory>

namespace xero
{
    namespace sim2
    {
        class SimulatorMessageSink ;

        class SimulatorMessages
        {
        public:
            enum class MessageType
            {
                Debug,
                Info,
                Warning,
                Error
            };


        public:
            SimulatorMessages();
            virtual ~SimulatorMessages();

            void addSink(std::shared_ptr<SimulatorMessageSink> sink) {
                sinks_.push_back(sink);
            }

            void startMessage(MessageType mt);
            SimulatorMessages &operator<<(const std::string &str);
            SimulatorMessages &operator<<(int);
            SimulatorMessages &operator<<(double);
            void endMessage(double t);

        private:
            MessageType mt_;
            std::string msg_;
            std::list<std::shared_ptr<SimulatorMessageSink>> sinks_;
        };
    }
}