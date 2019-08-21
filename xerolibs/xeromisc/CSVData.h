#pragma once

#include "XeroPathConsts.h"
#include <map>
#include <string>
#include <array>
#include <vector>
#include <iostream>
#include <fstream>
#include <memory>
#include <algorithm>
#include <cassert>

namespace xero {
    namespace misc {
        class CSVData {
        public:
            CSVData(const std::string & fileName, bool data) ;
            CSVData(std::istream &strm) ;
            
            void loadData(std::istream &strm) ;

            const double getData(size_t row, size_t column) {
                assert(didLoad_) ;
                return data_[row][column];
            }

            const std::array<double, HEADER_COUNT> &getRow(int rowNum) {
                assert(didLoad_) ;
                return data_[rowNum];
            }

            auto getAllData() {
                assert(didLoad_) ;
                return data_;
            }

            auto isLoaded() {
                return didLoad_;
            }

            auto size() {
                return data_.size() ;
            }            

        private:
            bool didLoad_ ;
            size_t rowIdx_, colIdx_ ;
            std::string fileName_ ;
            std::ifstream file_ ;
            std::array<std::string, HEADER_COUNT> headers_ ;
            std::vector<std::array<double, HEADER_COUNT>> data_ ;
        } ;
    }
}