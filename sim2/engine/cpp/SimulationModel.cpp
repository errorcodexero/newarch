#include <SimulationModel.h>
#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableInstance.h>

namespace xero
{
    namespace sim2
    {
        SimulationModel::SimulationModel(SimulatorEngine &engine, const std::string &modelname, const std::string &instname) : engine_(engine)
        {
            model_name_ = modelname;
            inst_name_ = instname ;
        }

        SimulationModel::~SimulationModel()
        {            
        }

        int SimulationModel::getInteger(const std::string &name)
        {
            if (!hasProperty(name))
            {
                SimulatorMessages &msg = engine_.getMessageOutput() ;
                msg.startMessage(SimulatorMessages::MessageType::Error) ;
                msg << "model " << getModelName() << " instance " << getInstanceName() << " - cannot find property" ;
                msg << "'" << name << "'" ;
                msg.endMessage(engine_.getSimulationTime()) ;

                std::runtime_error err("could not create model") ;
                throw err ;
            }

            SimValue v = getProperty(name);
            if (!v.isInteger())
            {
                SimulatorMessages &msg = engine_.getMessageOutput() ;
                msg.startMessage(SimulatorMessages::MessageType::Error) ;
                msg << "model " << getModelName() << " instance " << getInstanceName() << " - property exists but is not an integer" ;
                msg << "'" << name << "'" ;
                msg.endMessage(engine_.getSimulationTime()) ;

                std::runtime_error err("could not create model") ;
                throw err ;                
            }

            return v.getInteger();
        }

        void SimulationModel::dumpProperties()
        {
            for(auto it : props_)
            {
                SimulatorMessages &msg = engine_.getMessageOutput() ;   
                msg.startMessage(SimulatorMessages::MessageType::Debug, 1) ;
                msg << it.first << " = " << it.second.toString() ;
                msg.endMessage(engine_.getSimulationTime()) ;
            }
        }


        void SimulationModel::registerDataFormat(const std::string &format, const std::vector<std::string> &names)
        {
            nt::NetworkTableInstance inst = nt::NetworkTableInstance::GetDefault() ;
            std::string name = "SIM2/" + model_name_ + "/" + inst_name_ ;
            auto plottable = inst.GetTable(name) ;
            plottable->PutStringArray("varnames", names) ;
            plottable->PutString("format", format) ;
        }

        void SimulationModel::presentDataValues(std::vector<double> &values)
        {
            nt::NetworkTableInstance inst = nt::NetworkTableInstance::GetDefault() ;
            std::string tname = "SIM2/" + model_name_ + "/" + inst_name_ ;
            auto plottable = inst.GetTable(tname) ;
            plottable->PutNumberArray("values", values) ;
        }
    }
}
