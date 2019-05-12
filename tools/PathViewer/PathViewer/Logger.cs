using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PathViewer
{
    class Logger
    {
        #region private member variables
        #endregion

        #region public enums
        public enum MessageType
        {
            Info,
            Debug,
            Warning,
            Error,
        };
        #endregion

        #region public constructors
        public Logger()
        {

        }
        #endregion

        #region public properties
        #endregion

        #region public methods
        public void LogMessage(MessageType mtype, string msg)
        {
        }
        #endregion

    }
}
