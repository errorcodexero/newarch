#include "CSVData.h"
#include <sstream>

namespace xero {
    namespace misc {
        double CSVData::CSVItem::getDouble() const { 
            assert(type_ == Type::Double);
            return value_.doubleValue;
        }
        std::string CSVData::CSVItem::getString() const { 
            assert(type_ == Type::String);
            return value_.stringValue; 
        }

        CSVData::CSVItem::CSVItem(const CSVItem &other): type_(other.type_) {
            if (type_ == Type::Double) value_.doubleValue = other.value_.doubleValue;
            else new (&value_.stringValue) std::string(other.value_.stringValue);
        }

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
                didLoad_ = true ;
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

            int lineno = 0 ;
            std::string line, word ;

            while (std::getline(strm, line)) {
                ++lineno ;

                // Strip Windows line endings
                while (line.length() > 0 && line[line.length() - 1] == '\r') line.pop_back();

                std::vector<CSVItem> newdata ;
                
                bool parsingString = false;
                for (size_t i = 0; i < line.length(); i++) {
                    if (!parsingString && word.size() == 0 && line[i] == '"') parsingString = true;
                    else if (parsingString) {
                        if (line[i] == '"') {
                            // a quote should be the last thing in a field
                            if (i+1 == line.length() || line[i+1] == ',') {
                                // end of string
                                newdata.push_back(CSVItem(word));
                                word.clear();
                                parsingString = false;
                                i++;    // skip the comma
                            } else {
                                assert(0 == "Strings containing double quotes are not supported");
                            }
                        } else {
                            word += line[i];
                        }
                    } else {
                        if (line[i] == ',') {
                            // end of double
                            newdata.push_back(CSVItem(std::stod(word)));
                            word.clear();
                        } else {
                            word += line[i];
                        }
                    }
                }

                if (!word.empty()) {
                    if (parsingString) newdata.push_back(CSVItem(word));
                    else newdata.push_back(CSVItem(std::stod(word)));
                }

                if (lineno == 1) {
                    columns_ = newdata.size();
                    headers_.reserve(columns_);
                    for (CSVItem item : newdata) {
                        assert(item.isString() && "CSV header may only contain strings");
                        headers_.push_back(item.getString());
                    }
                } else {
                    assert(newdata.size() == columns_ && "CSV rows must have the same number of columns");
                    data_.push_back(newdata);
                    newdata.clear();
                }
            }
        }
    }
}
