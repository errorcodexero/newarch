#pragma once

#include "PhoenixAutoMode.h"

namespace xero {
	namespace phoenix {
		class CrossTheLine : public PhoenixAutoMode
		{
		public:
			CrossTheLine(Phoenix &robot) ;
			virtual ~CrossTheLine() ;

		private:
			void create() ;
		} ;
	}
}