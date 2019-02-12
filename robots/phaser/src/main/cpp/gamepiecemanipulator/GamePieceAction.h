#pragma once

#include <Action.h>

namespace xero {
    namespace phaser {
        class GamePiece ;

        class GamePieceAction : public xero::base::Action {
        public:
            GamePieceAction(GamePiece &game_piece) : game_piece_(game_piece) {                
            }
            
            GamePiece &getGamePiece() {
                return game_piece_ ;
            }

        private:
            GamePiece &game_piece_ ;
        } ;
    }
}
