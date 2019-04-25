#include <iostream>
#include <cassert>
#include <numeric>
#include <iomanip>
#include <cmath>

#include "message_dest_stream.h"
#include "message_logger.h"
#include "motor_current_monitor.h"


MotorCurrentMonitor::MotorCurrentMonitor(unsigned int n_motors)
{
    assert(n_motors != UNSPECIFIED);
    n_motors_ = n_motors;
    max_current_ = UNSPECIFIED;
    n_measurements_to_average_ = 40;  // Default to 40 == (2 seconds / 50 ms loop)
    variance_threshold_ = 0.20; // Default to 20% variance
    total_per_motor_.resize(n_motors);
    assert(measurements_.empty());
}

void MotorCurrentMonitor::setMaxCurrent(double max_current)
{
    assert(max_current > 0);
    max_current_ = max_current;
}

void MotorCurrentMonitor::setMeasurementsToAverage(unsigned int n_measurements)
{
    assert(n_measurements != UNSPECIFIED);
    n_measurements_to_average_ = n_measurements;
}

void MotorCurrentMonitor::setVarianceThreshold(double variance_threshold)
{
    variance_threshold_ = variance_threshold;
}

void MotorCurrentMonitor::validate() const
{
    if (n_motors_ == 1) {
        assert(max_current_ != UNSPECIFIED);  // If only 1 motor monitored, current threshold must be specified because this is all we can check
    }
}

void MotorCurrentMonitor::logNewMeasurement(const Measurement& motor_currents)
{
    validate();
    assert(n_motors_ == motor_currents.size());

    if (measurements_.size() < n_measurements_to_average_) {    // Still filling up queue of measurements
        for (unsigned int i=0; i<motor_currents.size(); ++i) {
            total_per_motor_[i] += motor_currents[i];
        }
    } else {   // Queue of measurements full. Add latest data, then subtract and remove oldest data points.
        assert(measurements_.size() == n_measurements_to_average_);
        const Measurement& oldest_measurement = measurements_.front();
        for (unsigned int i=0; i<motor_currents.size(); ++i) {
            total_per_motor_[i] = total_per_motor_[i] - oldest_measurement[i] + motor_currents[i];
        }
        measurements_.pop_front();
    }
    measurements_.push_back(motor_currents);
    (void)checkViolation();
}

bool MotorCurrentMonitor::checkViolation() const
{
    bool violation_found(false);
    assert(measurements_.size() <= n_measurements_to_average_);

    // Only check if we have enough data logged
    if (measurements_.size() == n_measurements_to_average_)
    {
        double average = std::accumulate(total_per_motor_.begin(), total_per_motor_.end(), 0.0) / total_per_motor_.size() / measurements_.size();
        if (average != 0) {
            for (unsigned int i=0; i<total_per_motor_.size(); ++i) {
                double val = total_per_motor_[i] / measurements_.size();
                // Check if motor exceeds threshold
                if (max_current_ != UNSPECIFIED) {
                    if (val > max_current_) {
                        violation_found = true;
                        messageLogger& logger = messageLogger::get();
                        logger.startMessage(messageLogger::messageType::warning);
                        logger << "Motor " << static_cast<int>(i) << " may have failed. ";
                        //logger << std::setprecision(2) << std::fixed;
                        logger << "Current = " << val << ". ";
                        logger << "Threshold = " << max_current_ << ".";
                        logger.endMessage();
                    }
                }

                // Check variance between motors
                double deviation = std::fabs(val-average)/average;
                if (deviation > variance_threshold_) {
                    violation_found = true;
                    messageLogger& logger = messageLogger::get();
                    logger.startMessage(messageLogger::messageType::warning);
                    logger << "Motor " << static_cast<int>(i) << " may have failed. ";
                    //logger << std::setprecision(2) << std::fixed;
                    logger << "Current = " << val << ". ";
                    logger << "Average of motors = " << average << ". ";
                    logger << "(" << deviation*100.0 << "% > " << variance_threshold_*100.0 << "% threshold.)";
                    logger.endMessage();
                }
            }
        }
    }
    return violation_found;
}


#ifdef MOTOR_CURRENT_MONITOR_TEST


#include "message_logger.cpp"



int main()
{
    messageLogger& logger = messageLogger::get();
    logger.enableType(messageLogger::messageType::error);
    logger.enableType(messageLogger::messageType::warning);
    logger.enableType(messageLogger::messageType::info);
    logger.enableType(messageLogger::messageType::debug);
    std::shared_ptr<messageDestStream> dest_p = std::make_shared<messageDestStream>(std::cout);
    logger.addDestination(dest_p);
    MotorCurrentMonitor m(2);
    m.setMeasurementsToAverage(2);
        
    logger.startMessage(messageLogger::messageType::info);
    logger << "Logging measurement #1";
    logger.endMessage();
    m.logNewMeasurement( std::vector<double>{10, 10} );
        
    logger.startMessage(messageLogger::messageType::info);
    logger << "Logging measurement #2";
    logger.endMessage();
    m.logNewMeasurement( std::vector<double>{10, 20} );
    
    logger.startMessage(messageLogger::messageType::info);
    logger << "Logging measurement #3";
    logger.endMessage();
    m.logNewMeasurement( std::vector<double>{10, 30} );
    
    logger.startMessage(messageLogger::messageType::info);
    logger << "Logging measurement #4";
    logger.endMessage();
    m.logNewMeasurement( std::vector<double>{10, 90} );

    m.setMaxCurrent(100);
    
    logger.startMessage(messageLogger::messageType::info);
    logger << "Logging measurement #5";
    logger.endMessage();
    m.logNewMeasurement( std::vector<double>{10, 90} );
    
    logger.startMessage(messageLogger::messageType::info);
    logger << "Logging measurement #6";
    logger.endMessage();
    m.logNewMeasurement( std::vector<double>{10, 200} );
    return 0;
}

#endif
