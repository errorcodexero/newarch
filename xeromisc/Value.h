#pragma once

#include <string>
#include <cassert>
#include <sstream>

namespace xero {
    namespace misc {
        class Value {
        public:
            enum class Type {
                Invalid,
                Integer,
                Real,
                String
            } ;
        public:
            Value() {
                type_ = Type::Invalid ;
            }
            
            Value(const std::string &v) {
                type_ = Type::String ;
                strvalue_ = v ;
            }

            Value(int v) {
                type_ = Type::Integer ;
                intvalue_ = v ;
            }

            Value(double v) {
                type_ = Type::Real ;
                doublevalue_ = v ;
            }

            Type getType() const {
                return type_ ;
            }

            int getInteger() const {
                assert(type_ == Type::Integer) ;
                return intvalue_ ;
            }

            double getReal() const {
                assert(type_ == Type::Real || type_ == Type::Integer) ;
                if (type_ == Type::Integer)
                    return static_cast<double>(intvalue_) ;

                return doublevalue_ ;
            }

            const std::string &getString() const {
                assert(type_ == Type::String) ;
                return strvalue_ ;
            }

            std::string toString() const {
                std::stringstream strm ;

                strm << '[' ;
                switch(type_) {
                case Type::Invalid:
                    strm << "Invalid" ;
                    break ;
                case Type::Integer:
                    strm << "Integer," << intvalue_ ;
                    break ;
                case Type::String:
                    strm << "String,'" << strvalue_ << "'" ;
                    break ;
                case Type::Real:
                    strm << "Real," << doublevalue_ ;
                    break ;                                                            
                }
                strm << ']' ;
                return strm.str() ;
            }
        private:
            Type type_ ;
            int intvalue_ ;
            double doublevalue_ ;
            std::string strvalue_ ;
        } ;
    }
}