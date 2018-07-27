#pragma once

#include <cassert>

namespace xero {
	namespace misc {
		class Setting {
		public:
			enum class Type {
				Invalid,
				Boolean,
				Integer,
				Double,
				String
			};

			Setting() {
				type_ = Type::Invalid;
			}

			Setting(bool b) {
				type_ = Type::Boolean;
				bool_ = b;
			}

			Setting(int i) {
				type_ = Type::Integer;
				int_ = i;
			}

			Setting(double d) {
				type_ = Type::Double;
				double_ = d;
			}

			Setting(std::string s) {
				type_ = Type::String;
				string_ = s;
			}

			bool getBoolean() {
				assert(type_ == Type::Boolean);

				return bool_;
			}

			int getInteger() {
				assert(type_ == Type::Integer);

				return int_;
			}

			double getDouble() {
				assert(type_ == Type::Double || type_ == Type::Integer);
				
				if (type_ == Type::Integer)
					return static_cast<double>(int_);
				return double_;
			}

			std::string getString() {
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
