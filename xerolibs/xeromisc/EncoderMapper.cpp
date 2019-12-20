#include "EncoderMapper.h"
#include <iostream>
#include <cmath>

namespace xero
{
    namespace misc
    {
        EncoderMapper::EncoderMapper(double rmax, double rmin, double emax, double emin)
        {    
            rmax_ = rmax ;
            rmin_ = rmin ;
            emax_ = std::max(emax, emin) ;
            emin_ = std::min(emax, emin) ;
            kEncoder2Robot_ = (rmax - rmin) / (emax - emin) ;        
        }

        EncoderMapper::~EncoderMapper()
        {                
        }

        void EncoderMapper::calibrate(double encoder, double robot)
        {
            ec_ = encoder ;
            rc_ = robot ;
        }

        double EncoderMapper::toRobot(double encoder)
        {
            double ret ;
            double offset; 

            if (encoder < emin_)
                encoder = emin_ ;
            else if (encoder > emax_)
                encoder = emax_ ;

            offset = ec_ - (rc_ - rmin_) / kEncoder2Robot_ ;   
            if (offset < emin_)
                offset += emax_ ;
            else if (offset > emax_)
                offset -= emax_ ;

            if (kEncoder2Robot_ < 0.0)
            {
                ret = emax_ - encoder + offset ;
            }
            else
            {
                ret = encoder - offset ;
            }

            if (ret < emin_)
                ret += emax_ ;
            else if (ret > emax_)
                ret -= emax_ ;

            ret *= std::fabs(kEncoder2Robot_) ;

            return ret ;
        }
    }
}