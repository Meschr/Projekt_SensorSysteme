using ScottPlot;

namespace LadaPlotter.UI.TabElements.DataTab.DataTabControl.MeasurementPlots;

public interface IMeasurementPlotViewModel
{
    WpfPlot Plot { get; set; }
}