#pragma once

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
            struct CSVItem {
            public:
                enum class Type {
                    Double,
                    String
                };

                CSVItem(std::string value): type_(Type::String) { new (&value_.stringValue) std::string(value); }
                CSVItem(double value): type_(Type::Double) { value_.doubleValue = value; }
                CSVItem(const CSVItem &other);
                ~CSVItem() { if (isString()) value_.stringValue.~basic_string(); }

                Type getType() { return type_; }
                bool isDouble() const { return type_ == Type::Double; }
                bool isString() const { return type_ == Type::String; }

                double getDouble() const;
                std::string getString() const;
            private:
                union Value {
                    double doubleValue;
                    std::string stringValue;

                    Value() {}
                    ~Value() {}
                } value_;
                Type type_;
            };

            CSVData(const std::string & fileOrData, bool data) ;
            CSVData(std::istream &strm) ;
            
            void loadData(std::istream &strm) ;

            CSVItem getData(size_t row, size_t column) const {
                assert(didLoad_) ;
                return data_[row][column];
            }

            double getDouble(size_t row, size_t column) const {
                return getData(row, column).getDouble();
            }

            std::string getString(size_t row, size_t column) const {
                return getData(row, column).getString();
            }

            const std::vector<CSVItem> &getRow(int rowNum) {
                assert(didLoad_) ;
                return data_[rowNum];
            }

            /// Returns the number of columns in the CSV file.
            int getColumns() { return columns_; }

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
            size_t columns_;
            size_t rowIdx_, colIdx_ ;
            std::string fileName_ ;
            std::ifstream file_ ;
            std::vector<std::string> headers_ ;
            std::vector<std::vector<CSVItem>> data_ ;
        } ;
    }
}