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

            void setDebugLevel(int v) {
                debug_level_ = v;
            }

            void startMessage(MessageType mt);
            void startMessage(MessageType mt, int level);
            SimulatorMessages &operator<<(const std::string &str);
            SimulatorMessages &operator<<(int);
            SimulatorMessages &operator<<(double);
            SimulatorMessages &operator<<(uint64_t) ;
            SimulatorMessages &operator<<(unsigned int) ;
            void endMessage(int64_t t);

        private:
            MessageType mt_;
            std::string msg_;
            int level_;

            std::list<std::shared_ptr<SimulatorMessageSink>> sinks_;
            int debug_level_;
        };
    }
}