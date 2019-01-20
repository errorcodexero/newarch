#include "CSVData.h"

namespace xero {
    namespace misc {
        CSVData::CSVData(const std::string & fileName) : fileName_(fileName) {
            file_ = std::ifstream(fileName, std::ios::binary);
            didLoad_ = !file_.fail();
            if (didLoad_)
                loadData() ;
            file_.close() ;
        }

        void CSVData::loadData() {            
            if (!didLoad_) return;
            std::array<double, HEADER_COUNT> newdata ;

            int lineno = 0 ;
            std::string line, word ;

            int dataIdx ;
            while (std::getline(file_, line)) {
                ++lineno ;
                dataIdx = 0 ;

                if (lineno > 1) {
                    for(size_t i = 0 ; i < line.length() ; i++) {
                        if (line[i] == ',') {
                            if (dataIdx != 0 && dataIdx <= HEADER_COUNT)
                                newdata[dataIdx - 1] = std::stod(word) ;
                            word.clear() ;
                            dataIdx++ ;
                        }
                        else {
                            word += line[i] ;
                        }
                    }
                    if (dataIdx != 0 && dataIdx <= HEADER_COUNT)
                        newdata[dataIdx - 1] = std::stod(word) ;

                    data_.push_back(newdata) ;
                }
            }
        }
    }
}
