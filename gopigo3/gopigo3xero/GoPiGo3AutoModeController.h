#pragma once

#include <AutoController.h>

namespace xero {
	namespace gopigo {
		class GoPiGo3AutoModeController : public xero::base::AutoController {
		public:
			GoPiGo3AutoModeController(xero::base::Robot &robot) ;
			virtual ~GoPiGo3AutoModeController() ;

		private:
			xero::base::ActionPtr createAutoMode() ;
		} ;
	}
}