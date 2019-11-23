#include "RanseurDiagnosticAction.h"
#include "RanseurSubsystemAction.h"
#include "RanseurRobotSubsystem.h"
#include <MessageLogger.h>
#include <Robot.h>
#include <actions/Action.h>

using namespace xero::misc ;
using namespace xero::base ;

namespace xero {
    namespace ranseur {

        RanseurDiagnosticAction::RanseurDiagnosticAction(RanseurRobotSubsystem &robotss) : RanseurSubsystemAction(robotss) {
        }

        RanseurDiagnosticAction::~RanseurDiagnosticAction() {            
        }
        
        void RanseurDiagnosticAction::start() {
            index_ = 0 ;
            count_ = -1 ;
            on_ = false ;
            between_ = false ;
        }

        void RanseurDiagnosticAction::run() {
            auto rans = getRanseurSubsystem() ;

            if (count_ == -1)
            {
                //
                // This means we are ready to start with a new DIO
                //

                //
                // Blink the IO a number of times based on the index of the DIO
                // (e.g. Blink the first one once, the second one twice, etc.)
                //
                count_ = index_ + 1 ;                   // The number of times to blink
                on_ = true ;                            // The current LED state
                start_ = rans.getRobot().getTime() ;    // The start time for this blink
                rans.setDIOState(index_, on_) ;         // Set the DIO to match the state of the on_ var
            }

            if (between_)
            {
                if (rans.getRobot().getTime() - start_ > blinktime)
                {             
                    //
                    // We just finished a given DIO and the delay after that DIO so 
                    // now lets move to the next DIO.
                    //

                    // Move to the next DIO
                    index_++ ;
                    if (index_ == 10)
                        index_ = 0 ;

                    //
                    // Set count to -1 to cause this code to initialize the next time
                    // for the current DIO, and turn off the between state
                    //
                    count_ = -1 ;
                    between_ = false ;                    
                }
            }
            else
            {
                if (rans.getRobot().getTime() - start_ > blinktime)
                {
                    //
                    // The current half cycle (either on or off) is complete
                    //

                    on_ = !on_ ;                            // Toggle to the next state
                    rans.setDIOState(index_, on_) ;         // Set the digital IO to match next state
                    start_ = rans.getRobot().getTime() ;    // Start the time for the current blink state

                    if (!on_)
                    {
                        //
                        // If we just transitioned from ON to OFF, we decrement the blink count
                        //
                        count_-- ;

                        //
                        // If the blink count is zero, we are done with the DIO, so enter the between state
                        // which is a long pause between blinks
                        //
                        if (count_ == 0)
                        {
                            between_ = true ;
                            start_ = rans.getRobot().getTime() ;
                        }
                    }
                }
            }
        }

        bool RanseurDiagnosticAction::isDone() {
            return false ;
        }

        void RanseurDiagnosticAction::cancel() {
        }
    }
}
