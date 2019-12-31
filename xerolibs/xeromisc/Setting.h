#pragma once

#include <cassert>
#include <iostream>


/// \file

namespace xero {
    namespace misc {
        /// \brief A single setting item, capable of holding a boolean, integer, double, or string value
        class Setting {
        public:
            /// \brief The type of value held by the setting
            enum class Type {
                Invalid,
                Boolean,
                Integer,
                Double,
                String
            };

            /// \brief Create a new setting with unset type
            Setting() {
                type_ = Type::Invalid;
                int_ = 0 ;
                double_ = 0.0 ;
                bool_ = false ;
            }

            /// \brief Create a new setting holding the given boolean value
            /// \param b the boolean value to store
            Setting(bool b) {
                type_ = Type::Boolean;
                bool_ = b;
                int_ = 0 ;
                double_ = 0.0 ;                
            }

            /// \brief Create a new setting holding the given integer value
            /// \param i the integer value to store
            Setting(int i) {
                type_ = Type::Integer;
                int_ = i;
                double_ = 0.0 ; 
                bool_ = false ;                
            }

            /// \brief Create a new setting holding the given double value
            /// \param d the double value to store
            Setting(double d) {
                type_ = Type::Double;
                double_ = d;
                int_ = 0 ;
                bool_ = false ;
            }

            /// \brief Create a new setting holding the given string value
            /// \param s the string value to store
            Setting(std::string s) {
                type_ = Type::String;
                string_ = s;
                int_ = 0;
                double_ = 0.0 ; 
                int_ = 0 ;     
                bool_ = false ;                              
            }

            /// \brief Create a new setting holding the given string value
            /// \param s the string value to store
            Setting(const char *s) {
                type_ = Type::String ;
                string_ = s ;
                int_ = 0;
                double_ = 0.0 ; 
                int_ = 0 ;      
                bool_ = false ;                             
            }

            /// \brief returns true if the setting is a boolean
            /// \returns true if the setting is a boolean
            bool isBoolean() const {
                return type_ == Type::Boolean ;
            }

            /// \brief returns true if the setting is a double
            /// \returns true if the setting is a double
            bool isDouble() const {
                return type_ == Type::Double ;
            }

            /// \brief returns true if the setting is an integer
            /// \returns true if the setting is an integer
            bool isInteger() const {
                return type_ == Type::Integer ;
            }

            /// \brief returns true if the setting is a string
            /// \returns true if the setting is a string
            bool isString() const {
                return type_ == Type::String ;
            }
            
            /// \brief Return the boolean value held by the setting
            /// \returns the boolean value held by the setting
            bool getBoolean() const {
                assert(type_ == Type::Boolean);

                return bool_;
            }

            /// \brief Return the integer value held by the setting
            /// \returns the integer value held by the setting
            int getInteger() const {
                assert(type_ == Type::Integer) ;

                return int_;
            }

            /// \brief Return the double value held by the setting
            /// \returns the double value held by the setting
            double getDouble() const {
                assert(type_ == Type::Double || type_ == Type::Integer);
                
                if (type_ == Type::Integer)
                    return static_cast<double>(int_);
                return double_;
            }

            /// \brief Return the string value held by the setting
            /// \returns the string value held by the setting
            const std::string &getString() const {
                assert(type_ == Type::String);

                return string_;
            }

            /// \brief The == operator for the setting object
            /// \param s the setting to compare to
            /// \returns true if the objects are the same type and value
            bool operator==(const Setting &s) const  {
                bool ret = true ;

                if (type_ != s.type_)
                    return false ;

                switch(type_) {
                case Type::Boolean:
                    ret = (bool_ == s.bool_) ;
                    break ;

                case Type::Integer:
                    ret = (int_ == s.int_) ;
                    break ;

                case Type::Double:
                    ret = (double_ == s.double_) ;
                    break ;

                case Type::String:
                    ret = (string_ == s.string_) ;
                    break ;

                case Type::Invalid:
                    ret = true ;
                }

                return ret ;
            }

        private:
            Type type_;
            bool bool_;
            int int_;
            double double_;
            std::string string_;
        };
    }
}
