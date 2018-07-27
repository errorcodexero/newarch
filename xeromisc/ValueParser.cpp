#include "ValueParser.h"
#include "messageLogger.h"
#include <fstream>
#include <cctype>
#include <string>

namespace xero {
    namespace misc {
        ValueParser::ValueParser() {            
        }

        bool ValueParser::divideTwo(const std::string &line, std::string &name, std::string &value) {
            size_t pos = 0 ;

            name.clear() ;
            value.clear() ;

            // Skip leading spaces
            while (pos < line.length() && std::isspace(line[pos]))
                pos++ ;

            // Empty or comments only line
            if (pos == line.length() || line[pos] == '#')
                return true ;

            // Now, grab the first token (name)
            while (pos < line.length() && !std::isspace(line[pos]))
            {
                if (line[pos] == '#')
                    return false ;

                name += line[pos++] ;
            }

            // Skip leading spaces
            while (pos < line.length() && std::isspace(line[pos]))
                pos++ ;

            if (pos == line.length() || line[pos] == '#')
                return false ;

            // Now, grab the second token (value)
            if (line[pos] == '"') {
                value += line[pos++] ;
                while (pos < line.length() && line[pos] != '"')
                    value = line[pos++] ;

                // Check for unterminated string
                if (pos == line.length())
                    return false ;

                value += line[pos++] ;
            }
            else {                
                //
                // Grab characters til we hit the end of the line, hit white space, or hit a '#' character
                //
                while (pos < line.length() && !std::isspace(line[pos]) && line[pos] != '#')
                    value += line[pos++] ;
            }

            // Skip spaces
            while (pos < line.length() && std::isspace(line[pos]))
                pos++ ;

            // More than three tokes on the line (excluding comments)
            if (pos != line.length() && line[pos] != '#')
                return false ;

            return true ;
        }

        bool ValueParser::parseInteger(const std::string &value, int &result) {
            size_t endp ;
            bool ret = true ;

            try {
                result = std::stoi(value, &endp) ;
                if (endp != value.length()) {
                    // There are extra characters at the end
                    ret = false ;
                }
            }
            catch(...) {
                ret = false ;
            }

            return ret ;
        }

        bool ValueParser::parseDouble(const std::string &value, double &result) {
            size_t endp ;
            bool ret = true ;

            try {
                result = std::stod(value, &endp) ;
                if (endp != value.length()) {
                    // There are extra characters at the end
                    ret = false ;
                }
            }
            catch(...) {
                ret = false ;
            }

            return ret ;
        }

        bool ValueParser::parseString(const std::string &value, std::string &result) {
            if (value.length() < 2 || (value[0] != '"' && value[value.length() - 1] != '"'))
                return false ;

            result = value.substr(1, value.length() - 2) ;
            return true ;
        }

        bool ValueParser::readFile(const std::string &file, messageLogger &logger, uint64_t msggroup) {
            size_t lineno = 1 ;
            std::string name, value ;

            std::ifstream in(file) ;
            if (in.bad() || in.fail())
                return false ;

            std::string line ;
            std::string svalue ;
            int ivalue ;
            double dvalue ;

            while (std::getline(in, line)) {
                const char *line_p = line.c_str() ;
                if (divideTwo(line, name, value)) {
                    const char *name_p = name.c_str() ;
                    const char *value_p = value.c_str() ;

                    if (name.length() > 0 && value.length() > 0) {
                        if (parseString(value, svalue)) {
                            putValue(value, svalue) ;
                        } else if (parseInteger(value, ivalue)) {
                            putValue(name, ivalue) ;
                        } else if (parseDouble(value, dvalue)) {
                            putValue(name, dvalue) ;
                        } else {
                            logger.startMessage(messageLogger::messageType::warning, msggroup) ;
                            logger << file << ": line " << lineno << ": invalid value '" << value << "'" ;
                            logger.endMessage() ;                                                                
                        }
                    }
                }
                else {
                    logger.startMessage(messageLogger::messageType::warning, msggroup) ;
                    logger << file << ": line " << lineno << ": invalid line '" ;
                    logger << line << "'" ;
                    logger.endMessage() ;
                }

                lineno++ ;
            }
        }

        std::string ValueParser::toString() {
            std::string result ;

            for(auto entry : values_) {
                result += entry.first ;
                result += " = " ;
                result += entry.second.toString() ;
                result += "\n" ;
            }

            return result ;
        }
    }
}