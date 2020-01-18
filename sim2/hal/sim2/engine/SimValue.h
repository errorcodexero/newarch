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
                Invalid
            };

        public:
            SimValue() {
                type_ = ValueType::Invalid;
            }

            SimValue(const SimValue &sv) {
                type_ = sv.type_;
                integer_ = sv.integer_;
                double_ = sv.double_;
                string_ = sv.string_;
                boolean_ = sv.boolean_;
            }

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

            virtual ~SimValue() {                
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

            bool isInvalid() const {
                return type_ == ValueType::Invalid;
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

            bool getBoolean() const {
                return boolean_ ;
            }

            std::string toString() const {
                std::string ret ;

                if (isInvalid())
                    ret = "**INVALID**";
                else if (isBoolean())
                    ret = std::string("boolean[") + (getBoolean() ? "true" : "false") + std::string("]");
                else if (isInteger())
                    ret = std::string("integer[") + std::to_string(getInteger()) + std::string("]");
                else if (isDouble())
                    ret = std::string("double[") + std::to_string(getDouble()) + std::string("]");
                else if (isString())
                    ret = std::string("string[") + getString() + std::string("]");
                else
                    ret = "**CORRUPT**";

                return ret;
            }

        private:
            ValueType type_;
            double double_;
            int integer_;
            std::string string_;
            bool boolean_ ;
        } ;

        inline std::ostream &operator<<(std::ostream &strm, const SimValue &v)
        {
            strm << v.toString();
            return strm;
        }
    }
}