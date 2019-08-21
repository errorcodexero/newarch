#include "XeroPathManager.h"
#include "CSVData.h"

namespace xero {
    namespace misc {
        //
        // Note, this reversal of left and right is intentional.  I am pretty sure that the
        // PathWeaver is reversing these when writing out the paths (or their nomemclature
        // is backward and I don't understand it).
        //
        constexpr const char* rightSuffix  = ".right.csv";
        constexpr const char* leftSuffix = ".left.csv";

        XeroPathManager::XeroPathManager(const std::string &basedir) : basedir_(basedir) {
        }

        std::list<std::string> XeroPathManager::getNames()
        {
            std::list<std::string> names ;
            for(const auto &pair : paths_)
                names.push_back(pair.first) ;

            return names ;
        }

        bool XeroPathManager::replaceData(const std::string &pathName, const std::string &left, const std::string &right)
        {
            CSVData leftData(left, true) ;
            CSVData rightData(right, true) ;

            if (!leftData.isLoaded()) {
                std::cerr <<"XeroPathManager: failed loading path '" << pathName << "' from network table, data is invalid" << std::endl ;                
                return false;
            }

            if (!rightData.isLoaded()) {
                std::cerr <<"XeroPathManager: failed loading path '" << pathName << "' from network table, data is invalid" << std::endl ;
                return false;
            }

            if(rightData.size() != leftData.size()) {
                std::cerr <<"XeroPathManager: failed loading path '" << pathName << "' from network table, data size mismatch" << std::endl ;                
                return false;
            }

            if (leftData.size() == 0)
            {
                std::cerr <<"XeroPathManager: failed loading path '" << pathName << "' from network table, file is empty" << std::endl ;                
                return false ;
            }

            paths_[pathName] = std::make_shared<XeroPath>(pathName, leftData, rightData);
            return true ;
        }

        bool XeroPathManager::loadPath(const std::string & pathName) {
            std::string filename = basedir_ + "/" + pathName + leftSuffix ;
            CSVData leftData(filename, false);
            if (!leftData.isLoaded()) {
                std::cerr <<"XeroPathManager: failed loading file '" << filename << "'" << std::endl ;                
                return false;
            }

            filename = basedir_ + "/" + pathName + rightSuffix ;
            CSVData rightData(filename, false);
            if (!rightData.isLoaded()) {
                std::cerr <<"XeroPathManager: failed loading file '" << filename << "'" << std::endl ;
                return false;
            }

            if(rightData.size() != leftData.size()) {
                std::cerr <<"XeroPathManager: failed loading file '" << filename << "' data size mismatch" << std::endl ;                
                return false;
            }

            if (leftData.size() == 0)
            {
                std::cerr <<"XeroPathManager: failed loading file '" << filename << "' file is empty" << std::endl ;                
                return false ;
            }

            paths_[pathName] = std::make_shared<XeroPath>(pathName, leftData, rightData);
            return true;
        }

        bool XeroPathManager::hasPath(const std::string & pathName) {
            auto iter = paths_.find(pathName);
            return iter != std::end(paths_);
        }

        std::shared_ptr<XeroPath> XeroPathManager::getPath(const std::string &pathName) {
            if(hasPath(pathName)) {
                return paths_[pathName];
            } else {
                return nullptr;
            }
        }
    }
}
