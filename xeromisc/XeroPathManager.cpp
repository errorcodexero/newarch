#include "XeroPathManager.h"
#include "XeroPathReader.h"
#include <cassert>

namespace xero {
    namespace misc {

        XeroPathManager::XeroPathManager(MessageLogger &logger, const std::string &loc) : logger_(logger) {
            basedir_ = loc ;
        }

        XeroPathManager::~XeroPathManager() {            
        }

        bool XeroPathManager::loadPath(const std::string &pathname) {
            std::string filename ;
            
            filename = basedir_ + "/" + pathname ;
            std::shared_ptr<XeroPath> path = XeroPathReader::loadPath(logger_, pathname, filename) ;
            if (path == nullptr)
                return false ;

            paths_[pathname] = path ;

            return true ;
        }

        bool XeroPathManager::hasPath(const std::string &pathname) {
            auto it = paths_.find(pathname) ;
            return it != paths_.end() ;
        }

        std::shared_ptr<XeroPath> XeroPathManager::getPath(const std::string &pathname) {            
            auto it = paths_.find(pathname) ;
            if (it == paths_.end())
                return nullptr ;

            return it->second ;
        }
    }
}