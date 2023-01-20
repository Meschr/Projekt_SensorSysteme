using ScottPlot;

namespace LadaPlotter.UI.TabElements.DataTab.MeasurementPlots;

public interface IMeasurementPlotViewModel
{
    WpfPlot Plot { get; set; }
}