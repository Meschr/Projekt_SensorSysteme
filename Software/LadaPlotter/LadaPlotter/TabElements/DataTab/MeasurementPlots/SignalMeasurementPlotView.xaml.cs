using System.Windows;
using System.Windows.Controls;

namespace LadaPlotter.UI.TabElements.DataTab.MeasurementPlots
{
    /// <summary>
    /// Interaktionslogik für SignalMeasurementPlotView.xaml
    /// </summary>
    public partial class SignalMeasurementPlotView : UserControl
    {
        public SignalMeasurementPlotView()
        {
            InitializeComponent();
            DataContextChanged += DataPlotView_OnDataContextChanged;
        }

        private void DataPlotView_OnDataContextChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            if (e.NewValue is SignalMeasurementPlotViewModel signalMeasurementPlotViewModel)
            {
                PlotGrid.Children.Clear();

                PlotGrid.Children.Add(signalMeasurementPlotViewModel.Plot);

                signalMeasurementPlotViewModel.Plot.Render();
            }
        }
    }
}
