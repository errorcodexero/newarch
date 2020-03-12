#pragma once

#include <SimulationEvent.h>
#include <string>

namespace xero
{
    namespace sim2
    {        
        
        class SimulationAssert : public SimulationEvent
        {
        public:
            enum class PropType
            {
                Double,
                Integer,
                String,
                Boolean
            } ;

        public:
            SimulationAssert(double t, const std::string &subsystem, const std::string &property, double value, double delta) : SimulationEvent(t)
            {
                subsystem_name_ = subsystem ;
                property_name_ = property ;
                dvalue_ = value ;
                delta_ = delta ;
                type_ = PropType::Double ;
            }

            SimulationAssert(double t, const std::string &subsystem, const std::string &property, int value) : SimulationEvent(t)
            {
                subsystem_name_ = subsystem ;
                property_name_ = property ;
                ivalue_ = value ;
                delta_ = 0.0 ;
                type_ = PropType::Integer ;
            }

            SimulationAssert(double t, const std::string &subsystem, const std::string &property, bool value) : SimulationEvent(t)
            {
                subsystem_name_ = subsystem ;
                property_name_ = property ;
                bvalue_ = value ;
                delta_ = 0.0 ;
                type_ = PropType::Boolean ;
            } 

            SimulationAssert(double t, const std::string &subsystem, const std::string &property, const std::string &value) : SimulationEvent(t)
            {
                subsystem_name_ = subsystem ;
                property_name_ = property ;
                svalue_ = value ;
                delta_ = 0.0 ;
                type_ = PropType::String ;
            }                         

            virtual ~SimulationAssert() {
            }

            PropType getType() const {
                return type_ ;
            }

            const std::string &getSubsystemName() const {
                return subsystem_name_ ;
            }

            const std::string &getPropertyName() const {
                return property_name_ ;
            }

            double getDoubleValue() const {
                return dvalue_ ;
            }

            int getIntegerValue() const {
                return ivalue_ ;
            }  

            bool getBoolValue() const {
                return bvalue_ ;
            } 

            const std::string &getStringValue() const {
                return svalue_ ;
            }                                        

            double getDelta() const {
                return delta_ ;
            }
            
            std::string toString() const override
            {
                std::string ret ;

                ret += "assert" ;
                ret += " subsystem='" + subsystem_name_ + "'" ;
                ret += " property='" + property_name_ + "'" ;
                return ret ;
            }
        private:
            PropType type_ ;
            std::string subsystem_name_ ;
            std::string property_name_ ;
            double dvalue_ ;
            double delta_ ;
            int ivalue_ ;
            bool bvalue_ ;
            std::string svalue_ ;
        } ;
    }
}
