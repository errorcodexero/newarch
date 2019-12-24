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
            emax_ = emax ;
            emin_ = emin ;
            kEncoder2Robot_ = (rmax - rmin) / (emax - emin) ;        
        }

        EncoderMapper::~EncoderMapper()
        {                
        }

        double EncoderMapper::normalize(double value, double vmax, double vmin)
        {
            if (vmax < vmin)
            {
                double temp = vmax ;
                vmax = vmin ;
                vmin = temp ;
            }

            while (value < vmin)
            {
                value += (vmax - vmin) ;
            }

            while (value > vmax)
            {
                value -= (vmax - vmin) ;
            }

            return value ;
        }

        double EncoderMapper::clamp(double value, double vmax, double vmin)
        {
            if (vmax < vmin)
            {
                double temp = vmax ;
                vmax = vmin ;
                vmin = temp ;
            }

            if (value > vmax)
                value = vmax ;
            else if (value < vmin)
                value = vmin ;

            return value ;
        }

        void EncoderMapper::calibrate(double robot, double encoder)
        {
            ec_ = encoder ;
            rc_ = robot ;
        }

        double EncoderMapper::toRobot(double encoder)
        {
            double ret ;
            double offset; 

            encoder = clamp(encoder, emax_, emin_) ;
            offset = normalize(ec_ - (rc_ - rmin_) / kEncoder2Robot_, emax_, emin_) ;
            ret = normalize((encoder - offset) * kEncoder2Robot_, rmax_, rmin_) ;
            
            return ret ;
        }

        double EncoderMapper::toEncoder(double robot)
        {
            double ret ;
            double offset ;

            robot = clamp(robot, rmax_, rmin_) ;
            offset = normalize(ec_ - (rc_ - rmin_) / kEncoder2Robot_,  emax_, emin_) ;
            ret = normalize(offset + robot / kEncoder2Robot_,  emax_, emin_) ;
            
            return ret ;
        }
    }
}