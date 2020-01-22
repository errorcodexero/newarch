#include <SimulatorEngine.h>
#include <SimulationEvent.h>
#include <SimulationModel.h>
#include <ModelFactory.h>
#include <SimulatorStreamMessageSink.h>
#include <CTREManager.h>
#include <REVManager.h>
#include <json.h>
#include <hal/HalBase.h>
#include <mockdata/EncoderData.h>
#include <mockdata/PWMData.h>
#include <cassert>
#include <iostream>
#include <fstream>

namespace xero
{
    namespace sim2
    {
        SimulatorEngine *SimulatorEngine::theOne = nullptr;

        SimulatorEngine &SimulatorEngine::getEngine() {
            if (theOne == nullptr)
                theOne = new SimulatorEngine();
            return *theOne;
        }

        SimulatorEngine::SimulatorEngine()
        {
            auto sink = std::make_shared<SimulatorStreamMessageSink>(std::cout);
            msg_.addSink(sink);

            props_ = std::make_shared<SimulationProperties>(*this);
            events_ = std::make_shared<EventsManager>(*this);

            // Create the hardware managers for things not covered by the HAL
            ctre_mgr_ = std::make_shared<CTREManager>(*this);
            rev_mgr_ = std::make_shared<REVManager>(*this);            

            sim_time_step_ = 1000 ;
        }

        SimulatorEngine::~SimulatorEngine()
        {
            assert(theOne == this);
            theOne = nullptr;
        }

        std::shared_ptr<SimulationModel> SimulatorEngine::createModelInstance(const std::string &model, const std::string &inst)
        {
            auto it = model_factories_.find(model);
            if (it == model_factories_.end())
                return nullptr;

            return it->second->createModel(*this, inst);
        }

        bool SimulatorEngine::parseCommandLineArgs(int ac, char **av)
        {
            ac--;
            av++;

            while (ac-- > 0)
            {
                std::string arg = *av++;

                if (arg == "--events")
                {
                    if (ac == 0)
                    {
                        msg_.startMessage(SimulatorMessages::MessageType::Error);
                        msg_ << "error: command line option '--events' requires an additional argument";
                        msg_.endMessage(sim_time_);
                        return false;
                    }

                    simfile_ = *av++;
                    ac--;
                }
                else if (arg == "--props")
                {
                    if (ac == 0)
                    {
                        msg_.startMessage(SimulatorMessages::MessageType::Error);
                        msg_ << "error: command line option '--props' requires an additional argument";
                        msg_.endMessage(sim_time_);
                        return false;
                    }

                    propfile_ = *av++;
                    ac--;
                }
                else if (arg == "--debug")
                {
                    size_t index;

                    if (ac == 0)
                    {
                        msg_.startMessage(SimulatorMessages::MessageType::Error);
                        msg_ << "error: command line option '--debug' requires an additional argument";
                        msg_.endMessage(sim_time_);
                        return false;
                    }

                    std::string vstr = *av++;
                    ac--;
                    int v;
                    bool error = false;
                    try
                    {
                        v = std::stoi(vstr, &index);
                    }
                    catch(const std::exception& e)
                    {
                        error = true;
                    }

                    if (error || index != vstr.length())
                    {
                        msg_.startMessage(SimulatorMessages::MessageType::Error);
                        msg_ << "error: command line option '--debug' requires an additional integer argument, argument was not an integer";
                        msg_.endMessage(sim_time_);
                        return false;                        
                    }

                    msg_.setDebugLevel(v);
                }
                else
                {
                    msg_.startMessage(SimulatorMessages::MessageType::Error);
                    msg_ << "error: unknown command line option '" << arg << "'";
                    msg_.endMessage(sim_time_);
                    return false;                       
                }
                
            }

            return true;
        }

        SimulatorEngine::ErrorCode SimulatorEngine::start(int ac, char **av)
        {
            int32_t status = 0 ;

            if (!parseCommandLineArgs(ac, av))
                return ErrorCode::BadCommandLine ;

            // Initialize the HAL so our time base is setup
            HAL_Initialize(500, 0);            

            // Print information about models
            msg_.startMessage(SimulatorMessages::MessageType::Debug, 1);
            msg_ << "Models Defined: ";
            for(const std::string &model : ModelFactoryBase::getModelList())
                msg_ << " " << model;
            msg_.endMessage(sim_time_);

            // Read the properties file
            if (!props_->loadProperties(propfile_))
                return ErrorCode::PropertyFileError;

            // Read the event file
            if (!events_->loadEvents(simfile_))
                return ErrorCode::EventFileError;

            // Register to proces interesting hal events
            if (!registerForHALEvents())
                return ErrorCode::HALError;

            sim_time_ = HAL_GetFPGATime(&status) ;
            if (status != 0)
                return ErrorCode::HALError ;

            msg_.startMessage(SimulatorMessages::MessageType::Debug, 1);
            msg_ << "SimulatorEngine: start ";
            msg_ << " StarTime " << sim_time_ ;
            msg_.endMessage(sim_time_);                

            return ErrorCode::NoError;
        }

        bool SimulatorEngine::registerForHALEvents()
        {
            return true;
        }

        void SimulatorEngine::runSim()
        {
            int32_t status  = 0 ;

            uint64_t haltime = HAL_GetFPGATime(&status) ;
            assert(status == 0) ;

            while (sim_time_ < haltime)
            {
                runEvents() ;
                runModels() ;

                sim_time_ += sim_time_step_ ;
            }
        }

        SimulatorEngine::ErrorCode SimulatorEngine::end()
        {
            return ErrorCode::NoError;
        }

        void SimulatorEngine::runEvents()
        {
        }

        void SimulatorEngine::runModels()
        {
            for(auto model : models_)
                model->run();
        }

        void SimulatorEngine::setEncoder(int indexA, int indexB, int32_t value)
        {
            // HALSIM_SetEncoderCount(index, value) ;
        }

        double SimulatorEngine::getPWM(int index)
        {
            return HALSIM_GetPWMPosition(index) ;
        }
    }
}
