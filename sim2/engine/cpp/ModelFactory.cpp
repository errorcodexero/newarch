#include <ModelFactory.h>

namespace xero
{
    namespace sim2
    {
        std::list<std::string> &ModelFactoryBase::getModelList() {
            static std::list<std::string> models_;
            return models_;
        }
    }
}
