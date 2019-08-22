#pragma once

#include "XeroPath.h"
#include <string>
#include <map>
#include <memory>

namespace xero {
    namespace misc {

        class XeroPathManager {
        public:                
            XeroPathManager(const std::string &basedir);
            bool loadPath(const std::string & pathName) ;
            bool hasPath(const std::string & pathName) ;
            std::shared_ptr<XeroPath> getPath(const std::string &pathName) ;

            void setExtensions(const std::string &left, const std::string &right) {
                leftext_ = left ;
                rightext_ = right ;
            }

        private:
            std::string basedir_;
            std::map<std::string, std::shared_ptr<XeroPath>> paths_;
            std::string leftext_ ;
            std::string rightext_ ;
        } ;  
    }
}
