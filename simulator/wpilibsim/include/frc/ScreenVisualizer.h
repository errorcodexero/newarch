#pragma once

#include "Visualizer.h"
#include "TankDriveModel.h"
#include "LifterModel.h"
#include "TurnTableModel.h"
#include <ncurses.h>

namespace xero {
    namespace sim {
        class ScreenVisualizer : public Visualizer {
        public:
            ScreenVisualizer(RobotSimBase &sim, const std::string &dbname) ;
            virtual ~ScreenVisualizer() ;

            virtual void beginCycle(double time) ;
            virtual void endCycle() ;       

            void windowSizeChanged() ;          
            virtual void visualizeSubsystem(std::shared_ptr<SubsystemModel> subsystem_p)  ;

        protected:

            void displayTankDrive(std::shared_ptr<TankDriveModel> subsystem_p, int row) ;
            void displayLifter(std::shared_ptr<LifterModel> subsystem_p, int row) ;
            void displayTurnTable(std::shared_ptr<TurnTableModel> subsystem_p, int row) ;

            WINDOW *getRobotWindow() {
                return robot_window_ ;
            }

            int getRobotWindowWidth() {
                return RobotWindowWidth ;
            }

            int getOIWindowWidth() {
                return OIWindowWidth ;
            }

            WINDOW *getOIWindow() {
                return oi_window_ ;
            }

            void initScreen() ;
            void deinitScreen() ;
            void drawField() ;
            void doLayout() ;

            virtual void plotRobot(int x, int y, double angle, int width, int length) ;

            virtual char getRobotChar() {
                return ' ' ;
            }

            virtual void drawGameSpecificField(WINDOW *win) {               
            }

            void physicalToScreen(double px, double py, int &sx, int &sy) ;
            void drawScreenRectangle(WINDOW *win, int x1, int y1, int width, int height) ;
            void drawFieldRectangle(WINDOW *win, double x1, double y1, double width, double height) ;
            void drawFilledScreenRectangle(WINDOW *win, char ch, int x1, int y1, int x2, int y2) ;
            void drawFilledFieldRectangle(WINDOW *win, char ch, double x1, double y1, double width, double height) ;    

        private:        
            static int angleToSide(double angle) ;
            void updateSmartDashboard() ;
            std::string getSmartDashboardKeyValue(const std::string &key) ;

        private:
            static const int RobotWindowWidth = 20 ;
            static const int OIWindowWidth = 24 ;
            static const char *rotate_chars_ ;              

        protected:
            WINDOW *field_window_ ;
            WINDOW *robot_window_ ;
            WINDOW *oi_window_ ;
            WINDOW *smart_window_ ;

            bool inited_ ;

            int subsystem_row_ ;

            // The last position of the robot
            int last_x_ ;
            int last_y_ ;           

            // The last angle of the robot
            double last_angle_ ;

            // If true, the last info is valid
            bool last_info_valid_ ;

            // Used to animate the rotating robot
            size_t same_count_ ;
            size_t rotate_index_ ;          

            // The physical dimensions of the field
            double max_x_ ;
            double max_y_  ;

            double scale_ ;

            // The layout of the field and the text area on the screen
            int width_ ;
            int height_ ;

            std::string drivebase_model_ ;  
        } ;
    }
}
