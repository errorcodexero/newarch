#include <singlemotorsubsystem/SingleMotorSubsystem.h>

namespace xero {
    namespace droid {
        class Conveyor : public xero::base::SingleMotorSubsystem {
            friend class ConveyorPrepareToEmitAction;
            friend class ConveyorReceiveAction;
            friend class ConveyorEmitAction;
        public:
            Conveyor(xero::base::Subsystem *parent);
            virtual ~Conveyor() {}

            void postHWInit() override;

            int getBallCount() { return ballCount_; }

            static const int MAX_BALLS = 5;

        private:
            int ballCount_ = 0;
        };
    }
}