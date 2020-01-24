#include <NavXSim.h>
#include <SimulatorEngine.h>
#include <SimulatedMotor.h>
#include <mockdata/SPIData.h>
#include <IMURegisters.h>

using namespace xero::sim2;

xero::models::NavXSim *blah = nullptr ;

namespace xero
{
    namespace models
    {
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

        NavXSim::NavXSim(SimulatorEngine &engine, const std::string &inst) : SimulationModel(engine, "NavXSim", inst)
        {
            blah = this ;
            active_ = false ;

            for(size_t i = 0 ; i < registers_.size() ; i++)
                registers_[i] = 0 ;

            registers_[NAVX_REG_WHOAMI] = 0x32 ;          // Who AM I
            registers_[NAVX_REG_HW_REV] = 0x42 ;          // Board revision
            registers_[NAVX_REG_FW_VER_MAJOR] = 0x02 ;          // Firmware major revision
            registers_[NAVX_REG_FW_VER_MINOR] = 0x03 ;          // Firmware minor revision


        }

        NavXSim::~NavXSim()
        {
        }        

        bool NavXSim::create()
        {
            index_ = getInteger(IndexName) ;

            HALSIM_RegisterSPIInitializedCallback(index_, SPIInitialize, this, true) ;
            HALSIM_RegisterSPIReadCallback(index_, SPIRead, this) ;
            HALSIM_RegisterSPIWriteCallback(index_, SPIWrite, this) ;

            return true ;
        }

        void NavXSim::run(uint64_t microdt) 
        {
            std::lock_guard<std::mutex> lock(lock_);            

            uint64_t ts = getEngine().getSimulationTime() / 1000 ;

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

                int addr = buffer[0] ;
                int index = 0 ;
                count -= 2 ;
                do
                {
                    buffer[index++] = registers_[addr++] ;
                } while (--count != 0);
                
                buffer[index] = getCRC(buffer, count) ;
            }
        }

        void NavXSim::SPIWrite(const std::string &name, const unsigned char *buffer, unsigned int count)
        {
            if (active_ && (buffer[0] & 0x80) == 0x80)
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
        }
    }
}
