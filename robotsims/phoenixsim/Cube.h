#pragma once

namespace xero {
    namespace sim {
        namespace phoenix {
            class Cube {
            public:
                Cube(double x, double y) {
                    x_ = x ;
                    y_ = y ;
                }

                double getX() const {
                    return x_ ;
                }

                double getY() const {
                    return y_ ;
                }

            private:
                double x_ ;
                double y_ ;
            } ;
        }
    }
}
