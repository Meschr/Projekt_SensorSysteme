namespace LadaPlotter.Resources.Data
{
    public class AccelerationMeasurement : IMeasurement
    {
        public AccelerationMeasurement(double[] values, int samplingRate)
        {
            Values = values;
            Name = "Acceleration";
            SamplingRate = samplingRate;
            Unit = "g";
            Plotable = true;
        }

        public double[] Values { get; set; }
        public string Name { get; }
        public int SamplingRate { get; }
        public bool Plotable { get; set; }
        public string Unit { get; }
    }
}