#include "XeroPathReader.h"
#include <fstream>
#include <cctype>

namespace xero {
    namespace misc {
        const std::string XeroPathReader::v1_left_side_suffix_(".left.pf1.csv") ;
        const std::string XeroPathReader::v1_right_side_suffix_(".right.pf1.csv") ;

        bool XeroPathReader::getValuesFromLine(const std::string &line, std::vector<double> &values) {
            size_t i = 0, pos ;
            std::string number ;
            double d ;
            bool ret = true ;

            values.clear() ;

            while (i < line.length() && ret) {
                while (i < line.length() && std::isspace(line[i]))
                    i++ ;

                while (i < line.length() && line[i] != ',')
                    number += line[i++] ;

                try {
                    d = std::stod(number, &pos) ;
                    if (pos != number.length())
                        ret = false ;
                }
                catch(...) {
                    ret = false ;
                }
                values.push_back(d) ;

                // Skip comma if necessary
                if (i != line.length())
                    i++ ;
            }

            return ret ;
        }

        bool XeroPathReader::readCSVFile(MessageLogger &logger, const std::string &pathname, const std::string &filename, std::vector<XeroPathSegment> &data) {
            std::ifstream ifile(filename) ;
            std::string line ;
            size_t lineno = 0 ;
            std::vector<double> values ;

            if (ifile.bad() || ifile.fail()) {
                logger.startMessage(MessageLogger::MessageType::error) ;
                logger << "XeroPathReader: path " << pathname << " invalid:" ;
                logger << " cannot open file '" << filename << "' for reading" ;
                logger.endMessage() ;
                return false ;                
            }

            while (std::getline(ifile, line)) {
                lineno++ ;

                if (lineno == 1)
                    continue ;

                if (!getValuesFromLine(line, values)) {
                    logger.startMessage(MessageLogger::MessageType::error) ;
                    logger << "XeroPathReader: path " << pathname << " invalid:" ;
                    logger << " line " << lineno << " does not contain a valid set of numbers" ;
                    logger.endMessage() ;
                    return false ;                         
                }

                if (values.size() != 8) {
                    logger.startMessage(MessageLogger::MessageType::error) ;
                    logger << "XeroPathReader: path " << pathname << " invalid:" ;
                    logger << " line " << lineno << " does not contain eight numberes" ;
                    logger.endMessage() ;
                    return false ;                      
                }

                XeroPathSegment seg(values[1], values[2], values[3], values[4], values[5], values[6], values[7]) ;
                data.push_back(seg) ;
            }

            return true ;            
        }

        std::shared_ptr<XeroPath> XeroPathReader::readPath(MessageLogger &logger, const std::string &pathname, const std::string &filename, XeroPathReader::PathType type) {
            if (type != PathType::PathFinderV1)
                return nullptr ;

            std::vector<XeroPathSegment> left ;
            std::vector<XeroPathSegment> right ;

            std::string  fullname ;

            fullname = filename + v1_left_side_suffix_ ;
            if (!readCSVFile(logger, pathname, fullname, left))
                return nullptr ;

            fullname = filename + v1_right_side_suffix_ ;
            if (!readCSVFile(logger, pathname, fullname, right))
                return nullptr ;
            
            if (left.size() != right.size()) {
                logger.startMessage(MessageLogger::MessageType::error) ;
                logger << "XeroPathReader: path " << pathname << " invalid:" ;
                logger << " left and right sides have a different number of segments" ;
                logger.endMessage() ;
                return nullptr ;
            }

            std::shared_ptr<XeroPath> path = std::make_shared<XeroPath>(pathname, left, right) ;
            return path ;
        }
    }
}