using System.Windows;
using System.Windows.Controls;

namespace LadaPlotter.UI.TabElements.DataTab.DataTabControl.MeasurementPlots
{
    /// <summary>
    /// Interaktionslogik für SignalMeasurementPlotView.xaml
    /// </summary>
    public partial class SignalMeasurementPlotView : UserControl
    {
        private SignalMeasurementPlotViewModel _vm;

        public SignalMeasurementPlotView()
        {
            InitializeComponent();
            DataContextChanged += DataPlotView_OnDataContextChanged;
            Loaded += OnLoaded;
        }

        private void AddPlotToGrid()
        {
            if (_vm?.Plot != null && !PlotGrid.Children.Contains(_vm.Plot))
                PlotGrid.Children.Add(_vm.Plot);
        }
        private void DataPlotView_OnDataContextChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            _vm = DataContext as SignalMeasurementPlotViewModel;
            
            if (_vm is null || !IsLoaded)
                return;
            
            PlotGrid.Children.Clear();
            AddPlotToGrid();
        }

        private void OnLoaded(object sender, RoutedEventArgs e)
        {
            Loaded -= OnLoaded;
            Unloaded += OnUnloaded;
            DataContextChanged -= DataPlotView_OnDataContextChanged;
            DataContextChanged += DataPlotView_OnDataContextChanged; 
            
            AddPlotToGrid();
        }

        private void OnUnloaded(object sender, RoutedEventArgs e)
        {
            Unloaded -= OnUnloaded;
            Loaded += OnLoaded;
            DataContextChanged -= DataPlotView_OnDataContextChanged; 

            PlotGrid.Children.Clear();
        }
        
        /*
        private void DataPlotView_OnDataContextChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            if (e.NewValue is SignalMeasurementPlotViewModel signalMeasurementPlotViewModel)
            {
                PlotGrid.Children.Clear();

                if (IsLoaded  !PlotGrid.Children.Contains(signalMeasurementPlotViewModel.Plot))
                {
                    PlotGrid.Children.Add(signalMeasurementPlotViewModel.Plot);
                }

                signalMeasurementPlotViewModel.Plot.Render();
            }
        }
        */
    }
}
