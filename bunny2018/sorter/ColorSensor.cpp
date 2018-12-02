#include "ColorSensor.h"
#include <Timer.h>
#include <iostream>

namespace xero {
    namespace bunny2018 {
        ColorSensor::ColorSensor(int devaddr, tcs34725IntegrationTime_t time, tcs34725Gain_t gain) {
            std::cout << "Creating ColorSensor at address " << std::hex << "0x" << (int)devaddr << std::endl ;
            i2c_ = std::make_shared<frc::I2C>(frc::I2C::kOnboard, devaddr) ;

            time_ = time ;
            gain_ = gain ;
        }

        int ColorSensor::read8(uint8_t regaddr) {
            uint8_t v ;
            
            if (i2c_->Read(regaddr | CommandBit, 1, &v)) {
                std::cout << "I2C read8 failed, address " << std::hex << "0x" << (int)regaddr << std::endl ;
                return -1 ;
            }

            return v ;
        }

       bool ColorSensor::write8(uint8_t regaddr, uint8_t value) {
            if (i2c_->Write(regaddr | CommandBit, value)) {
                std::cout << "I2C write failed, address " << std::hex << "0x" << (int)regaddr << std::endl ;
                return false ;
            }

            return true ;
        }

        int ColorSensor::read16(uint8_t regaddr)
        {
            uint8_t data[2] ;

            if (!i2c_->Read(regaddr | CommandBit, 2, data)) {
                std::cout << "I2C read16 failed, address " << std::hex << "0x" << (int)regaddr << std::endl ;
                return -1 ;
            }

            return data[0] << 8 | data[1] ;
        }

        bool ColorSensor::setIntegrationTime(tcs34725IntegrationTime_t t) {
            return write8(TCS34725_ATIME, t) ;
        }

        bool ColorSensor::setGain(tcs34725Gain_t g) {
            return write8(TCS34725_ATIME, g) ;
        }        

        bool ColorSensor::isAlive() {
            int id = read8(IDAddress) ;
            if (id != IDRegValue)
                return false ;

            return true ;
        }

        bool ColorSensor::initialize() {
            if (!setIntegrationTime(time_))
                return false ;

            if (!setGain(gain_))
                return false ;

            return true ;
        }

        bool ColorSensor::enable(void)
        {
            if (!write8(TCS34725_ENABLE, TCS34725_ENABLE_PON))
                return false ;

            frc::Wait(0.100) ;

            if (!write8(TCS34725_ENABLE, TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN))
                return false ;

            frc::Wait(0.100) ;
            
            return true ;
        }

        bool ColorSensor::disable()
        {
            /* Turn the device off to save power */
            int reg = 0;
            reg = read8(TCS34725_ENABLE);
            if (reg == -1)
                return false ;

            if (!write8(TCS34725_ENABLE, reg & ~(TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN)))
                return false ;

            return true ;
        }

        void ColorSensor::getRawData (uint16_t *r, uint16_t *g, uint16_t *b, uint16_t *c)
        {
            *c = read16(TCS34725_CDATAL);
            *r = read16(TCS34725_RDATAL);
            *g = read16(TCS34725_GDATAL);
            *b = read16(TCS34725_BDATAL);
        }
    }
}
