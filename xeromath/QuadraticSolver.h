#pragma once

#include <vector>

namespace xero {
    namespace math {

		class QuadraticSolver {
		public:
			QuadraticSolver() = delete ;
			~QuadraticSolver() = delete ;

			static std::vector<double> solve(double a, double b, double c) ;
		} ;
	}
}
