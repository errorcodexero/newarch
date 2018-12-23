#pragma once

namespace xero {
	namespace base {
		/// \brief The type of robot loop we are running
		enum class LoopType {
			OperatorControl,				///< Operator control robot loop
			Autonomous,						///< Autnomous robot loop
			Test,							///< Test robot loop
		} ;		
	}
}