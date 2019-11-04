#pragma once

#include "CSVData.h"
#include "XeroSegment.h"
#include <cassert>

/// \file

namespace xero {
    namespace misc {
        /// \brief This class represents a single path that the path follower can follow.
        /// It caontains data for the left and right wheels of the robot as well as flag data
        /// if flag data is present.
        class XeroPath {
        public:
            /// \brief create a new path
            /// \param name the name of the path
            /// \param left the data for the left side of the robot
            /// \param right the data for the right side of the robot
            XeroPath(const std::string &name, CSVData & left, CSVData & right) : name_(name) {
                for (auto elem : left.getAllData()) {
                    leftPath_.push_back(XeroSegment{elem});
                }
                for (auto elem : right.getAllData()) {
                    rightPath_.push_back(XeroSegment{elem});
                }
            }

            /// \brief returns the name of the path
            /// \returns the name of the path
            const std::string &getName() const {
                return name_ ;
            }

            /// \brief returns the number of sets points in the path
            /// \returns the number of data points in the path
            size_t size() const {
                return leftPath_.size() ;
            }

            /// \brief returns a specific segment for the left side of the robot give its index
            /// \param idx the index of the segment to return            
            /// \returns a specific segment for the left side of the robot give its index
            const XeroSegment &getLeftSegment(size_t idx) {
                return leftPath_[idx];
            }

            /// \brief returns a specific segment for the right side of the robot give its index
            /// \param idx the index of the segment to return
            /// \returns a specific segment for the right side of the robot give its index
            const XeroSegment &getRightSegment(size_t idx) {
                return rightPath_[idx];
            }

            /// \brief returns the left side start position for the path
            /// \returns the left side start position for the path
            double getLeftStartPos() const {
                assert(size() > 0) ;
                return leftPath_[0].getPOS() ;
            }

            /// \brief returns the right side start position for the path
            /// \returns the right side start position for the path
            double getRightStartPos() const {
                assert(size() > 0) ;
                return rightPath_[0].getPOS() ;
            }        

        private:
            std::string name_ ;
            std::vector<XeroSegment> leftPath_;
            std::vector<XeroSegment> rightPath_;
        };     
    }
}
   
