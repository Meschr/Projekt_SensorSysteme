using System.Collections.ObjectModel;

namespace LadaPlotter.Resources.Data
{
    public class LogData
    {
        public LogData()
        {
            Processed = false;
            TimeStamp = DateTime.Now; //todo später auslesen aus logfile
        }

        public bool Processed { get; set; }

        public DateTime TimeStamp { get; }

        public string Name { get; set; }

        public ObservableCollection<IMeasurement> Measurements { get; } = new ObservableCollection<IMeasurement>();

        public void AddMeasurement(IMeasurement measurement)
        {
            Measurements.Add(measurement);
        }
        public bool Any()
        {
            return Measurements.Any();
        }
    }
}