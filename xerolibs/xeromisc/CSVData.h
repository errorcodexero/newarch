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

/// @file

namespace xero {
    namespace misc {
        /// @brief represents a single peice of data from a CSV file.
        /// Can represent a string or a number
        class CSVData {
        public:
            struct CSVItem {
            public:
                /// @brief the type of the data
                enum class Type {
                    Double,             /// @brief a floating point number
                    String              /// @brief a string
                };

                /// @brief create a new string data item
                CSVItem(std::string value): type_(Type::String), stringValue_(value) {}

                /// @brief create a new double data item
                CSVItem(double value): type_(Type::Double), doubleValue_(value) {}

                /// @brief returns the type of the data item
                /// @returns the type of the data item
                Type getType() { return type_; }

                /// @brief returns true if the data item is a double
                /// @returns true if the data item is a double
                bool isDouble() const { return type_ == Type::Double; }

                /// @brief returns true if the data item is a string
                /// @returns true if the data item is a string                
                bool isString() const { return type_ == Type::String; }

                /// @brief return the double value for the data item
                /// This method asserts if the type is not double
                /// @returns the double value for the data item
                double getDouble() const;

                /// @brief return the string value for the data item
                /// This method asserts if the type is not string
                /// @returns the string value for the data item
                std::string getString() const;
                
            private:
                Type type_;
                double doubleValue_;
                std::string stringValue_;
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