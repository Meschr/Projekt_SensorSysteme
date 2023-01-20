namespace LadaPlotter.Resources.Data
{
    public interface IMeasurement
    {
        double[] Values { get; set; }

        string Name { get; }
        int SamplingRate { get; }
        bool Plotable { get; set; }
        string Unit { get; }
    }
}