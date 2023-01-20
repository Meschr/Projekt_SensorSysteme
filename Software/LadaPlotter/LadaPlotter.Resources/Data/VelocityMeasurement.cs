namespace LadaPlotter.Resources.Data
{
    public class VelocityMeasurement : IMeasurement
    {
        public VelocityMeasurement(double[] values, int samplingRate)
        {
            Values = values;
            Name = "Velocity";
            SamplingRate = samplingRate;
            Unit = "m/s";
            Plotable = true;
        }
        public double[] Values { get; set; }
        public string Name { get; }
        public int SamplingRate { get; }
        public bool Plotable { get; set; }
        public string Unit { get; }
    }
}
