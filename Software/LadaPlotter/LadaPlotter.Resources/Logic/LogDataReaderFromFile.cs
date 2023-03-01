using System.Globalization;
using LadaPlotter.Resources.Data;

namespace LadaPlotter.Resources.Logic
{
    public class LogDataReaderFromFile : ILogDataReader
    {
        public async Task<LogData> Read(string path)
        {
            var logData = new LogData("");
            logData.Name = Path.GetFileNameWithoutExtension(path);
            using (var fileReader = File.OpenText(path))
            {
                var fileText = await fileReader.ReadToEndAsync();
                var stringReader = new StringReader(fileText);
                stringReader.ReadLine();
                var line = stringReader.ReadLine();

                var positionValues = new List<double>();
                var accelerationX = new List<double>();
                var accelerationY = new List<double>();
                var accelerationZ = new List<double>();

                while (line != null)
                {
                    var linesplit = line.Split(new[] {","}, StringSplitOptions.None);
                    uint index = 0;
                    double accelZ = 0;
                    double accelY = 0;
                    double accelX = 0;
                    var marker = false;
                    double pos = 0;

                    
                    if (linesplit.Length >= 6) index = Convert.ToUInt32(linesplit[5]);
                    if (linesplit.Length >= 5) accelZ = Convert.ToDouble(linesplit[4], CultureInfo.InvariantCulture);
                    if (linesplit.Length >= 4) accelY = Convert.ToDouble(linesplit[3], CultureInfo.InvariantCulture);
                    if (linesplit.Length >= 3) accelX = Convert.ToDouble(linesplit[2], CultureInfo.InvariantCulture);
                    if (linesplit.Length >= 2) marker = Convert.ToInt32(linesplit[1]) != 0;
                    if (linesplit.Length >= 1) pos = Convert.ToDouble(linesplit[0], CultureInfo.InvariantCulture);

                    positionValues.Add(pos);
                    accelerationX.Add(accelX);
                    accelerationY.Add(accelY);
                    accelerationZ.Add(accelZ);

                    line = stringReader.ReadLine(); // read next line
                }

                logData.AddMeasurement(new PositionMeasurement(positionValues.ToArray(), 500));
                logData.AddMeasurement(new AccelerationMeasurement(accelerationX.ToArray(), 500));
                logData.AddMeasurement(new AccelerationMeasurement(accelerationY.ToArray(), 500));
                logData.AddMeasurement(new PositionMeasurement(accelerationZ.ToArray(), 500));
            }

            return logData;
        }
    }
}