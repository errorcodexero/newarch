#pragma once

#include <AutoController.h>

namespace xero {
	namespace gopigo {
		class GoPiGo3AutoModeController : public xero::base::AutoController {
		public:
			GoPiGo3AutoModeController(xero::base::Robot &robot) ;
			virtual ~GoPiGo3AutoModeController() ;

			virtual void updateAutoMode(int sel, const std::string &gamedata) ;

		private:
			xero::base::ActionSequencePtr createAutoMode() ;
		} ;
	}
}