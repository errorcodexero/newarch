using System;
using System.Windows.Forms;
using System.Threading;

namespace PathViewer
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Thread.CurrentThread.Name = "MainThread";

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            try
            {
                Application.Run(new PathViewerForm());
            }
            catch
            {

            }
        }
    }
}
