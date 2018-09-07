#pragma once

#include <xeromath.h>

namespace xero {
	namespace misc {
		class Kinematics {
		public:
			Kinematics(double width, double scrub) ;
			virtual ~Kinematics() ;

			double getX() const {
				return x_ ;
			}

			double getY() const {
				return y_ ;
			}

			double getAngle() const {
				return angle_ ;
			}

			void set(double x, double y, double angle) ;
			void set (double x, double y) ;

			void move(double right, double left) ;
			void move(double right, double left, double angle) ;

		private:
			static constexpr double StraightTolerance = 1e-6 ;

		private:
			double x_ ;
			double y_ ;
			double angle_ ;
			double scrub_ ;
			double width_ ;
		} ;
	}
}