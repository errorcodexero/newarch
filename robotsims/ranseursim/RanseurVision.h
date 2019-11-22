#pragma once

#include <TankDriveModel.h>
#include <frc/SubsystemModel.h>
#include <vector>
#include <string>

namespace xero  {
    namespace sim {
        namespace ranseur {
            class RanseurVision : public SubsystemModel
            {
            public:
                RanseurVision(RobotSimBase &sim, xero::sim::TankDriveModel &model) ;
                virtual ~RanseurVision() ;

                /// \brief Convert the model to a string.
                /// \returns details about the lifter's state
                virtual std::string toString() ;

                // Process events from the simulator
                virtual bool processEvent(const std::string &name, int value) ;

                /// \brief Run the model.
                /// \param dt the time difference since the last code loop
                virtual void run(double dt);

                /// \brief Update the state of the model.  Called when one of its inputs changes.
                /// \param obj the input object which changed state
                virtual void inputChanged(SimulatedObject *obj);

                /// \brief Generate display information for the model
                void generateDisplayInformation(std::list<std::string> &lines) ;

            private:
                static constexpr const char *table_name = "limelight" ;

                void compute() ;

            private:
                int tv_ ;           // If 1, there is a valid target
                double tx_ ;        // Horizontal offset from crosshair to target
                double ty_ ;        // Vertical offset from crosshair to target
                double ta_ ;        // Target area (0% - 100% of image)
                double ts_ ;        // Skew or rotation (-90 to 0 degrees)
                double tl_ ;        // Pipeline latency (add 11 ms for image capture)
                double tshort_ ;    // Sidelength of shortest side of bounding box
                double tlong_ ;     // Sidelength of longest side of bounding box
                double thor_ ;      // Horizontal sidelength of rough bounding box
                double tvert_ ;     // Vertical sidelength of rough bounding box
                int getpipe_ ;      // True active pipline index of the camera
                std::vector<double> camtran_ ;   // Result of 3d position solution, 6 numbers

                double tubxpos_ ;
                double tubypos_ ;
                bool tubxpos_set_ ;
                bool tubypos_set_ ;

                xero::sim::TankDriveModel &tankdrive_ ;
            } ;
        }
    }
}
