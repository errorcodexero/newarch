using System;

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
