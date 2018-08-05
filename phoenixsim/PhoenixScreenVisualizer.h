#include <Visualizer.h>

#ifdef XEROSCREEN
#include <TankDriveModel.h>
#include "LifterModel.h"
#include "IntakeModel.h"
#include "GrabberModel.h"
#include "WingsModel.h"
#include "CubeSensorModel.h"

#include <ncurses.h>

namespace xero {
    namespace sim {
        namespace phoenix {
            class PhoenixScreenVisualizer : public Visualizer {
            public:
                PhoenixScreenVisualizer(RobotSimBase &sim) ;
                virtual ~PhoenixScreenVisualizer() ;

                virtual void beginCycle(double time) ;
                virtual void visualizeSubsystem(std::shared_ptr<SubsystemModel> subsystem_p) ;
                virtual void endCycle() ;

                void windowSizeChanged() ;

            private:
                static const int RightSideWidth = 40 ;
                static const int TimeRow = 1 ;
                static const int TankDriveRow = 4 ;
                static const int LifterRow = 10 ;
                static const int IntakeRow = 17 ;
                static const int GrabberRow = 21 ;
                static const int WingsRow = 25 ;
                static const int CubeSensorRow = 28 ;
                static const int MinimumHeight = 30 ;

                static const char *rotate_chars_ ;

                static const char withcube = '@' ;
                static const char withoutcube = '$' ;

            private:
                void initScreen() ;
                void deinitScreen() ;
                void doLayout() ;

                void drawField() ;
                void drawTextField() ;

                void displayTankDrive(std::shared_ptr<xero::sim::TankDriveModel> subsystem_p) ;
                void displayLifter(std::shared_ptr<LifterModel> subsystem_p) ;
                void displayGrabber(std::shared_ptr<GrabberModel> subsystem_p) ;
                void displayIntake(std::shared_ptr<IntakeModel> subsystem_p) ;
                void displayWings(std::shared_ptr<WingsModel> subsystem_p) ;
                void displayCubeSensor(std::shared_ptr<CubeSensorModel> subsystem_p) ;

                void plotRobot(double x, double y, double angle) ;

            private:
                bool inited_ ;
                int width_ ;
                int height_ ;
                int right_side_ ;

                double min_x_ ;
                double min_y_ ;
                double max_x_ ;
                double max_y_  ;

                int last_row_ ;
                int last_col_ ;

                double last_angle_ ;

                size_t same_count_ ;
                size_t rotate_index_ ;

		bool cube_ ;
            } ;
        }
    }
}

#endif
