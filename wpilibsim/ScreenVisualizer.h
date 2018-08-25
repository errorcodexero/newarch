#pragma once

#include "Visualizer.h"
#include <ncurses.h>

namespace xero {
	namespace sim {
		class ScreenVisualizer : public Visualizer {
		public:
			ScreenVisualizer(RobotSimBase &sim, const std::string &dbname) ;
			virtual ~ScreenVisualizer() ;

            virtual void beginCycle(double time) ;
            virtual void visualizeSubsystem(std::shared_ptr<SubsystemModel> subsystem_p)  {				
			}
            virtual void endCycle() ;		

            void windowSizeChanged() ;			

		protected:

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

			int getFirstSubsystemRow() const {
				return TimeRow + 3 ;
			}

			void initScreen() ;
			void deinitScreen() ;
			void drawField() ;
			void doLayout() ;
			void plotRobot(double x, double y, double angle) ;

			virtual char getRobotChar() {
				return '@' ;
			}

			virtual std::string getModeString() {
				return "NoModeDefined" ;
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

		private:
            static const int RobotWindowWidth = 20 ;
			static const int OIWindowWidth = 24 ;
            static const int TimeRow = 1 ;		
            static const char *rotate_chars_ ;				

		private:
			WINDOW *field_window_ ;
			WINDOW *robot_window_ ;
			WINDOW *oi_window_ ;

			bool inited_ ;

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

			// The robot width and height
			int robot_width_ ;
			int robot_height_ ;

			std::string drivebase_model_ ;	
		} ;
	}
}