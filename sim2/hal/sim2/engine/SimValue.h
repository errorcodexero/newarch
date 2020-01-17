#pragma once

#include <string>
#include <cassert>

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
                String,
                Boolean,
            };

        public:
            SimValue(int v) {
                type_ = ValueType::Integer;
                integer_ = v;
            }

            SimValue(double v) {
                type_ = ValueType::Double;
                double_ = v;
            }

            SimValue(const std::string &v) {
                type_ = ValueType::String;
                string_ = v;
            }

            SimValue(bool v) {
                type_ = ValueType::Boolean ;
                boolean_ = v ;
            }

            bool isInteger() const {
                return type_ == ValueType::Integer ;
            }

            bool isDouble() const {
                return type_ == ValueType::Double ;
            }

            bool isString() const {
                return type_ == ValueType::String ;
            }

            bool isBoolean() const {
                return type_ == ValueType::Boolean ;
            }

            int getInteger() const {
                assert(isInteger()) ;
                return integer_ ;
            }

            double getDouble() const {
                assert(isDouble()) ;
                return double_ ;
            }

            const std::string &getString() const {
                assert(isString()) ;
                return string_ ;
            }

            bool getBoolear() const {
                return boolean_ ;
            }

        private:
            ValueType type_;
            double double_;
            int integer_;
            std::string string_;
            bool boolean_ ;
        } ;
    }
}