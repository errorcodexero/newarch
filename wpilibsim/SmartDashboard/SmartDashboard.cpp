#include "SmartDashboard.h"

namespace frc {
        std::map<std::string, SmartDashboard::Value> SmartDashboard::values_ ;

        void SmartDashboard::init() {            
        }

        bool SmartDashboard::ContainsKey(const std::string &key) {
            auto it = values_.find(key) ;
            return it != values_.end() ;
        }

        std::vector<std::string> SmartDashboard::GetKeys(int types) {
            std::vector<std::string> result ;

            for(auto pair: values_) {
                if (types == 0 || (pair.second.getFlags() & types) != 0) {
                    result.push_back(pair.first) ;
                }
            }

            return result ;
        }

        void SmartDashboard::SetPersistent(const std::string &key) {
            auto it = values_.find(key) ;
            if (it != values_.end()) {
                it->second.setPersistent(true) ;
            }
        }

        void SmartDashboard::ClearPersistent(const std::string &key) {
            auto it = values_.find(key) ;
            if (it != values_.end()) {
                it->second.setPersistent(false) ;
            }
        }

        bool SmartDashboard::IsPersistent(const std::string &key) {
            auto it = values_.find(key) ;
            if (it == values_.end())
                return false ;

            return it->second.getPersistent() ;
        }

        void SmartDashboard::SetFlags(const std::string &key, unsigned int flags) {
            auto it = values_.find(key) ;
            if (it != values_.end()) {
                unsigned int current = it->second.getFlags() ;
                current |= flags ;
                it->second.setFlags(current) ;
            }
        }

        void SmartDashboard::ClearFlags(const std::string &key, unsigned int flags) {
            auto it = values_.find(key) ;
            if (it != values_.end()) {
                unsigned int current = it->second.getFlags() ;
                current &= ~flags ;
                it->second.setFlags(current) ;
            }
        }

        unsigned int SmartDashboard::GetFlags(const std::string &key) {
            auto it = values_.find(key) ;
            if (it == values_.end())
                return 0 ;

            return it->second.getFlags() ;
        }

        void SmartDashboard::Delete(const std::string &key) {
            auto it = values_.find(key) ;
            if (it != values_.end())
                values_.erase(it) ;
        }

        bool SmartDashboard::PutBoolean(const std::string &keyName, bool value) {
            bool ret = true ;

            auto it = values_.find(keyName) ;
            if (it == values_.end()) {
                Value v(value) ;
                values_[keyName] = v ;
            }
            else {
                if (it->second.isBoolean()) {
                    it->second.setBoolean(value) ;
                }
                else {
                    ret = false; 
                }
            }

            return ret ;
        }

        bool SmartDashboard::SetDefaultBoolean(const std::string &key, bool defaultValue) {
            bool ret = true ;

            auto it = values_.find(key) ;
            if (it == values_.end()) {
                Value v(defaultValue) ;
                values_[key] =  v ;           
            }
            else {
                if (!it->second.isBoolean())
                    ret = false ;
            }

            return ret ;
        }

        bool SmartDashboard::GetBoolean(const std::string &keyName, bool defaultValue) {
            auto it = values_.find(keyName) ;
            if (it == values_.end() || it->second.isBoolean() == false)
                return defaultValue ;

            return it->second.getBoolean() ;
        }

        bool SmartDashboard::PutNumber(const std::string &keyName, double value) {
            bool ret = true ;

            auto it = values_.find(keyName) ;
            if (it == values_.end()) {
                Value v(value) ;
                values_[keyName] = v ;
            }
            else {
                if (it->second.isNumber()) {
                    it->second.setNumber(value) ;
                }
                else {
                    ret = false; 
                }
            }

            return ret ;            
        }
        bool SmartDashboard::SetDefaultNumber(const std::string &key, double defaultValue) {
            bool ret = true ;

            auto it = values_.find(key) ;
            if (it == values_.end()) {
                Value v(defaultValue) ;
                values_[key] = v ;                
            }
            else {
                if (!it->second.isNumber())
                    ret = false ;
            }

            return ret ;            
        }

        double SmartDashboard::GetNumber(const std::string &keyName, double defaultValue) {
            auto it = values_.find(keyName) ;
            if (it == values_.end() || it->second.isNumber() == false)
                return defaultValue ;

            return it->second.getNumber() ;            
        }

        bool SmartDashboard::PutString(const std::string &keyName, const std::string &value) {
            bool ret = true ;

            auto it = values_.find(keyName) ;
            if (it == values_.end()) {
                Value v(value) ;
                values_[keyName] = v ;
            }
            else {
                if (it->second.isString()) {
                    it->second.setString(value) ;
                }
                else {
                    ret = false; 
                }
            }

            return ret ;             
        }
        bool SmartDashboard::SetDefaultString(const std::string &key, const std::string &defaultValue) {
            bool ret = true ;

            auto it = values_.find(key) ;
            if (it == values_.end()) {
                Value v(defaultValue) ;
                values_[key] = v ;                
            }
            else {
                if (!it->second.isString())
                    ret = false ;
            }

            return ret ;                                            
                                    }
        std::string SmartDashboard::GetString(const std::string &keyName, const std::string &defaultValue) {
            auto it = values_.find(keyName) ;
            if (it == values_.end() || it->second.isString() == false)
                return defaultValue ;

            return it->second.getString() ;                
        }

        void SmartDashboard::UpdateValues() {
            
        }
}