using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PathViewer
{
    public class LoggerOutputEventArgs : EventArgs
    {
        public readonly string MessageText;
        public readonly Logger.MessageType MessageType;

        public LoggerOutputEventArgs(Logger.MessageType mtype, string msg)
        {
            MessageType = mtype;
            MessageText = msg;
        }
    }
}
