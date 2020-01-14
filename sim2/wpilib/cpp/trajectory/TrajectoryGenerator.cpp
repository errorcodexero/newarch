/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "frc/trajectory/TrajectoryGenerator.h"

#include <utility>

#include "frc/spline/SplineHelper.h"
#include "frc/trajectory/TrajectoryParameterizer.h"

using namespace frc;

Trajectory TrajectoryGenerator::GenerateTrajectory(
    Spline<3>::ControlVector initial,
    const std::vector<Translation2d>& interiorWaypoints,
    Spline<3>::ControlVector end, const TrajectoryConfig& config) {
  const Transform2d flip{Translation2d(), Rotation2d(180_deg)};

  // Make theta normal for trajectory generation if path is reversed.
  // Flip the headings.
  if (config.IsReversed()) {
    initial.x[1] *= -1;
    initial.y[1] *= -1;
    end.x[1] *= -1;
    end.y[1] *= -1;
  }

  auto points =
      SplinePointsFromSplines(SplineHelper::CubicSplinesFromControlVectors(
          initial, interiorWaypoints, end));

  // After trajectory generation, flip theta back so it's relative to the
  // field. Also fix curvature.
  if (config.IsReversed()) {
    for (auto& point : points) {
      point = {point.first + flip, -point.second};
    }
  }

  return TrajectoryParameterizer::TimeParameterizeTrajectory(
      points, config.Constraints(), config.StartVelocity(),
      config.EndVelocity(), config.MaxVelocity(), config.MaxAcceleration(),
      config.IsReversed());
}

Trajectory TrajectoryGenerator::GenerateTrajectory(
    const Pose2d& start, const std::vector<Translation2d>& interiorWaypoints,
    const Pose2d& end, const TrajectoryConfig& config) {
  auto [startCV, endCV] = SplineHelper::CubicControlVectorsFromWaypoints(
      start, interiorWaypoints, end);
  return GenerateTrajectory(startCV, interiorWaypoints, endCV, config);
}

Trajectory TrajectoryGenerator::GenerateTrajectory(
    std::vector<Spline<5>::ControlVector> controlVectors,
    const TrajectoryConfig& config) {
  const Transform2d flip{Translation2d(), Rotation2d(180_deg)};
  // Make theta normal for trajectory generation if path is reversed.
  if (config.IsReversed()) {
    for (auto& vector : controlVectors) {
      // Flip the headings.
      vector.x[1] *= -1;
      vector.y[1] *= -1;
    }
  }

  auto points = SplinePointsFromSplines(
      SplineHelper::QuinticSplinesFromControlVectors(controlVectors));

  // After trajectory generation, flip theta back so it's relative to the
  // field. Also fix curvature.
  if (config.IsReversed()) {
    for (auto& point : points) {
      point = {point.first + flip, -point.second};
    }
  }

  return TrajectoryParameterizer::TimeParameterizeTrajectory(
      points, config.Constraints(), config.StartVelocity(),
      config.EndVelocity(), config.MaxVelocity(), config.MaxAcceleration(),
      config.IsReversed());
}

Trajectory TrajectoryGenerator::GenerateTrajectory(
    const std::vector<Pose2d>& waypoints, const TrajectoryConfig& config) {
  return GenerateTrajectory(
      SplineHelper::QuinticControlVectorsFromWaypoints(waypoints), config);
}
