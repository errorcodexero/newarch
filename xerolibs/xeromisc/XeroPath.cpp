#include "XeroPath.h"
#include <cassert>

namespace xero {
    namespace misc {
        XeroPath::XeroPath(const std::string &name, std::vector<XeroPathSegment> left, std::vector<XeroPathSegment> right) : left_segments_(std::move(left)), right_segments_(std::move(right)) {
            name_ = name ;

            assert(left_segments_.size() > 0) ;
            assert(left_segments_.size() == right_segments_.size()) ;
        }
        
        XeroPath::~XeroPath() {            
        }
    }
}