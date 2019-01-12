#include "Encoder.h"
#include "RobotSimBase.h"

using namespace xero::sim;

namespace frc
{
Encoder::Encoder(int first, int second)
{
    first_ = first;
    second_ = second;
    value_ = 0;

    RobotSimBase &sim = RobotSimBase::getRobotSimulator();
    sim.connect(this);
}

Encoder::~Encoder()
{
    RobotSimBase &sim = RobotSimBase::getRobotSimulator();
    sim.disconnect(this);
}

int Encoder::Get()
{
    std::lock_guard<std::mutex> lock(getLockMutex());
    return value_;
}

void Encoder::SimulatorSetValue(int value) {
    std::lock_guard<std::mutex> lock(getLockMutex());
    if (reverse_)
        value_ = -value ;
    else
        value_ = value ;
}

void Encoder::Reset()
{
    value_ = 0;
    changed();
}
} // namespace frc
