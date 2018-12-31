#pragma once

#include "SettingsParser.h"


/// \file

namespace xero {
    namespace misc {

        /// \brief Monitors a stream of values to check if they cease to change beyond a specified threshold
        class StallMonitor {
        public:
            /// \brief Create a stall monitor
            StallMonitor();

            /// \brief Create a stall monitor with the given number of entries and given threshold
            /// \param samples the number of samples to store
            /// \param threshold the threshold within which the monitored value is considered stalled
            StallMonitor(size_t samples, double threshold);

            /// \brief Initialize the stall monitor with the given number of entries and given threshold
            /// \param samples the number of samples to store
            /// \param threshold the threshold within which the monitored value is considered stalled
            void init(size_t samples, double threshold);

            /// \brief Initialize the stall monitor from settings using the given prefix
            ///
            /// Used settings: integer :samples, double :threshold
            /// \param parser the settings parser to read from
            /// \param prefix the prefix to use when reading stall monitor settings
            void initFromSettings(SettingsParser parser, std::string prefix);

            /// \brief Add a sample to the history
            /// \param sample the sample to add to the history
            void addSample(double sample);

            /// \brief Return whether the monitored value has stopped changing
            /// \returns whether the monitored value has stopped changin
            bool isStalled();

            /// \brief Reset the stall monitor's history
            void reset();

        private:
            size_t samples_;
            double threshold_;

            std::list<double> history_;
        };

    }
}
