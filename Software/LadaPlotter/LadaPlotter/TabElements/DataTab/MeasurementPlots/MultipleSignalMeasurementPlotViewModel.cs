using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using LadaPlotter.Resources.Data;
using ScottPlot;

namespace LadaPlotter.UI.TabElements.DataTab.MeasurementPlots
{
    public class MultipleSignalMeasurementPlotViewModel : IMeasurementPlotViewModel
    {
        private readonly IEnumerable<IMeasurement> _measurements;

        private WpfPlot _plot;

        public MultipleSignalMeasurementPlotViewModel(IEnumerable<IMeasurement> measurements)
        {
            _measurements = measurements;
            Plot = new WpfPlot();

            if (measurements != null)
                InitPlotWithValues();
        }

        public WpfPlot Plot
        {
            get => _plot;
            set
            {
                _plot = value;
                InitPlotWithoutValues();
            }
        }

        private void InitPlotWithoutValues()
        {
            var rand = new Random(0);
            var values = DataGen.RandomWalk(rand, 100_000);
            var sampleRate = 20_000;

            // Signal plots require a data array and a sample rate (points per unit)
            _plot.Plot.Style(Style.Gray1);
            _plot.Height = 400;
            _plot.Plot.AddSignal(values, sampleRate, Color.LawnGreen);
            _plot.Plot.Title("Empty Plot: " + values.Length + " Sample Points");
            _plot.Render();
        }

        public void InitPlotWithValues()
        {
            _plot.Plot.Clear();
            _plot.Plot.Title("Acceleration in all Axis");

            foreach (var measurement in _measurements)
            {
                _plot.Plot.AddSignal(measurement.Values, measurement.SamplingRate, label: measurement.Name);
            }

            _plot.Plot.YLabel(_measurements.First().Name + " in " + _measurements.First().Unit);
            _plot.Render();
        }
    }
}