#pragma once

#include "XeroPath.h"
#include <string>
#include <map>
#include <list>
#include <memory>

namespace xero {
    namespace misc {

        class XeroPathManager {
        public:                
            XeroPathManager(const std::string &basedir);
            bool loadPath(const std::string & pathName) ;
            bool hasPath(const std::string & pathName) ;
            std::shared_ptr<XeroPath> getPath(const std::string &pathName) ;
            std::list<std::string> getNames() ;

            bool replaceData(const std::string &pathName, const std::string &left, const std::string &right) ;

        private:
            std::string basedir_;
            std::map<std::string, std::shared_ptr<XeroPath>> paths_;      
        } ;  
    }
}
