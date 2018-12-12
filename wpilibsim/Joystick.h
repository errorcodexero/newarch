#pragma once

namespace frc {
	class Joystick {
	public:
		Joystick(int port) {
		}

		virtual ~Joystick() {
		}

		void SetOutput(int output, bool state) {
		}
	} ;
}