#pragma once
#include <SimulatedMotor.h>
#include <SimulationModel.h>
#include <mockdata/SPIData.h>
#include <memory>
#include <mutex>

namespace xero
{
    namespace models
    {
        class NavXSim : public xero::sim2::SimulationModel
        {
        public:
            NavXSim(xero::sim2::SimulatorEngine &engine, const std::string &inst);
            virtual ~NavXSim();

            virtual bool create() ;
            virtual void run(uint64_t microdt);

        private:
            constexpr static const char *IndexName = "hw:index" ;

            static void SPIInitialize(const char *name, void *param, const struct HAL_Value *value) ;
            static void SPIRead(const char *name, void *param, unsigned char *buffer, unsigned int count) ;
            static void SPIWrite(const char *name, void *param, const unsigned char *buffer, unsigned int count) ;

            void SPIInitialize(const std::string &name, const struct HAL_Value *value) ;
            void SPIRead(const std::string &name, unsigned char *buffer, unsigned int count) ;
            void SPIWrite(const std::string &name, const unsigned char *buffer, unsigned int count) ;

            uint8_t getCRC(uint8_t message[], uint8_t length) ;

        private:
            int index_ ;
            bool active_ ;
            std::array<uint8_t, 256> registers_ ;
            std::mutex lock_ ;
        };
    }
}