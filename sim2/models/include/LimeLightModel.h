#pragma once
#include <SimulationModel.h>
#include <SimValue.h>
#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableInstance.h>
#include <memory>
#include <mutex>

namespace xero
{
    namespace models
    {
        class LimeLightModel : public xero::sim2::SimulationModel
        {
        public:
            LimeLightModel(xero::sim2::SimulatorEngine &engine, const std::string &inst);
            virtual ~LimeLightModel();

            virtual bool create() ;
            virtual void run(uint64_t microdt);
            void processEvent(const std::string &name, const xero::sim2::SimValue &value) ;

        private:
            static constexpr const char *tablename = "limelight" ;
            static constexpr const char *tvkey = "tv" ;
            static constexpr const char *txkey = "tx" ;
            static constexpr const char *tykey = "ty" ;            
            static constexpr const char *takey = "ta" ;

        private:
            std::shared_ptr<nt::NetworkTable> table_ ;        
        };
    }
}