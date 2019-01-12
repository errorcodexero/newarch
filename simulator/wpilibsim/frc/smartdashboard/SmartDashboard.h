#pragma once

#include <vector>
#include <map>
#include <string>
#include <memory>
#include <cassert>

enum NT_Type {
  NT_UNASSIGNED = 0,
  NT_BOOLEAN = 0x01,
  NT_DOUBLE = 0x02,
  NT_STRING = 0x04,
  NT_RAW = 0x08,
  NT_BOOLEAN_ARRAY = 0x10,
  NT_DOUBLE_ARRAY = 0x20,
  NT_STRING_ARRAY = 0x40,
  NT_RPC = 0x80
};

namespace frc
{
    class SmartDashboard
    {
    public:
        SmartDashboard() = delete;
        ~SmartDashboard() = delete;

        static void init();

        static bool ContainsKey(const std::string &key);

        static std::vector<std::string> GetKeys(int types = 0);

        static void SetPersistent(const std::string &key);
        static void ClearPersistent(const std::string &key);
        static bool IsPersistent(const std::string &key);

        static void SetFlags(const std::string &key, unsigned int flags);
        static void ClearFlags(const std::string &key, unsigned int flags);
        static unsigned int GetFlags(const std::string &key);

        static void Delete(const std::string &key);

        static bool PutBoolean(const std::string &keyName, bool value);
        static bool SetDefaultBoolean(const std::string &key, bool defaultValue);
        static bool GetBoolean(const std::string &keyName, bool defaultValue);

        static bool PutNumber(const std::string &keyName, double value);
        static bool SetDefaultNumber(const std::string &key, double defaultValue);
        static double GetNumber(const std::string &keyName, double defaultValue);

        static bool PutString(const std::string &keyName, const std::string &value);
        static bool SetDefaultString(const std::string &key,
                                    const std::string &defaultValue);
        static std::string GetString(const std::string &keyName,
                                    const std::string &defaultValue);

        static void UpdateValues();

    private:
        class Value {
        public:
            Value() {
                flags_ = 0 ;
                persistent_ = false ;
            }

            Value(double v) {
                number_ = v ;
                flags_ = NT_DOUBLE ;
                persistent_ = false ;
            }

            Value(const std::string &v) {
                string_ = v ;
                flags_ = NT_STRING ;
                persistent_ = false ;
            }            
            
            Value(bool v) {
                boolean_ = v ;
                flags_ = NT_BOOLEAN ;
                persistent_ = false ;
            }

            void setPersistent(bool b) {
                persistent_ = b ;
            }

            bool getPersistent() const {
                return persistent_ ;
            }

            void setFlags(unsigned int flags) {
                flags_ = flags ;
            }

            unsigned int getFlags() const {
                return flags_ ;
            }

            bool isNumber() const {
                return (flags_ & NT_DOUBLE) == NT_DOUBLE ;
            }

            double getNumber() const {
                assert(isNumber()) ;
                return number_ ;
            }

            void setNumber(double v) {
                clearType() ;
                flags_ |= NT_DOUBLE ;
                number_ = v ;
            }

            bool isString() const {
                return (flags_ & NT_STRING) == NT_STRING ;
            }

            const std::string &getString() const {
                assert(isString()) ;
                return string_ ;
            }

            void setString(const std::string &str) {
                clearType() ;
                flags_ |= NT_STRING ;
                string_ = str ;
            }

            double isBoolean() const {
                return (flags_ & NT_BOOLEAN) == NT_BOOLEAN ;
            }

            bool getBoolean() const {
                assert(isBoolean()) ;
                return boolean_ ;
            }

            void setBoolean(bool v) {
                clearType() ;
                flags_ |= NT_BOOLEAN ;
                boolean_ = v ;
            }

        private:
            void clearType() {
                flags_ &= (NT_DOUBLE | NT_STRING | NT_BOOLEAN | NT_DOUBLE_ARRAY | NT_STRING_ARRAY | NT_BOOLEAN_ARRAY) ;
            }

        private:
            bool persistent_ ;
            unsigned int flags_ ;
            double number_ ;
            std::string string_ ;
            bool boolean_ ;
        } ;

    private:
        static std::map<std::string, SmartDashboard::Value> values_ ;
    } ;

} // namespace frc
