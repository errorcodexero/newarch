#include <NavXSim.h>
#include <SimulatorEngine.h>
#include <SimulatedMotor.h>
#include <mockdata/SPIData.h>
#include <IMURegisters.h>
#include <cassert>

using namespace xero::sim2;

namespace xero
{
    namespace models
    {
        NavXSim *NavXSim::theOneNavX = nullptr ;

        NavXSim *NavXSim::getNavXSim()
        {
            return theOneNavX ;
        }

        void NavXSim::SPIInitialize(const char *name, void *param, const struct HAL_Value *value) 
        {
            NavXSim *obj = reinterpret_cast<NavXSim *>(param) ;
            obj->SPIInitialize(name, value) ;
        }

        void NavXSim::SPIRead(const char *name, void *param, unsigned char *buffer, unsigned int count)
        {
            NavXSim *obj = reinterpret_cast<NavXSim *>(param) ;
            obj->SPIRead(name, buffer, count) ;
        }

        void NavXSim::SPIWrite(const char *name, void *param, const unsigned char *buffer, unsigned int count)
        {
            NavXSim *obj = reinterpret_cast<NavXSim *>(param) ;
            obj->SPIWrite(name, buffer, count) ;            
        }                

        NavXSim::NavXSim(SimulatorEngine &engine, const std::string &inst) : SimulationModel(engine, "navxsim", inst)
        {
            active_ = false ;

            for(size_t i = 0 ; i < registers_.size() ; i++)
                registers_[i] = 0 ;

            registers_[NAVX_REG_WHOAMI] = 0x32 ;                            // Who AM I
            registers_[NAVX_REG_HW_REV] = 0x42 ;                            // Board revision
            registers_[NAVX_REG_FW_VER_MAJOR] = 0x02 ;                      // Firmware major revision
            registers_[NAVX_REG_FW_VER_MINOR] = 0x03 ;                      // Firmware minor revision
            registers_[NAVX_REG_OP_STATUS] = NAVX_OP_STATUS_NORMAL ;        // Operational status
            registers_[NAVX_REG_CAL_STATUS] =                               // Calibration status
                    NAVX_CAL_STATUS_IMU_CAL_COMPLETE | 
                    NAVX_CAL_STATUS_MAG_CAL_COMPLETE | 
                    NAVX_CAL_STATUS_BARO_CAL_COMPLETE ;
            registers_[NAVX_REG_SENSOR_STATUS_L] =                          // Sensor status
                    NAVX_SENSOR_STATUS_MOVING | 
                    NAVX_SENSOR_STATUS_YAW_STABLE | 
                    NAVX_SENSOR_STATUS_FUSED_HEADING_VALID ;
            registers_[NAVX_REG_SELFTEST_STATUS] =                          // Self test status
                    NAVX_SELFTEST_STATUS_COMPLETE | 
                    NAVX_SELFTEST_RESULT_BARO_PASSED | 
                    NAVX_SELFTEST_RESULT_MAG_PASSED |
                    NAVX_SELFTEST_RESULT_GYRO_PASSED | 
                    NAVX_SELFTEST_RESULT_MAG_PASSED ;

            assert(theOneNavX == nullptr) ;
            theOneNavX = this ;
            read_transaction_ = false ;
            timestamp_ = 0 ;
        }

        NavXSim::~NavXSim()
        {
            assert(theOneNavX == this) ;
            theOneNavX = nullptr ;
        }

        bool NavXSim::create()
        {
            index_ = getInteger(IndexName) ;

            if (!hasProperty("hw:interface"))
            {
                SimulatorMessages &msg = getEngine().getMessageOutput() ;
                msg.startMessage(SimulatorMessages::MessageType::Error) ;
                msg << "model " << getModelName() << " instance " << getInstanceName() ;
                msg << " - the hardware interface 'interface' has not been specified, must be 'SPI', 'I2C', or 'UART'" ;
                msg.endMessage(getEngine().getSimulationTime()) ;
                return false ;
            }

            const SimValue &prop = getProperty("hw:interface") ;
            if (!prop.isString())
            {
                SimulatorMessages &msg = getEngine().getMessageOutput() ;
                msg.startMessage(SimulatorMessages::MessageType::Error) ;
                msg << "model " << getModelName() << " instance " << getInstanceName() ;
                msg << " - the hardware interface 'interface' has been specified, but is not a string, must be 'SPI', 'I2C', or 'UART'" ;
                msg.endMessage(getEngine().getSimulationTime()) ;  
                return false ;              
            }

            if (prop.getString() == "SPI")
            {
                SimulatorMessages &msg = getEngine().getMessageOutput() ;
                msg.startMessage(SimulatorMessages::MessageType::Debug, 5) ;
                msg << "model " << getModelName() << " instance " << getInstanceName() ;
                msg << " - the hardware interface 'SPI' has been initialized" ;
                msg.endMessage(getEngine().getSimulationTime()) ;                 
            }
            else if (prop.getString() == "I2C")
            {
                SimulatorMessages &msg = getEngine().getMessageOutput() ;
                msg.startMessage(SimulatorMessages::MessageType::Error) ;
                msg << "model " << getModelName() << " instance " << getInstanceName() ;
                msg << " - the hardware interface 'I2C' is valid, but is not supported by the model (yet)" ;
                msg.endMessage(getEngine().getSimulationTime()) ; 
                return false ;
            }
            else if (prop.getString() == "UART")
            {
                SimulatorMessages &msg = getEngine().getMessageOutput() ;
                msg.startMessage(SimulatorMessages::MessageType::Error) ;
                msg << "model " << getModelName() << " instance " << getInstanceName() ;
                msg << " - the hardware interface 'UART' is valid, but is not supported by the model (yet)" ;
                msg.endMessage(getEngine().getSimulationTime()) ; 
                return false ;                
            }
            else
            {
                SimulatorMessages &msg = getEngine().getMessageOutput() ;
                msg.startMessage(SimulatorMessages::MessageType::Error) ;
                msg << "model " << getModelName() << " instance " << getInstanceName() ;
                msg << " - the hardware interface '" << prop.getString() << "' is not a valid interface, must be 'SPI', 'I2C', or 'UART'" ;
                msg.endMessage(getEngine().getSimulationTime()) ; 
                return false ;
            }

            HALSIM_RegisterSPIInitializedCallback(index_, SPIInitialize, this, true) ;
            HALSIM_RegisterSPIReadCallback(index_, SPIRead, this) ;
            HALSIM_RegisterSPIWriteCallback(index_, SPIWrite, this) ;

            return true ;
        }

        void NavXSim::run(uint64_t microdt) 
        {
            std::lock_guard<std::mutex> lock(lock_);            

            timestamp_ += microdt ;
            uint64_t ts = timestamp_ / 1000 ;

            registers_[NAVX_REG_TIMESTAMP_L_L] = static_cast<uint8_t>((ts >> 0) & 0xFF) ;
            registers_[NAVX_REG_TIMESTAMP_L_H] = static_cast<uint8_t>((ts >> 8) & 0xFF) ;
            registers_[NAVX_REG_TIMESTAMP_H_L] = static_cast<uint8_t>((ts >> 16) & 0xFF) ;
            registers_[NAVX_REG_TIMESTAMP_H_H] = static_cast<uint8_t>((ts >> 24) & 0xFF) ;
        }

        void NavXSim::SPIInitialize(const std::string &name, const struct HAL_Value *value)
        {
            if (name == "Initialized" && value->type == HAL_BOOLEAN && value->data.v_boolean)
            {
                active_ = true ;
                SimulatorMessages &msg = getEngine().getMessageOutput() ;
                msg.startMessage(SimulatorMessages::MessageType::Debug, 2) ;
                msg << "model " << getModelName() << " instance " << getInstanceName() ;
                msg << " - simulated NAVX SPI bus has been initialized" ;
                msg.endMessage(getEngine().getSimulationTime()) ;
            }                
        }

        uint8_t NavXSim::getCRC(uint8_t message[], uint8_t length)
        {
            uint8_t i, j, crc = 0;
            static const uint8_t poly = 0x91 ;

            for (i = 0; i < length; i++)
            {
                crc ^= message[i];
                for (j = 0; j < 8; j++)
                {
                    if (crc & 1) {
                        crc ^= poly;
                    }
                    crc >>= 1;
                }
            }
            return crc;
        }

        void NavXSim::SPIRead(const std::string &name, unsigned char *buffer, unsigned int count)
        {
            if (active_)
            {
                std::lock_guard<std::mutex> lock(lock_);

                SimulatorMessages &msg = getEngine().getMessageOutput() ;
                msg.startMessage(SimulatorMessages::MessageType::Debug, 9) ;
                msg << "model " << getModelName() << " instance " << getInstanceName() ;
                msg << " - simulated NAVX SPI bus read " << count << " bytes" ;
                msg.endMessage(getEngine().getSimulationTime()) ;

                assert(count - 1 == static_cast<unsigned int>(count_)) ;

                int index = 0 ;
                for(int i = 0 ; i < count_ ; i ++)
                    buffer[index++] = registers_[addr_++] ;
                
                buffer[index] = getCRC(buffer, count_) ;
            }

            read_transaction_ = false ;
        }

        void NavXSim::SPIWrite(const std::string &name, const unsigned char *buffer, unsigned int count)
        {
            if (active_)
            {
                if ((buffer[0] & 0x80) == 0x80)
                {
                    std::lock_guard<std::mutex> lock(lock_);
                                    
                    SimulatorMessages &msg = getEngine().getMessageOutput() ;
                    msg.startMessage(SimulatorMessages::MessageType::Debug, 9) ;
                    msg << "model " << getModelName() << " instance " << getInstanceName() ;
                    msg << " - simulated NAVX SPI bus write " << count << " bytes" ;
                    msg.endMessage(getEngine().getSimulationTime()) ;

                    int addr = buffer[0] & 0x7F ;
                    int index = 1 ;
                    count -= 2 ;
                    do {
                        registers_[addr++] = buffer[index++] ;
                        msg.startMessage(SimulatorMessages::MessageType::Debug, 9) ;
                        msg << "model " << getModelName() << " instance " << getInstanceName() ;
                        msg << " - wrote register " << (buffer[0] & 0x7F) << " value " << buffer[1] ;
                        msg.endMessage(getEngine().getSimulationTime()) ;
                    } while (--count != 0) ;
                }
                else if (!read_transaction_ && count == 3)
                {
                    SimulatorMessages &msg = getEngine().getMessageOutput() ;                     

                    addr_ = buffer[0] ;
                    count_ = buffer[1] ;

                    if (static_cast<size_t>(addr_) < registers_.size() && static_cast<size_t>(addr_ + count_) <= registers_.size())
                    {
                        msg.startMessage(SimulatorMessages::MessageType::Debug, 9) ;
                        msg << "model " << getModelName() << " instance " << getInstanceName() ;
                        msg << " - write portion of read request, addr " << addr_ << " count " << count_ ;
                        msg.endMessage(getEngine().getSimulationTime()) ;
                        read_transaction_ = true ;
                    }
                    else
                    {                       
                        msg.startMessage(SimulatorMessages::MessageType::Warning) ;
                        msg << "model " << getModelName() << " instance " << getInstanceName() ;
                        msg << " - invalid read request, addr " << addr_ << " count " << count_ ;
                        msg.endMessage(getEngine().getSimulationTime()) ;                        
                    }
                }
            }
        }

        void NavXSim::setYaw(double angle)
        {
            int16_t v = static_cast<int16_t>(angle * 100.0) ;
            uint8_t *p = reinterpret_cast<uint8_t *>(&v) ;
            registers_[NAVX_REG_YAW_L] = p[0] ;
            registers_[NAVX_REG_YAW_H] = p[1] ;            
        }
    }
}
