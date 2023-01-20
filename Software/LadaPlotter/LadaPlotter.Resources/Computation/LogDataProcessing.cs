using System.Collections.ObjectModel;
using LadaPlotter.Resources.Data;

namespace LadaPlotter.Resources.Computation
{
    public class LogDataProcessing
    {
        public void ProcessLogData(LogData logData)
        {
            if(logData.Processed)
                return;

            var measurements = new ObservableCollection<IMeasurement>(logData.Measurements);
            foreach (var measurement in measurements)
            {
                if (measurement is PositionMeasurement positionMeasurement)
                {
                    var velocityMeasurement = new VelocityMeasurement(ComputeVelocity(positionMeasurement), 1000); //todo refactor sampling rate handling
                    logData.AddMeasurement(velocityMeasurement);
                }
            }

            logData.Processed = true;
        }

        private double [] ComputeVelocity(PositionMeasurement positionMeasurement)
        {
            //1.Schritt Eckdeteck 
            //2.Spline
            var positionValues = positionMeasurement.Values;
            var velocityValues = new double[positionValues.Length];

            for (var i = 0; i < positionValues.Length; i++)
            {
                velocityValues[i] = positionValues[i] / 2;
            }

            return velocityValues;
        }
    }
}