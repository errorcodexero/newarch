#pragma once

namespace xero
{
    namespace misc
    {
        class EncoderMapper
        {
        public:
            /// \brief create an endoer object
            /// \param rmax the value of the encoder in robot units at the point where the encoder units are maximumized
            /// \param rmin the value of the encoder in robot units at the point where the encoder units are minimized
            /// \param emax the maxmimum value of the encoder units
            /// \param emin the minimum value of the encoder units
            EncoderMapper(double rmax, double rmin, double emax, double emin) ;

            /// \brief destroys the encoder object
            virtual ~EncoderMapper() ;

            /// \brief calibrate the encoder
            /// Given a single point on both the encoder scale and the robot scale this 
            /// method calibrates the encoder.  This must be called to create the mapping from encoder units to
            /// robot units at this provide the information about where the units "wrap".
            /// \param robot a point on the robot scale for the encoder
            /// \param encoder a point on the encoder scale for the encoder
            void calibrate(double robot, double encoder) ;

            /// \brief convert robot units to encoder units
            /// \param encoder the encoder value in robot units
            /// \returns the value in encoder units
            double toRobot(double encoder) ;

            /// \brief convert robot units to encoder units
            /// \param robot the encoder value in robot units
            /// \returns the value in encoder units
            double toEncoder(double robot) ;

        private:
            double normalize(double value, double vmax, double vmin) ;
            double clamp(double value, double vmax, double vmin) ;

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