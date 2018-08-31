#pragma once

#include <cassert>
#include <iostream>

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
			}

			/// \brief Create a new setting holding the given boolean value
			/// \param b the boolean value to store
			Setting(bool b) {
				type_ = Type::Boolean;
				bool_ = b;
			}

			/// \brief Create a new setting holding the given integer value
			/// \param i the integer value to store
			Setting(int i) {
				type_ = Type::Integer;
				int_ = i;
			}

			/// \brief Create a new setting holding the given double value
			/// \param d the double value to store
			Setting(double d) {
				type_ = Type::Double;
				double_ = d;
			}

			/// \brief Create a new setting holding the given string value
			/// \param s the string value to store
			Setting(std::string s) {
				type_ = Type::String;
				string_ = s;
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

		private:
			Type type_;
			bool bool_;
			int int_;
			double double_;
			std::string string_;
		};
	}
}
