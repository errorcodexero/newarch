#include <CTREManager.h>
#include <SimulatorMessages.h>

namespace xero
{
    namespace sim2
    {
        CTREManager::CTREManager(SimulatorEngine &engine) : HardwareManager(engine)
        {            
        }

        CTREManager::~CTREManager()
        {

        }

        bool CTREManager::create(int index)
        {
            std::lock_guard<std::mutex> lock(lock_);
            auto &msg = getEngine().getMessageOutput();

            auto it = status_.find(index);
            if (it != status_.end())
            {
                msg.startMessage(xero::sim2::SimulatorMessages::MessageType::Warning);
                msg << "failed to create duplicate CTRE motor controller, index " << index ;
                msg.endMessage(getEngine().getSimulationTime());
                return false;
            }

            CTREStatus st;
            st.power_ = 0.0;
            status_[index] = st;

            msg.startMessage(xero::sim2::SimulatorMessages::MessageType::Debug, 5);
            msg << "created CTRE motor controller, index " << index ;
            msg.endMessage(getEngine().getSimulationTime());

            return true;
        }

        bool CTREManager::destroy(int index)
        {
            std::lock_guard<std::mutex> lock(lock_);
            auto &msg = getEngine().getMessageOutput();

            auto it = status_.find(index);
            if (it != status_.end())
            {
                msg.startMessage(xero::sim2::SimulatorMessages::MessageType::Warning);
                msg << "failed to destroy CTRE motor controller - does not exist, index " << index ;
                msg.endMessage(getEngine().getSimulationTime());                
                return false;
            }

            msg.startMessage(xero::sim2::SimulatorMessages::MessageType::Debug, 5);
            msg << "destroyed CTRE motor controller, index " << index ;
            msg.endMessage(getEngine().getSimulationTime());                

            status_.erase(it);
            return true;
        }

        bool CTREManager::set(int index, double value)
        {
            std::lock_guard<std::mutex> lock(lock_);
            auto &msg = getEngine().getMessageOutput();

            auto it = status_.find(index);
            if (it != status_.end())
            {
                msg.startMessage(xero::sim2::SimulatorMessages::MessageType::Warning);
                msg << "failed to set CTRE motor controller - does not exist, index " << index ;
                msg.endMessage(getEngine().getSimulationTime());                 
                return false;
            }

            msg.startMessage(xero::sim2::SimulatorMessages::MessageType::Debug, 9);
            msg << "set CTRE motor controller, index " << index << " value " << value ;
            msg.endMessage(getEngine().getSimulationTime());               

            status_[index].power_ = value;
            return true;
        }

        bool CTREManager::get(int index, double &value)
        {
            std::lock_guard<std::mutex> lock(lock_);
            auto &msg = getEngine().getMessageOutput();

            auto it = status_.find(index);
            if (it != status_.end())
            {
                msg.startMessage(xero::sim2::SimulatorMessages::MessageType::Warning);
                msg << "failed to get CTRE motor controller - does not exist, index " << index ;
                msg.endMessage(getEngine().getSimulationTime());
                return false;
            }

            value = status_[index].power_;

            msg.startMessage(xero::sim2::SimulatorMessages::MessageType::Debug, 9);
            msg << "get CTRE motor controller, index " << index << " value " << value ;
            msg.endMessage(getEngine().getSimulationTime());   

            return true;
        }        
    }
}