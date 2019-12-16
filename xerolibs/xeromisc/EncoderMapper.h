#pragma once

namespace xero
{
    namespace misc
    {
        class EncoderMapper
        {
        public:
            EncoderMapper(double rmax, double rmin, double emax, double emin) ;
            virtual ~EncoderMapper() ;

            void calibrate(double robot, double encoder) ;

            double toRobot(double encoder) ;

        private:
            // The maximum robot value for the mechanism (inches, degrees, etc.)
            double rmax_ ;

            // The minimum robot value for the mechanism
            double rmin_ ;

            // The conversion from encoder units to robot units
            double kEncoder2Robot_ ;

            // The maximum value of the encoder as read from the encoder
            double emax_ ;

            // The minimum value of the encoder as read from the encoder
            double emin_ ;

            // The calibration value of the encoder
            double ec_ ;

            // The calibration value of the robot
            double rc_ ;
        } ;
    }
}