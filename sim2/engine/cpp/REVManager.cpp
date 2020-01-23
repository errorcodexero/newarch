#include <REVManager.h>
#include <SimulatorMessages.h>

namespace xero
{
    namespace sim2
    {
        REVManager::REVManager(SimulatorEngine &engine) : HardwareManager(engine)
        {            
        }

        REVManager::~REVManager()
        {

        }

        bool REVManager::create(int index)
        {
            std::lock_guard<std::mutex> lock(lock_);
            auto &msg = getEngine().getMessageOutput();

            auto it = status_.find(index);
            if (it != status_.end())
            {
                msg.startMessage(xero::sim2::SimulatorMessages::MessageType::Warning);
                msg << "failed to create duplicate REV motor controller, index " << index ;
                msg.endMessage(getEngine().getSimulationTime());
                return false;
            }

            REVStatus st;
            st.power_ = 0.0;
            status_[index] = st;

            msg.startMessage(xero::sim2::SimulatorMessages::MessageType::Debug, 5);
            msg << "created REV motor controller, index " << index ;
            msg.endMessage(getEngine().getSimulationTime());

            return true;
        }

        bool REVManager::destroy(int index)
        {
            std::lock_guard<std::mutex> lock(lock_);
            auto &msg = getEngine().getMessageOutput();

            auto it = status_.find(index);
            if (it != status_.end())
            {
                msg.startMessage(xero::sim2::SimulatorMessages::MessageType::Warning);
                msg << "failed to destroy REV motor controller - does not exist, index " << index ;
                msg.endMessage(getEngine().getSimulationTime());                
                return false;
            }

            msg.startMessage(xero::sim2::SimulatorMessages::MessageType::Debug, 5);
            msg << "destroyed REV motor controller, index " << index ;
            msg.endMessage(getEngine().getSimulationTime());                

            status_.erase(it);
            return true;
        }

        bool REVManager::set(int index, double value)
        {
            std::lock_guard<std::mutex> lock(lock_);
            auto &msg = getEngine().getMessageOutput();

            auto it = status_.find(index);
            if (it != status_.end())
            {
                msg.startMessage(xero::sim2::SimulatorMessages::MessageType::Warning);
                msg << "failed to set REV motor controller - does not exist, index " << index ;
                msg.endMessage(getEngine().getSimulationTime());                 
                return false;
            }

            msg.startMessage(xero::sim2::SimulatorMessages::MessageType::Debug, 9);
            msg << "set REV motor controller, index " << index << " value " << value ;
            msg.endMessage(getEngine().getSimulationTime());               

            status_[index].power_ = value;
            return true;
        }

        bool REVManager::get(int index, double &value)
        {
            std::lock_guard<std::mutex> lock(lock_);
            auto &msg = getEngine().getMessageOutput();

            auto it = status_.find(index);
            if (it != status_.end())
            {
                msg.startMessage(xero::sim2::SimulatorMessages::MessageType::Warning);
                msg << "failed to get REV motor controller - does not exist, index " << index ;
                msg.endMessage(getEngine().getSimulationTime());
                return false;
            }

            value = status_[index].power_;

            msg.startMessage(xero::sim2::SimulatorMessages::MessageType::Debug, 9);
            msg << "get REV motor controller, index " << index << " value " << value ;
            msg.endMessage(getEngine().getSimulationTime());   

            return true;
        }    


        bool REVManager::setEncoder(int index, double value)
        {
            std::lock_guard<std::mutex> lock(lock_);
            auto &msg = getEngine().getMessageOutput();

            auto it = status_.find(index);
            if (it != status_.end())
            {
                msg.startMessage(xero::sim2::SimulatorMessages::MessageType::Warning);
                msg << "failed to set REV motor controller - does not exist, index " << index ;
                msg.endMessage(getEngine().getSimulationTime());                 
                return false;
            }

            msg.startMessage(xero::sim2::SimulatorMessages::MessageType::Debug, 9);
            msg << "set REV motor controller, index " << index << " value " << value ;
            msg.endMessage(getEngine().getSimulationTime());               

            status_[index].encoder_ = value;
            return true;
        }

        bool REVManager::getEncoder(int index, double &value)
        {
            std::lock_guard<std::mutex> lock(lock_);
            auto &msg = getEngine().getMessageOutput();

            auto it = status_.find(index);
            if (it != status_.end())
            {
                msg.startMessage(xero::sim2::SimulatorMessages::MessageType::Warning);
                msg << "failed to get REV motor controller - does not exist, index " << index ;
                msg.endMessage(getEngine().getSimulationTime());
                return false;
            }

            value = status_[index].encoder_;

            msg.startMessage(xero::sim2::SimulatorMessages::MessageType::Debug, 9);
            msg << "get REV motor controller, index " << index << " value " << value ;
            msg.endMessage(getEngine().getSimulationTime());   

            return true;
        }               
    }
}