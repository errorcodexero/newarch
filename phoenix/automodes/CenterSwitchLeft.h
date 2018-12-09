#pragma once

#include "PhoenixAutoMode.h"

namespace xero {
	namespace phoenix {
		class CenterSwitchLeft : public PhoenixAutoMode
		{
		public:
			CenterSwitchLeft(Phoenix &robot) ;
			virtual ~CenterSwitchLeft() ;

		private:
			void create() ;
		} ;
	}
}