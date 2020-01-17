#pragma once

#include <string>

namespace xero
{
    namespace sim2
    {
        class SimValue
        {
        public:
            enum class ValueType
            {
                Integer,
                Double,
                String
            };

        public:
            SimValue(int v) {
                type_ = ValueType::Integer;
                integer_ = v;
            }
        private:
            ValueType type_;
            double double_;
            int integer_;
            std::string string_;
        }
    }
}