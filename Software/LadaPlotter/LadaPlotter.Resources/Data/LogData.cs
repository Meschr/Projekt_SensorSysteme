using System.Collections.ObjectModel;
using System.Runtime.InteropServices.ComTypes;

namespace LadaPlotter.Resources.Data
{
    public class LogData
    {
        private string _name;

        public LogData(string name)
        {
            _name = name;
            Processed = false;
            TimeStamp = DateTime.Now; //todo später auslesen aus logfile
        }

        public bool Processed { get; set; }

        public DateTime TimeStamp { get; }

        public string Name
        {
            get => _name;
            set => _name = value;
        }

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