using log4net;
using log4net.Config;

namespace LadaPlotter.Resources.Logging
{
    public class Logger<T> : ILogger
    {
        private static bool _isConfigured;
        private readonly ILog _logger;

        public Logger()
        {
            if (!_isConfigured)
            {
                var logFilePath = Path.Combine(AppDomain.CurrentDomain.BaseDirectory);
                XmlConfigurator.Configure(new FileInfo(logFilePath + @"Logging\log4net.config"));
                _isConfigured = true;
            }

            _logger = LogManager.GetLogger(typeof(T).FullName);
        }

        public static ILogger Create()
        {
            return new Logger<T>();
        }

        public void Debug(string message)
        {
            _logger.Debug(message);
        }

        public void Info(string message)
        {
            _logger.Info(message);
        }

        public void Warning(string message)
        {
            _logger.Warn(message);
        }

        public void Error(string message)
        {
            _logger.Error(message);
        }

        public void Fatal(string message)
        {
            _logger.Fatal(message);
        }

        public void Debug(Exception ex, string message)
        {
            _logger.Debug(message, ex);
        }

        public void Info(Exception ex, string message)
        {
            _logger.Info(message, ex);
        }

        public void Warning(Exception ex, string message)
        {
            _logger.Warn(message, ex);
        }

        public void Error(Exception ex, string message)
        {
            _logger.Error(message, ex);
        }

        public void Fatal(Exception ex, string message)
        {
            _logger.Fatal(message, ex);
        }

    }
}