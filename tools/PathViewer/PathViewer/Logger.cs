using System;

namespace PathViewer
{
    public class Logger
    {
        #region private member variables
        #endregion

        #region public events
        public event EventHandler<LoggerOutputEventArgs> OutputAvailable;
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
            OnLoggerOutput(new LoggerOutputEventArgs(mtype, msg));
        }
        #endregion

        virtual protected void OnLoggerOutput(LoggerOutputEventArgs args)
        {
            EventHandler<LoggerOutputEventArgs> handler = OutputAvailable;
            handler?.Invoke(this, args);
        }
    }
}
