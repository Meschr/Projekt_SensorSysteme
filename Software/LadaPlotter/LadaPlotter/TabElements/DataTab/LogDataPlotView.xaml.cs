using System.Windows;
using System.Windows.Controls;

namespace LadaPlotter.UI.TabElements.DataTab
{
    /// <summary>
    /// Interaction logic for LogDataPlotView.xaml
    /// </summary>
    public partial class LogDataPlotView : UserControl
    {
        public LogDataPlotView()
        {
            InitializeComponent();
            DataContextChanged += DataPlotView_OnDataContextChanged;
        }
        private void DataPlotView_OnDataContextChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            if (e.NewValue is LogDataPlotViewModel logDataPlotViewModel)
            {
                PlotScrollViewer.PreviewMouseWheel += logDataPlotViewModel.ScrollViewer_PreviewMouseWheel;
            }
        }
    }
}
