#pragma once

/// \file

namespace xero {
    namespace misc {
        /// \brief A utility class for debouncing a boolean value
        class DebounceBoolean {
        public:
            /// \brief Create a new boolean debouncer
            /// \param init the initial state of the boolean
            /// \param delay the delay before confirming the state of the boolean
            DebounceBoolean(bool init, double delay) {
                state_ = init ? State::high : State::low ;
                delaylow_ = delay ;
                delayhigh_ = delay ;
            }

            /// \brief Create a new boolean debouncer
            /// \param init the initial state of the boolean
            /// \param low the delay before confirming the state of the boolean as low
            /// \param high the delay before confirming the state of the boolean as high
            DebounceBoolean(bool init, double low, double high) {
                state_ = init ? State::high : State::low ;
                delaylow_ = low ;
                delayhigh_ = high ;
            }            

            /// \brief Return the debounced state of the boolean
            /// \param st the state of the original boolean
            /// \param now the current time
            /// \returns the debounced state of the boolean
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
