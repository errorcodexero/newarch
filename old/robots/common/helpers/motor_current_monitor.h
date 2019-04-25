#pragma once

#include <vector>
#include <list>
#include <limits>


///
/// \class MotorCurrentMonitor
///
/// \brief Monitor the current flowing through one or more motors operated in lockstep, and try to detect if any motor is potentially bad.
///
/// Create one object of this class for every motor you want to monitor.  If you have multiple motors operating in lockstep, such as
/// the 3 motors driving the right side of the drive base, create one instanceobject for that set of 3 motors.
/// Create a separate objectfor the other set of 3 motors driving the left side of the drive base.
///
/// If only monitoring one motor, we check if the current exceeds a threshold.
///
/// If monitoring a group of motors, all of which must be operated in lockstep, then we have the option
/// to monitor the max current, but additionally check the current consumption relative to each other.
/// If the relative current variates by some threshold, a violation is issued.
///
/// Because of instantaneous variations in current, what is compared is a trailing average of the last "N" measurements.
/// Pass in set of motor currents at a periodic interval.  Checking violations only occurs once we have accumulated at least "N" sets of measurements.
///

class MotorCurrentMonitor {
public:
    /// \brief One instantaneous measurement set for all motors operating in lockstep. 
    typedef std::vector<double> Measurement;

private:
    /// \brief Sentinel to indicate that a variable of type "unsigned int" is unspecified.
    const unsigned int UNSPECIFIED = std::numeric_limits<unsigned int>::max();

    /// \brief Trailing list of last "N" sets of measurements.
    typedef std::list<Measurement> Measurements;

public:
    /// \brief Constructor must specify the number of motors in that group of lockstep motors.
    MotorCurrentMonitor(unsigned int n_motors);
    
    /// \brief Specify the max current allowed through a motor. This is checked against the trailing average.
    ///
    /// Specifying the max current is optional for monitoring more than 1 motor since we can
    /// also compare them to each other, but when there's only 1 motor in a group, the max current
    /// is mandatory because this is the only thing we can check for.
    void setMaxCurrent(double max_current);
    
    /// \brief Specify the number of measurements we want to average.  The trailing average is what is compared, not the last instantaneous measurement.
    void setMeasurementsToAverage(unsigned int n_measurements);

    /// \brief For a group of 2 or more motors, how much is one motor allowed to vary from the average of the motors.  Set e.g. to 0.20 to allow a 20% variance.
    void setVarianceThreshold(double variance_threshold);
    
    /// \brief Pass in 1 set of instantaneous measurements for the group of motors.
    void logNewMeasurement(const Measurement& motor_currents);
    
    /// \brief Check if any violations (potentially bad motors) detected.  Return true if any found + register in the message logger.
    bool checkViolation() const;

    private:
    unsigned int n_motors_;
    unsigned int n_measurements_to_average_;
    double variance_threshold_;
    double max_current_;
    Measurement total_per_motor_;
    Measurements measurements_;
    
    /// \brief Validation of settings (not motor currents), performed when we log new data. It is assumed that settings like the max current allowed have been specified at that point.
    void validate() const;
};
