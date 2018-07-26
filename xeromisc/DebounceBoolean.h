#pragma once

namespace xero {
    namespace misc {
        class DebounceBoolean {
        public:
            DebounceBoolean(bool init, double delay) {
                state_ = init ? State::high : State::low ;
                delaylow_ = delay ;
                delayhigh_ = delay ;
            }

            DebounceBoolean(bool init, double low, double high) {
                state_ = init ? State::high : State::low ;
                delaylow_ = low ;
                delayhigh_ = high ;
            }            

            bool getState(bool st, double now) {
                bool ret = false ;

                switch(state_) {
                case State::low:
                    ret = false ;
                    if (st) {
                        //
                        // Start the transition to high
                        //
                        state_ = State::maybehigh ;
                        start_ = now ;
                    }
                    break ;

                case State::maybehigh:
                    ret = false ;
                    if (!st)
                        state_ = State::low ;
                    else if (now - start_ > delayhigh_) {
                        ret = true ;
                        state_ = State::high ;
                    }
                    break ;

                case State::high:
                    ret = true ;
                    if (!st) {
                        state_ = State::maybelow ;
                        start_ = now ;
                    }
                    break ;

                case State::maybelow:
                    ret = true ;
                    if (st)
                        state_ = State::high ;
                    else if (now - start_ > delaylow_) {
                        ret = false ;
                        state_ = State::low ;
                    }
                    break ;
                }

                return ret ;
            }

        private:
            enum class State { high, maybehigh, low, maybelow } ;

        private:
            double start_ ;
            double delaylow_ ;
            double delayhigh_ ;
            State state_ ;
        } ;
    }
}