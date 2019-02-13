#pragma once

#include <Action.h>

namespace xero {
    namespace phaser {
        class GamePieceManipulator ;

        class GamePieceAction : public xero::base::Action {
        public:
            GamePieceAction(GamePieceManipulator &game_piece) : game_piece_(game_piece) {                
            }
            
            GamePieceManipulator &getGamePiece() {
                return game_piece_ ;
            }

        private:
            GamePieceManipulator &game_piece_ ;
        } ;
    }
}
