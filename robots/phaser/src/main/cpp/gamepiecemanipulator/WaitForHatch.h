#pragma once 

#include "GamePieceAction.h"
#include "GamePieceManipulator.h"

namespace xero{
    namespace phaser{
        class WaitForHatch : public GamePieceAction {
        public:
            WaitForHatch(GamePieceManipulator &subsystem, bool present) ;
            virtual ~WaitForHatch() ;

            virtual void start() ;
            virtual void run() ;
            virtual bool isDone() ;
            virtual void cancel() ;
            virtual std::string toString() ;

        private:
            bool present_ ;
        } ;
    }
}