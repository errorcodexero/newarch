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
                /// @param value the string value for this item
                CSVItem(std::string value): type_(Type::String), stringValue_(value) {}

                /// @brief create a new double data item
                /// @param value the double value of this item
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

            /// @brief load data from a file or directly from a string
            /// @param fileOrData the filename or the direct data to load
            /// @param data if true, load direct data
            CSVData(const std::string & fileOrData, bool data) ;

            /// @brief load data from a file stream
            /// @param strm the stream to load data from
            CSVData(std::istream &strm) ;
            
            /// @brief load data from the stream given
            /// @param strm the stream to load data from
            void loadData(std::istream &strm) ;

            /// @brief return the data for a given row and column
            /// Asserts if the row or column are not present
            /// @param row the row of interest
            /// @param column the column of interest
            /// @returns the data at the row and column given
            CSVItem getData(size_t row, size_t column) const {
                assert(didLoad_) ;
                return data_[row][column];
            }

            /// @brief return the double data for a given row and column
            /// Asserts if the data is not a double or if the row or column are not
            /// present
            /// @param row the row of interest
            /// @param column the column of interest
            /// @returns a double from the row and column given
            double getDouble(size_t row, size_t column) const {
                return getData(row, column).getDouble();
            }

            /// @brief return the string data for a given row and column
            /// Asserts if the data is not a string or if the row or column are not
            /// present
            /// @param row the row of interest
            /// @param column the column of interest
            /// @returns a string from the row and column given
            std::string getString(size_t row, size_t column) const {
                return getData(row, column).getString();
            }

            /// @brief returns a specific row of data
            /// @param rowNum the row of interest
            /// @returns a row of data items
            const std::vector<CSVItem> &getRow(int rowNum) {
                assert(didLoad_) ;
                return data_[rowNum];
            }

            /// @brief returns the number of columns in the CSV file.
            /// @returns the number of columns in the data set
            int getColumns() { return columns_; }

            /// @brief returns the data array for the data stored
            /// Asserts if the data was not loaded sucessfully
            /// @returns the data array for the data.
            auto getAllData() {
                assert(didLoad_) ;
                return data_;
            }

            /// @brief returns true if the data loaded sucessfully
            /// @returns true if the data was loaded sucessfully, otherwise false
            auto isLoaded() {
                return didLoad_;
            }

            /// @brief returns the number of rows of data
            /// @returns the number of rows of data
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