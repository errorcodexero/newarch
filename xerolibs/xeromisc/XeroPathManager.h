#pragma once

#include "XeroPath.h"
#include <string>
#include <map>
#include <list>
#include <memory>

/// \file

namespace xero {
    namespace misc {

        /// \brief This class manages the set of paths loaded by the robot code
        /// Each path has a name and can be referenced by name.
        class XeroPathManager {
        public:                
            /// \brief create the path manager
            /// \param basedir the directory where all paths are loaded
            XeroPathManager(const std::string &basedir);

            /// \brief load a path from a set of files
            /// This will cause the path manager to load a path file for the left
            /// and right sides of the robot, as well as a flags file for the path
            /// if one exists.
            /// \param pathName the name of the path to load
            /// \returns true if the path was loaded, otherwise false
            bool loadPath(const std::string & pathName) ;

            /// \brief returns true if the path manager has loaded the path with the given name
            /// \param pathName the name of the path of interest
            /// \return true if the path has been loaded sucessfully, otherwise false
            bool hasPath(const std::string & pathName) ;

            /// \brief return a shared pointer to a path
            /// \param pathName the name of the path to return
            /// \returns a pointer to the path if it is persent, otherwise returns nullptr
            std::shared_ptr<XeroPath> getPath(const std::string &pathName) ;

            /// \brief returns a list of the names of the paths loaded
            /// \returns a list of the names of the paths loaded
            std::list<std::string> getNames() ;

            /// \brief replaces the data foa a given path with a new set of data.
            /// The data given is CSV formatted data, usually read from the network tables
            /// \param pathName the name of the path to replace data
            /// \param left the data for the left side of the path
            /// \param right the data for the right side of the path
            /// \returns true if the data is parsed correctly and replaces the existing data
            bool replaceData(const std::string &pathName, const std::string &left, const std::string &right) ;

            /// \brief set the extension for files that contains the data for the path
            /// \param left this extension is appended to the path name to find the data for the left side of the robot
            /// \param right this extension is appended to the path name to find the data for the right side of the robot            
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
