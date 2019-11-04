#pragma once

#include <string>
#include <vector>
#include <map>

namespace xero
{
    namespace base
    {
        class PlotManager
        {
        public:
            PlotManager() ;
            virtual ~PlotManager() ;

            int initPlot(const std::string &name) ;
            void startPlot(int id, const std::vector<std::string> &cols) ;
            void addPlotData(int id, const std::vector<double> &values) ;
            void endPlot(int id) ;
            void enable(bool b) {
                enabled_ = b ;
            }

        private:
        
            std::string getKeyForPlot(int id) ;

            struct plotinfo
            {
                std::string name_ ;
                int cols_ ;
                size_t index_ ;
            } ;


        private:
            // The ID of the next plot initializaed
            int next_plot_id_ ;

            // The name of the plot network table
            std::string plot_table_ ;

            // The map of plot IDs to active plots
            std::map<int, plotinfo> active_plots_ ;

            // If true, plotting is enabled
            bool enabled_ ;
        } ;
    }
}