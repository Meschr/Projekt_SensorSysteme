using System.Windows;
using System.Windows.Controls;

namespace LadaPlotter.UI.TabElements.DataTab.MeasurementPlots
{
    /// <summary>
    /// Interaktionslogik für SignalMeasurementPlotView.xaml
    /// </summary>
    public partial class MultipleSignalMeasurementPlotView : UserControl
    {
        public MultipleSignalMeasurementPlotView()
        {
            InitializeComponent();
            DataContextChanged += DataPlotView_OnDataContextChanged;
        }

        private void DataPlotView_OnDataContextChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            if (e.NewValue is MultipleSignalMeasurementPlotViewModel multipleSignalMeasurementPlotViewModel)
            {
                PlotGrid.Children.Clear();

                PlotGrid.Children.Add(multipleSignalMeasurementPlotViewModel.Plot);

                multipleSignalMeasurementPlotViewModel.Plot.Render();
            }
        }
    }
}
