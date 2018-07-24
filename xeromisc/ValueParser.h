#pragma once

#include "Value.h"
#include "messageLogger.h"
#include <string>
#include <map>
#include <iostream>

namespace xero {
    namespace misc {
        class ValueParser {
        public:
            ValueParser() ;

            bool readFile(const std::string &filename, messageLogger &logger, uint64_t msggroup) ;

            void putValue(const std::string &name, int v) {
                values_[name] = Value(v) ;
            }

            void putValue(const std::string &name, double v) {
                values_[name] = Value(v) ;
            }

            void putValue(const std::string &name, const std::string &v) {
                values_[name] = Value(v) ;
            }

            bool isDefined(const std::string &name) const {
                auto it = values_.find(name) ;
                return it != values_.end() ;
            }

            const Value &getValue(const std::string &name) const {
                auto it = values_.find(name) ;
                if (it == values_.end()) {
                    std::cerr << "ValueParser: request for missing property '" << name << "'" << std::endl ;
                    assert(it != values_.end()) ;
                }

                return it->second ;
            }

            std::string toString() ;

        private:
            bool divideTwo(const std::string &line, std::string &name, std::string &value) ;
            bool parseString(const std::string &v, std::string &result) ;
            bool parseInteger(const std::string &v, int &result) ;
            bool parseDouble(const std::string &v, double &result) ;

        private:
            std::map<std::string, Value> values_ ;
        } ;
    }
}