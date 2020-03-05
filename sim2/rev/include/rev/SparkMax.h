#pragma once

namespace rev
{
    class SparkMax
    {
    public:
        SparkMax(int id) {
            id_ = id ;
        }

        virtual ~SparkMax() {
        }

        void Set(double v) {            
        }

    private:
        int id_ ;
    } ;
}
