using System;
using System.Collections.Generic;
using System.IO;
using System.Diagnostics;
using System.Text;
using PathViewer;
using XeroMath;

namespace CheesyPoofsGenerator
{
    public class CheesyPoofsGen : PathGenerator
    {
        #region public constructor
        public CheesyPoofsGen()
        {
        }
        #endregion

        #region public properties
        public override string Name
        {
            get { return "CheesyPoofs Path Generation"; }
        }

        public override Version Version
        {
            get { return new Version(1, 0); }
        }

        public override bool TimingConstraintsSupported
        {
            get { return true; }
        }
        #endregion

        #region public methods
        public override void GenerateSplines(RobotPath path, List<Spline> xsplines, List<Spline> ysplines)
        {
            for(int i = 0; i < path.Points.Length - 1; i++)
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
            string execpath = Path.Combine(dir, "CheesyGen.exe");

            GeneratePathFile(robot, path, pathfile);

            string args = string.Empty;
            if (pathfile == string.Empty)
                return null;

            args += " --dx " + UnitConverter.Convert(2.0, "inches", robot.Units);
            args += " --dy " + UnitConverter.Convert(0.05, "inches", robot.Units);
            args += " --dtheta 0.1";
            args += " --outfile " + outfile;
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

        private void GenerateSpline(WayPoint p0, WayPoint p1, out Spline xsp, out Spline  ysp)
        {
            double p0h = XeroUtils.DegreesToRadians(p0.Heading);
            double p1h = XeroUtils.DegreesToRadians(p1.Heading);

            double scale = 1.2 * p0.Distance(p1);
            double x0 = p0.X;
            double x1 = p1.X;
            double dx0 = Math.Cos(p0h) * scale;
            double dx1 = Math.Cos(p1h) * scale;
            double ddx0 = 0.0;
            double ddx1 = 0.0;

            double y0 = p0.Y;
            double y1 = p1.Y;
            double dy0 = Math.Sin(p0h) * scale;
            double dy1 = Math.Sin(p1h) * scale;
            double ddy0 = 0.0;
            double ddy1 = 0.0;

            double ax = -6 * x0 - 3 * dx0 - 0.5 * ddx0 + 0.5 * ddx1 - 3 * dx1 + 6 * x1;
            double bx = 15 * x0 + 8 * dx0 + 1.5 * ddx0 - ddx1 + 7 * dx1 - 15 * x1;
            double cx = -10 * x0 - 6 * dx0 - 1.5 * ddx0 + 0.5 * ddx1 - 4 * dx1 + 10 * x1;
            double dx = 0.5 * ddx0;
            double ex = dx0;
            double fx = x0;

            double ay = -6 * y0 - 3 * dy0 - 0.5 * ddy0 + 0.5 * ddy1 - 3 * dy1 + 6 * y1;
            double by = 15 * y0 + 8 * dy0 + 1.5 * ddy0 - ddy1 + 7 * dy1 - 15 * y1;
            double cy = -10 * y0 - 6 * dy0 - 1.5 * ddy0 + 0.5 * ddy1 - 4 * dy1 + 10 * y1;
            double dy = 0.5 * ddy0;
            double ey = dy0;
            double fy = y0;

            double dist = p0.Distance(p1);
            xsp = new Spline(dist, new double[] { fx, ex, dx, cx, bx, ax });
            ysp = new Spline(dist, new double[] { fy, ey, dy, cy, by, ay });
        }
        #endregion
    }
}
