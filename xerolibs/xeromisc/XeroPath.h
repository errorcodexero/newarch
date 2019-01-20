#pragma once

#include "CSVData.h"
#include "XeroSegment.h"
#include <cassert>

namespace xero {
    namespace misc {
        class XeroPath {
        public:
            XeroPath(const std::string &name, CSVData & left, CSVData & right) : name_(name) {
                for (auto elem : left.getAllData()) {
                    leftPath_.push_back(XeroSegment{elem});
                }
                for (auto elem : right.getAllData()) {
                    rightPath_.push_back(XeroSegment{elem});
                }
            }

            const std::string &getName() const {
                return name_ ;
            }

            size_t size() const {
                return leftPath_.size() ;
            }

            const XeroSegment &getLeftSegment(size_t idx) {
                return leftPath_[idx];
            }

            const XeroSegment &getRightSegment(size_t idx) {
                return rightPath_[idx];
            }

            double getLeftStartPos() const {
                assert(size() > 0) ;
                return leftPath_[0].getPOS() ;
            }

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
   
