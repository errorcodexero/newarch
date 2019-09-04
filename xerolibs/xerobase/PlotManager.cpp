#include "PlotManager.h"
#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableInstance.h>
#include <cassert>

namespace xero {
    namespace base {
        PlotManager::PlotManager()
        {
            plot_table_ = "XeroPlot" ;
            next_plot_id_ = 1 ;
        }

        PlotManager::~PlotManager()
        {
        }

        std::string PlotManager::getKeyForPlot(int id)
        {
            auto it = active_plots_.find(id) ;
            assert(it != active_plots_.end()) ;

            auto &info = active_plots_[id] ;
            std::string key = plot_table_ + "/" + info.name_  ;
            return key ;
        }

        int PlotManager::initPlot(const std::string &name)
        {
            for(auto &pair : active_plots_)
            {
                if (pair.second.name_ == name)
                    return -1 ;
            }

            int id = next_plot_id_++ ;
            plotinfo info ;
            info.name_ = name ;
            active_plots_[id] = info ;

            //
            // This signals software to watch for data at this location, but that the
            // data has not been initialized yet
            //
            nt::NetworkTableInstance inst = nt::NetworkTableInstance::GetDefault() ;
            auto plottable = inst.GetTable(getKeyForPlot(id)) ;
            plottable->PutBoolean("inited", false) ;

            return id ;
        }

        void PlotManager::startPlot(int id, const std::vector<std::string> &cols) {
            auto &info = active_plots_[id] ;
            info.cols_ = cols.size() ;
            info.index_ = 0 ;

            nt::NetworkTableInstance inst = nt::NetworkTableInstance::GetDefault() ;
            auto plottable = inst.GetTable(getKeyForPlot(id)) ;

            plottable->PutStringArray("columns", cols) ;
            plottable->PutBoolean("active", true) ;
            plottable->PutBoolean("inited", true) ;
        }

        void PlotManager::addPlotData(int id, const std::vector<double> &values)
        {
            auto &info = active_plots_[id] ;

            if (values.size() == static_cast<size_t>(info.cols_))
            {
                nt::NetworkTableInstance inst = nt::NetworkTableInstance::GetDefault() ;
                auto plottable = inst.GetTable(getKeyForPlot(id)) ;
                std::string name = "data/" + std::to_string(info.index_) ;
                plottable->PutNumberArray(name, values) ;
                info.index_++ ;
            }
        }        

        void PlotManager::endPlot(int id)
        {
            nt::NetworkTableInstance inst = nt::NetworkTableInstance::GetDefault() ;
            auto plottable = inst.GetTable(getKeyForPlot(id)) ;
            plottable->PutBoolean("active", false) ;
        }
    }
}