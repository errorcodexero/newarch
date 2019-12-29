#include "PhoenixSimulator.h"
#include "LifterModel.h"
#include "WingsModel.h"
#include "GrabberModel.h"
#include "CubeSensorModel.h"
#include "IntakeModel.h"
#include "PhoenixScreenVisualizer.h"
#include <TankDriveModel.h>
#include <cassert>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>

using namespace ctre::phoenix::motorcontrol::can ;
using namespace frc ;
using namespace xero::sim ;

namespace xero
{
    namespace sim
    {
        namespace phoenix
        {
            PhoenixSimulator::PhoenixSimulator(const std::string &paramfile) : RobotSimBase(paramfile)
            {
                tankdrive_ = std::make_shared<TankDriveModel>(*this, TankDriveModel::MotorType::MTTalon) ;
                addModel(tankdrive_) ;

                lifter_ = std::make_shared<LifterModel>(*this) ;
                addModel(lifter_) ;

                wings_ = std::make_shared<WingsModel>(*this) ;
                addModel(wings_) ;

                grabber_ = std::make_shared<GrabberModel>(*this) ;
                addModel(grabber_) ;

                ballsensor_ = std::make_shared<CubeSensorModel>(*this) ;
                addModel(ballsensor_) ;

                intake_ = std::make_shared<IntakeModel>(*this) ;
                addModel(intake_) ;

                oi_ = std::make_shared<OIModel>(*this) ;
                addModel(oi_) ;

                visualizer_ = false ;
            }

            PhoenixSimulator::~PhoenixSimulator() {
            }

            void PhoenixSimulator::updateDriveBase() {
                double lift_height = lifter_->getHeight();
                double grabber_angle = grabber_->getAngle();
                bool cube = ballsensor_->getCubeSensed();

                std::ostringstream strm;

                strm << std::fixed << std::setw(2) << std::setprecision(0);
                strm << lift_height << "," << grabber_angle;
                strm << (cube ? "[#]" : "[_]");
                tankdrive_->setRobotText(strm.str());
            }

            void PhoenixSimulator::enableScreen() {
                visualizer_ = true ;
            }

            void PhoenixSimulator::connect(SimulatedObject *device) {
                if (visualizer_) {
                    auto vis = std::make_shared<PhoenixScreenVisualizer>(*this) ;
                    addVisualizer(vis) ;                    
                    visualizer_ = false ;
                }

                RobotSimBase::connect(device) ;
            }

            void PhoenixSimulator::disconnect(SimulatedObject *device) {
                RobotSimBase::disconnect(device) ;
            }

            size_t PhoenixSimulator::findCubeIndexByPos(double x, double y) {
                for(size_t i = 0 ; i < cubes_.size() ; i++) {
                    double dx = std::fabs(x - cubes_[i].getX()) ;
                    double dy = std::fabs(y - cubes_[i].getY()) ;

                    if (dx < 11 && dy < 11)
                        return i ;
                }

                return std::numeric_limits<size_t>::max() ;
            }           

            bool PhoenixSimulator::isCubeAtPosition(double x, double y) {
                size_t index = findCubeIndexByPos(x, y) ;
                return index != std::numeric_limits<size_t>::max() ;
            }

            void PhoenixSimulator::removeCube(double x, double y) {
                size_t index = findCubeIndexByPos(x, y) ;
                if (index != std::numeric_limits<size_t>::max())
                    cubes_.erase(cubes_.begin() + index) ;
            }
        }
    }
}
