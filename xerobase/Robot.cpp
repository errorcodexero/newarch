#include "Robot.h"
#include "Subsystem.h"
#include "ControllerBase.h"
#include "AutoController.h"
#include "basegroups.h"
#include "OISubsystem.h"
#include <MessageDestStream.h>
#include <MessageDestSeqFile.h>
#include <MessageDestDS.h>
#include <frc/DriverStation.h>
#include <frc/Filesystem.h>
#include <iostream>
#include <cassert>

using namespace xero::misc ;

namespace xero {
    namespace base {
        
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
        Robot::Robot(const std::string &name, double looptime) {
            frc::SmartDashboard::PutString("State", "Creation");
            name_ = name ;

            target_loop_time_ = looptime ;

            last_time_ = frc::Timer::GetFPGATimestamp() ;

            parser_ = new SettingsParser(message_logger_, MSG_GROUP_PARSER) ;
            output_stream_ = nullptr ;

            sleep_time_.resize(static_cast<int>(LoopType::MaxValue)) ;
            std::fill(sleep_time_.begin(), sleep_time_.end(), 0.0) ;

            iterations_.resize(static_cast<int>(LoopType::MaxValue)) ;
            std::fill(iterations_.begin(), iterations_.end(), 0) ;

            setupPaths() ;

       
        }
#pragma GCC diagnostic pop

        Robot::~Robot() {
            delete parser_ ;
            message_logger_.clear() ;
            if (output_stream_ != nullptr)
                delete output_stream_ ;
        }

        void Robot::setupPaths() {
#if defined(SIMULATOR)
            log_dir_ = "." ;
            deploy_dir_ = "./deploy" ;
#elif defined(GOPIGO)
            log_dir_ = "/home/pi/logs/" ;
            deploy_dir_ = "/home/pi/deploy" ;
#elif defined(XEROROBORIO)
            wpi::SmallVector<char, '\0'> dir ;
            frc::filesystem::GetDeployDirectory(dir) ;     
            deploy_dir_ = &dir[0] ;
            deploy_dir_ = "/home/lvuser/deploy" ;
#else
#error SIMULATOR, GOPIGO, or XEROROBORIO must be defined
#endif
        }

        void Robot::initializeMessageLogger() {
            MessageLogger& logger = getMessageLogger();

            //
            // Enable message of all severities
            //
            logger.enableType(MessageLogger::MessageType::error);
            logger.enableType(MessageLogger::MessageType::warning);
            logger.enableType(MessageLogger::MessageType::info);
            logger.enableType(MessageLogger::MessageType::debug);

            enableSpecificMessages() ;


            // Set up message logger destination(s)
            std::shared_ptr<MessageLoggerDest> dest_p ;

#if defined(SIMULATOR)
            if (!isScreen())
            {
                dest_p = std::make_shared<MessageDestStream>(std::cout);
                logger.addDestination(dest_p);
            }
#else
            dest_p = std::make_shared<MessageDestDS>();
            logger.addDestination(dest_p);
#endif            

            //
            // This is where the roborio places the first USB flash drive it
            // finds.  Other drives are placed at /V, /W, /X.  The devices are
            // actually mounted at /media/sd*, and a symbolic link is created
            // to /U.
            //

            std::string logname("logfile_");
            dest_p = std::make_shared<MessageDestSeqFile>(log_dir_, logname);
            logger.addDestination(dest_p);
        }       

        void Robot::RobotHardwareInit() {
            //
            // This method must be provided by a derived class that initializes the hardware for the robot
            //
            assert(false) ;
        }

        void Robot::setupRobotOutputFile(const std::string &file) {
            if (file.length() > 0) {
                output_stream_ = new std::ofstream(file) ;
                if (output_stream_->fail() || output_stream_->bad()) {
                    delete output_stream_ ;
                    output_stream_ = nullptr ;
                }
                else {
                    auto dest = std::make_shared<MessageDestStream>(*output_stream_) ;
                    message_logger_.addDestination(dest) ;
                }
            }
        }       

        bool Robot::readParamsFile(const std::string &filename) {
            return parser_->readFile(filename) ;
        }

        bool Robot::readParamsFile() {
            std::string filename ;

            //
            // Setup access to the parameter file
            //
            filename = deploy_dir_ + "/" + name_ + ".dat" ;
            if (!readParamsFile(filename)) {
                std::cerr << "Robot  Initialization failed - could not read robot data file '" ;
                std::cerr << filename << "'" << std::endl ;
                assert(false) ;
            }

            return true ;
        }
        
        void Robot::robotLoop(LoopType type) {
            frc::DriverStation &ds = frc::DriverStation::GetInstance() ;
            voltage_ = ds.GetBatteryVoltage() ;
            int index = static_cast<int>(type) ;
            double initial_time = frc::Timer::GetFPGATimestamp();

            message_logger_.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_ROBOTLOOP) ;
            message_logger_ << "Entering robot loop" ;
            message_logger_.endMessage() ;

            delta_time_ = initial_time - last_time_ ;

            robot_subsystem_->computeState() ;

            message_logger_.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_ROBOTLOOP) ;
            message_logger_ << "RobotLoop: completed compute state" ;
            message_logger_.endMessage() ;

            if (controller_ != nullptr)
                controller_->run();

            message_logger_.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_ROBOTLOOP) ;
            message_logger_ << "RobotLoop: completed controller run" ;
            message_logger_.endMessage() ;

            robot_subsystem_->run() ;

            message_logger_.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_ROBOTLOOP) ;
            message_logger_ << "RobotLoop: completed subsystem run" ;
            message_logger_.endMessage() ;            

            iterations_[index]++ ;

            double elapsed_time = frc::Timer::GetFPGATimestamp() - initial_time > target_loop_time_;
            if (elapsed_time < target_loop_time_) {
                sleep_time_[index] += target_loop_time_ - elapsed_time ;
                frc::Wait(target_loop_time_ - elapsed_time);
            } else if (elapsed_time > target_loop_time_) {
                message_logger_.startMessage(MessageLogger::MessageType::warning) ;
                message_logger_ << "Robot loop exceeded target loop time\n";
                message_logger_ << "Loop time: " << elapsed_time << "\n";
                message_logger_.endMessage() ;
            }

            message_logger_.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_ROBOTLOOP) ;
            message_logger_ << "    completed delay" ;
            message_logger_.endMessage() ;               

            last_time_ = initial_time ;

            if ((iterations_[index] % 500) == 0) {
                double avg = sleep_time_[index] / iterations_[index] ;
                message_logger_.startMessage(MessageLogger::MessageType::info) ;
                message_logger_ << "RobotLoop:" ;
                message_logger_ << "iterations " << iterations_[index] ;
                message_logger_ << ", average sleep time " << avg ;
                message_logger_.endMessage() ;
            }
        }

        void Robot::RobotInit() {
            //
            // Initialize message logger
            //
            initializeMessageLogger();

            //
            // Print initialization messages
            //
            message_logger_.startMessage(MessageLogger::MessageType::info) ;
            message_logger_ << "Initializing robot " << name_  ;
            message_logger_.endMessage() ;

            //
            // Read parameters from the parameters file
            // 
            message_logger_.startMessage(MessageLogger::MessageType::info) ;
            message_logger_ << ".... reading parameter file" ;
            message_logger_.endMessage() ;            
            readParamsFile() ;

            //
            // Reading required paths
            //
            message_logger_.startMessage(MessageLogger::MessageType::info) ;
            message_logger_ << ".... loading path files" ;
            message_logger_.endMessage() ;     
            paths_ = std::make_shared<XeroPathManager>(message_logger_, deploy_dir_ + "/output") ;
            loadPaths() ;

            //
            // Initialize the robot hardware
            //
            message_logger_.startMessage(MessageLogger::MessageType::info) ;
            message_logger_ << ".... initializing hardware" ;
            message_logger_.endMessage() ;              
            RobotHardwareInit() ;

            //
            // Create the auto mode controller.  Its around for the complete lifecycle of the
            // robot object
            //
            auto_controller_ = std::dynamic_pointer_cast<AutoController>(createAutoController()) ;
            assert(auto_controller_ != nullptr) ;

            message_logger_.startMessage(MessageLogger::MessageType::info) ;
            message_logger_ << "Robot Initialization complete." ;
            message_logger_.endMessage() ;              
        }

        void Robot::displayAutoModeState() {
            std::string str = std::to_string(getAutoModelSelection()) ;
            frc::SmartDashboard::PutString("AutoModeNumber", str) ;
            frc::SmartDashboard::PutString("AutoModeName", auto_controller_->getAutoModeName()) ;
        }


        /// \brief return the selector switch value for the automode
        /// \returns auto mode selector switch value
        int Robot::getAutoModelSelection() {
            int sel = -1 ;

            //
            // We generally get the automode selection from the OI.  If you get the OI selection
            // in some other way (e.g. robot switch or SmartDashboard) override this function in 
            // your robot specific derived class.
            //
            if (oi_subsystem_ != nullptr) {
                oi_subsystem_->computeState() ;         
                sel = oi_subsystem_->getAutoModeSelector() ;
            }

            sel = 2 ;
            return sel ;
        }       

        void Robot::logAutoModeState() {
            std::string value ;
            frc::DriverStation &ds = frc::DriverStation::GetInstance() ;
            
            message_logger_.startMessage(MessageLogger::MessageType::info) ;
            message_logger_ << "Entering Autonomous mode: " ;
            message_logger_.endMessage() ;

            message_logger_.startMessage(MessageLogger::MessageType::info) ;
            message_logger_ << "    Auto Mode Number: " << getAutoModelSelection() ;
            message_logger_.endMessage() ;          

            message_logger_.startMessage(MessageLogger::MessageType::info) ;
            if (auto_controller_ == nullptr)
                message_logger_ << "    Auto Mode: NO AUTO MODE CONTROLLER" ;
            else
                message_logger_ << "    Auto Mode: " << auto_controller_->getAutoModeName() ;
            message_logger_.endMessage() ;

            value = "undefined" ;
            if (ds.GetAlliance() == frc::DriverStation::kRed)
                value = "red" ;
            else if (ds.GetAlliance() == frc::DriverStation::kBlue)
                value = "blue" ;

            message_logger_.startMessage(MessageLogger::MessageType::info) ;
            message_logger_ << "    Alliance: " << value ;
            message_logger_.endMessage() ;

            value = std::to_string(ds.GetLocation()) ;
            message_logger_.startMessage(MessageLogger::MessageType::info) ;
            message_logger_ << "    Location: " << value ;
            message_logger_.endMessage() ;              

            value = ds.GetGameSpecificMessage() ;
            message_logger_.startMessage(MessageLogger::MessageType::info) ;
            message_logger_ << "    GameData: " << value ;
            message_logger_.endMessage() ;      

            value = ds.GetEventName() ;
            message_logger_.startMessage(MessageLogger::MessageType::info) ;
            message_logger_ << "    EventName: " << value ;
            message_logger_.endMessage() ;

            value = "invalid" ;
            frc::DriverStation::MatchType mt = ds.GetMatchType() ;
            switch(mt) {
            case frc::DriverStation::kPractice:
                value = "practice" ;
                break ;
            case frc::DriverStation::kNone:
                value = "none" ;
                break ;
            case frc::DriverStation::kQualification:
                value = "qualification" ;
                break ;
            case frc::DriverStation::kElimination:
                value = "elimination" ;
                break ;                                             
            }

            message_logger_.startMessage(MessageLogger::MessageType::info) ;
            message_logger_ << "    MatchType: " << value ;
            message_logger_.endMessage() ;

            value = std::to_string(ds.GetMatchNumber()) ;
            message_logger_.startMessage(MessageLogger::MessageType::info) ;
            message_logger_ << "    MatchNumber: " << value ;
            message_logger_.endMessage() ;  

            value = std::to_string(ds.GetReplayNumber()) ;
            message_logger_.startMessage(MessageLogger::MessageType::info) ;
            message_logger_ << "    ReplayNumber: " << value ;
            message_logger_.endMessage() ;                      

            value = "No" ;
            if (ds.IsFMSAttached())
                value = "Yes" ;

            message_logger_.startMessage(MessageLogger::MessageType::info) ;
            message_logger_ << "    FMS Attached: " << value ;
            message_logger_.endMessage() ;              
        }

        void Robot::updateAutoMode() {
            if (auto_controller_ != nullptr && oi_subsystem_ != nullptr) {

                auto &ds = frc::DriverStation::GetInstance() ;
                std::string msg = ds.GetGameSpecificMessage() ;             

                //
                // Query the OI subsystem to get the automode nubmer selected
                //

                //
                // If we have an automode controller (should be always), we allow the
                // auto mode controller to look at the automode selection and do any 
                // initialization it can while the robot is disabled to get ready for
                // the automode program.  This also includes providing information for
                // the drive team about what automode is selected
                //
                int sel = getAutoModelSelection() ;

                if (sel != automode_ || msg != gamedata_) {
                    automode_ = sel ;
                    gamedata_ = msg ;
                    frc::SmartDashboard::PutBoolean("ready", false);
                    auto_controller_->updateAutoMode(sel, gamedata_);
                    frc::SmartDashboard::PutBoolean("ready", true);
                    displayAutoModeState() ;
                }
            }           
        }

        void Robot::Autonomous() {
            //
            // Just in case something like the field data changes as we enter
            // the autonomous state
            //
            updateAutoMode() ;
            logAutoModeState() ;

            controller_ = auto_controller_ ;

            robot_subsystem_->init(LoopType::Autonomous) ;

            while (IsAutonomous() && IsEnabled())
                robotLoop(LoopType::Autonomous);

            controller_ = nullptr ;

            message_logger_.startMessage(MessageLogger::MessageType::info) ;
            message_logger_ << "Leaving Autonomous mode" ;
            message_logger_.endMessage() ;

            robot_subsystem_->reset() ;
        }

        void Robot::OperatorControl() {
            message_logger_.startMessage(MessageLogger::MessageType::info) ;
            message_logger_ << "Starting Teleop mode" ;
            message_logger_.endMessage() ;           

            controller_ = createTeleopController() ;

            robot_subsystem_->init(LoopType::OperatorControl) ;         

            while (IsOperatorControl() && IsEnabled())
                robotLoop(LoopType::OperatorControl) ;

            controller_ = nullptr ;

            message_logger_.startMessage(MessageLogger::MessageType::info) ;
            message_logger_ << "Leaving Teleop mode" ;
            message_logger_.endMessage() ;  

            robot_subsystem_->reset() ;                 
        }

        void Robot::Test() {
            message_logger_.startMessage(MessageLogger::MessageType::info) ;
            message_logger_ << "Starting Test mode" ;
            message_logger_.endMessage() ;

            controller_ = createTestController() ;

            robot_subsystem_->init(LoopType::Test) ;

            while (IsTest() && IsEnabled())
                robotLoop(LoopType::Test) ;

            controller_ = nullptr ;

            message_logger_.startMessage(MessageLogger::MessageType::info) ;
            message_logger_ << "Leaving Test mode" ;
            message_logger_.endMessage() ;      

            robot_subsystem_->reset() ;             
        }

        void Robot::Disabled() {

            message_logger_.startMessage(MessageLogger::MessageType::info) ;
            message_logger_ << "Robot Disabled" ;
            message_logger_.endMessage() ;

            automode_ = -1 ;

            while (IsDisabled()) {
                updateAutoMode() ;
                robot_subsystem_->computeState() ;
                frc::Wait(target_loop_time_) ;              
            }
            
            message_logger_.startMessage(MessageLogger::MessageType::info) ;
            message_logger_ << "Leaving Robot Disabled" ;
            message_logger_.endMessage() ;
        }
    }
}
