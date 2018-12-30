#pragma once

namespace frc
{
	class SendableBuilder ;
	
	class SendableBase
	{
	public:
		virtual void InitSendable(SendableBuilder &builder) {			
		}
	} ;

}
