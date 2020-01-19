#pragma once

#include <WestCoastDrive.h>
#include <engine/ModelFactory.h>
#include <memory>

namespace xero {
    namespace models
    {
        class WestCoastDriveFactory : public xero::sim2::ModelFactory
        {
        public:
            WestCoastDriveFactory() : xero::sim2::ModelFactory("westcoastdrive") {                
            }

            virtual ~WestCoastDriveFactory() {                
            }

            std::shared_ptr<xero::sim2::SimulationModel> createModel() {
                return std::make_shared<WestCoastDrive>();
            }

        private:
            std::string name_;
        };
    }
}
