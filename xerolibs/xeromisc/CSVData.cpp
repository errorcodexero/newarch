#include "CSVData.h"
#include <sstream>

namespace xero {
    namespace misc {
        CSVData::CSVData(const std::string & fileOrData, bool data)
        {
            if (!data)
            {
                std::ifstream file(fileOrData, std::ios::binary);
                didLoad_ = !file.fail();
                if (didLoad_)
                    loadData(file) ;
                file.close() ;
            }
            else
            {
                std::stringstream strm(fileOrData) ;
                loadData(strm) ;                
            }
        }

        CSVData::CSVData(std::istream &strm)
        {
            loadData(strm) ;
        }
        
        void CSVData::loadData(std::istream &strm) 
        {
            if (!didLoad_) return;
            std::array<double, HEADER_COUNT> newdata ;

            int lineno = 0 ;
            std::string line, word ;

            size_t dataIdx ;
            while (std::getline(strm, line)) {
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
