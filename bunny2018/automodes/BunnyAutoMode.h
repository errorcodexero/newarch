#pragma once

#include <AutoController.h>
#include <Action.h>
#include <ActionSequence.h>

namespace xero {
	namespace bunny2018 {
		/// \brief This is the base class for any autonomous mode controller
		/// This class contains the code to execute an automode program and
		/// the derived classs generally creates the auto program for a given
		/// robot, automode selector, and set of field conditions.
		class BunnyAutoMode : public xero::base::AutoController
		{
		public:
			BunnyAutoMode(xero::base::Robot &robot) ;

			virtual ~BunnyAutoMode() ;

			virtual void update(int sel) ;

        private:
			xero::base::ActionSequencePtr createAutoModeZero() ;
			xero::base::ActionSequencePtr createAutoModeOne() ;
			xero::base::ActionSequencePtr createAutoModeTwo() ;
			xero::base::ActionSequencePtr createAutoModeThree() ;
			xero::base::ActionSequencePtr createAutoModeFour() ;	
			xero::base::ActionSequencePtr createAutoModeFive() ;
			xero::base::ActionSequencePtr createAutoModeSix() ;
			xero::base::ActionSequencePtr createAutoModeSeven() ; 			
			xero::base::ActionSequencePtr createStageBallAutomode() ; 			
			xero::base::ActionSequencePtr createStraightBackAutomode() ;
			xero::base::ActionSequencePtr createRotateSorterAutoMode() ;
			xero::base::ActionSequencePtr createTestAlignAutoMode() ;
		};
	}
}
