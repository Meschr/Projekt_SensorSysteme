using System;
using System.Drawing;
using LadaPlotter.Resources.Data;
using ScottPlot;

namespace LadaPlotter.UI.TabElements.DataTab.MeasurementPlots
{
    public class SignalMeasurementPlotViewModel : IMeasurementPlotViewModel
    {
        private IMeasurement _measurement;

        private WpfPlot _plot;

        public SignalMeasurementPlotViewModel(IMeasurement measurement)
        {
            _measurement = measurement;
            Plot = new WpfPlot();

            if (measurement != null)
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
            _plot.Plot.AddSignal(_measurement.Values, _measurement.SamplingRate, Color.LawnGreen);
            _plot.Plot.Title(_measurement.Name + " " + _measurement.Values.Length + " Data Points");
            _plot.Plot.YLabel(_measurement.Name + " in " + _measurement.Unit);
            _plot.Render();
        }
    }
}
