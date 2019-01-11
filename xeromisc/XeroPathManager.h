#pragma once

#include "MessageLogger.h"
#include "XeroPath.h"
#include <string>
#include <map>
#include <memory>

namespace xero {
    namespace misc {
        class XeroPathManager
        {
        public:
            XeroPathManager(MessageLogger &logger, const std::string &location) ;
            virtual ~XeroPathManager() ;

            bool loadPath(const std::string &pathname) ;
            bool hasPath(const std::string &pathname) ;
            std::shared_ptr<XeroPath> getPath(const std::string &pathname) ;

        private:
            std::string basedir_ ;
            std::map<std::string, std::shared_ptr<XeroPath>> paths_ ;
            MessageLogger &logger_ ;
        } ;
    }
}