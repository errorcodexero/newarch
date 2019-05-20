using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Diagnostics;
using PathViewer;
using XeroMath;

namespace PathFinderV1
{
    public class PathFinderV1Gen : PathGenerator
    {
        #region public constructor
        public PathFinderV1Gen()
        {
        }
        #endregion

        #region public properties
        public override string Name
        {
            get { return "PathFinder Version 1"; }
        }

        public override Version Version
        {
            get { return new Version(1, 0); }
        }

        public override bool TimingConstraintsSupported
        {
            get { return false ; }
        }
        #endregion

        #region public methods
        public override void GenerateSplines(RobotPath path, List<Spline> xsplines, List<Spline> ysplines)
        {
            for (int i = 0; i < path.Points.Length - 1; i++)
            {
                Spline x, y;
                GenerateSpline(path.Points[i], path.Points[i + 1], out x, out y);

                xsplines.Add(x);
                ysplines.Add(y);
            }
        }

        public override PathSegment[] GenerateDetailedPath(RobotParams robot, RobotPath path)
        {
            StringBuilder stdout;
            StringBuilder stderr;
            string outfile = Path.GetTempFileName();
            string pathfile = Path.GetTempFileName();

            stdout = new StringBuilder();
            stderr = new StringBuilder();

            string dir = AppDomain.CurrentDomain.BaseDirectory;
            string execpath = Path.Combine(dir, "PathFinderV1Gen.exe");

            GeneratePathFile(robot, path, pathfile);

            string args = string.Empty;
            if (pathfile == string.Empty)
                return null;

            args += "--outfile " + outfile;
            args += " --pathfile " + pathfile;
            args += " --timestep " + robot.TimeStep.ToString();

            ProcessStartInfo info = new ProcessStartInfo();
            info.CreateNoWindow = true;
            info.RedirectStandardError = true;
            info.RedirectStandardOutput = true;
            info.UseShellExecute = false;
            info.Arguments = args;
            info.FileName = execpath;

            Process proc = new Process();
            proc.StartInfo = info;
            proc.EnableRaisingEvents = true;
            proc.OutputDataReceived += new DataReceivedEventHandler(delegate (object sender, DataReceivedEventArgs e)
            {
                stdout.Append(e.Data);
            }
            );
            proc.ErrorDataReceived += new DataReceivedEventHandler(delegate (object sender, DataReceivedEventArgs e)
            {
                stderr.Append(e.Data);
            }
            );
            proc.Start();
            proc.BeginErrorReadLine();
            proc.BeginOutputReadLine();
            proc.WaitForExit();
            proc.CancelErrorRead();
            proc.CancelOutputRead();

            string[] headers = { "time", "x", "y", "heading", "curvature", "dscurvature", "position", "velocity", "acceleration" };

            PathSegment[] seg = ParseOutputFile(outfile, headers);
            File.Delete(outfile);
            File.Delete(pathfile);

            return seg;
        }
        #endregion

        #region private methods


        private void GenerateSpline(WayPoint p0, WayPoint p1, out Spline xsp, out Spline ysp)
        {
            double dist = p0.Distance(p1);
            double aoffset = Math.Atan2(p1.Y - p0.Y, p1.X - p0.X);

            double a0_delta = Math.Tan(XeroUtils.BoundRadians(XeroUtils.DegreesToRadians(p0.Heading) - aoffset));
            double a1_delta = Math.Tan(XeroUtils.BoundRadians(XeroUtils.DegreesToRadians(p1.Heading) - aoffset));

            double[] coeffs = new double[6];
            coeffs[5] = -(3 * (a0_delta + a1_delta)) / (dist * dist * dist * dist);
            coeffs[4] = (8 * a0_delta + 7 * a1_delta) / (dist * dist * dist);
            coeffs[3] = -(6 * a0_delta + 4 * a1_delta) / (dist * dist);
            coeffs[2] = 0.0;
            coeffs[1] = a0_delta;
            coeffs[0] = 0.0;

            xsp = new Spline(dist, coeffs);
            xsp.XOffset = p0.X;
            xsp.YOffset = p0.Y;
            xsp.AngleOffset = XeroUtils.RadiansToDegrees(aoffset);
            ysp = null;
        }
        #endregion
    }
}
