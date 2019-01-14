#pragma once

#include "XeroPath.h"
#include "MessageLogger.h"
#include <string>
#include <memory>
#include <vector>

namespace xero {
    namespace misc {
        class XeroPathReader
        {
        public:
            enum class PathType {
                PathFinderV1,
                PathFinderV2,
            } ;

        public:
            XeroPathReader() = delete ;
            ~XeroPathReader() = delete ;

            static std::shared_ptr<XeroPath> loadPath(MessageLogger &logger, const std::string &pathname, const std::string &filename, PathType type = PathType::PathFinderV1) ;

        private:
            static const std::string v1_left_side_suffix_ ;
            static const std::string v1_right_side_suffix_ ;        

        private:
            static bool getValuesFromLine(const std::string &line, std::vector<double> &data) ;
            static bool readCSVFile(MessageLogger &logger, const std::string &pathname, const std::string &filename, std::vector<XeroPathSegment> &data) ;
        } ;
    }
}