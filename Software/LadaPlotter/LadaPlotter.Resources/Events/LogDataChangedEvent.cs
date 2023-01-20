namespace LadaPlotter.Resources.Events
{
    public class LogDataChangedEvent
    {
        public LogDataChangedEvent(string logDataName, string filepath)
        {
            LogDataName = logDataName;
            Filepath = filepath;
        }

        public string Filepath { get; }
        public string LogDataName { get; }
    }
}
