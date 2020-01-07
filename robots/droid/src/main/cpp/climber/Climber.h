#include <Subsystem.h>

namespace xero {
    namespace droid {
        class Climber : public xero::base::Subsystem {
        public:
            Climber(xero::base::Subsystem *parent);
            virtual ~Climber() {}
        private:
            uint64_t msg_id_;
        };
    }
}