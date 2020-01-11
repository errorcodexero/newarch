#include <Subsystem.h>

namespace xero {
    namespace droid {
        class Shooter;
        class Collector;
        class Conveyor;

        class GamePieceManipulator : public xero::base::Subsystem {
        public:
            GamePieceManipulator(xero::base::Subsystem *parent);
            virtual ~GamePieceManipulator() {}
          
            std::shared_ptr<Shooter> getShooter() {
                return shooter_ ;
            }
          
            std::shared_ptr<Collector> getCollector() {
                return collector_ ;
            }    
          
            std::shared_ptr<Conveyor> getConveyor() {
                return conveyor_ ;
            }    

        private:
            std::shared_ptr<Shooter> shooter_;
            std::shared_ptr<Collector> collector_;
            std::shared_ptr<Conveyor> conveyor_;
        };
    }
}