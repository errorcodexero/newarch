#pragma once

#include "XeroPathSegment.h"
#include <string>
#include <vector>

namespace xero {
    namespace misc {
        class XeroPath
        {
        public:
            XeroPath(const std::string &name, std::vector<XeroPathSegment> left, std::vector<XeroPathSegment> right) ; 
            virtual ~XeroPath() ;

            const std::string &getName() const {
                return name_ ;
            }

            size_t size() const {
                return left_segments_.size() ;
            }

            const XeroPathSegment &getLeft(size_t i) {
                return left_segments_[i] ;
            }

            const XeroPathSegment &getRight(size_t i) {
                return right_segments_[i] ;
            }

        private:
            std::string name_ ;
            std::vector<XeroPathSegment> left_segments_ ;
            std::vector<XeroPathSegment> right_segments_ ;
        } ;
    }
}