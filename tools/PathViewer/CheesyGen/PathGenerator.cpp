#include "PathGenerator.h"
#include "QuinticHermiteSpline.h"
#include "SplineGenerator.h"

namespace xero
{
	namespace paths
	{

		PathGenerator::PathGenerator(double maxx, double maxy, double maxtheta)
		{
			maxDx_ = maxx;
			maxDy_ = maxy;
			maxDTheta_ = maxtheta;
		}


		PathGenerator::~PathGenerator()
		{
		}

		Trajectory PathGenerator::generate(const std::vector<Pose2d>& waypoints, bool reverse) const
		{
			std::vector<Pose2d> points = waypoints;
			Pose2d flip = Pose2d(Rotation2d(-1.0, 0.0, false));

			if (reverse)
			{
				points.clear();
				for (const Pose2d& pt : waypoints)
				{
					Pose2d newpt = pt.transformBy(flip);
					points.push_back(newpt);
				}
			}

			std::vector<std::shared_ptr<QuinticHermiteSpline>> splines;
			for (size_t i = 0; i < points.size() - 1; i++)
			{
				auto spline = std::make_shared<QuinticHermiteSpline>(points[i], points[i + 1]);
				splines.push_back(spline);
			}
			QuinticHermiteSpline::optimizeSpline(splines);
			std::vector<Pose2dWithCurvature> path = SplineGenerator::parameterizeSpline(splines, maxDx_, maxDy_, maxDTheta_);
			if (!reverse)
				return Trajectory(path);

			std::vector<Pose2dWithCurvature> flippedpath;
			for (const Pose2dWithCurvature& pt : path)
			{
				Pose2d newpose(pt.getPose().transformBy(flip));
				Pose2dWithCurvature newpt(newpose, -pt.getCurvature(), pt.getDcurvatureDs());
				flippedpath.push_back(newpt);
			}

			return Trajectory(flippedpath);
		}
	}
}
