using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Controls;
using System.Windows.Input;
using Caliburn.Micro;
using LadaPlotter.Resources.Computation;
using LadaPlotter.Resources.Data;
using LadaPlotter.Resources.Events;
using LadaPlotter.UI.TabElements.DataTab.MeasurementPlots;

namespace LadaPlotter.UI.TabElements.DataTab
{
    public class LogDataPlotViewModel : IHandle<ScrollingStateToggledEvent>
    {
        private readonly IEventAggregator _eventAggregator;
        private readonly LogDataProcessing _logDataProcessing = new LogDataProcessing();

        private bool _scrollingEnabled = true;


        public LogDataPlotViewModel(IEventAggregator eventAggregator)
        {
            _eventAggregator = eventAggregator;
            _eventAggregator.Subscribe(this);
            //display the empty plots when no LogData is loaded
            MeasurementPlotViewModels.Add(new SignalMeasurementPlotViewModel(null));
            MeasurementPlotViewModels.Add(new SignalMeasurementPlotViewModel(null));
            MeasurementPlotViewModels.Add(new SignalMeasurementPlotViewModel(null));
        }

        public ObservableCollection<IMeasurementPlotViewModel> MeasurementPlotViewModels { get; } =
            new ObservableCollection<IMeasurementPlotViewModel>();

        public void Handle(ScrollingStateToggledEvent toggleEvent)
        {
            _scrollingEnabled = toggleEvent.ToggleSwitchState;
        }

        public void ScrollViewer_PreviewMouseWheel(object sender, MouseWheelEventArgs e)
        {
            var scrollViewer = (ScrollViewer) sender;
            if (_scrollingEnabled)
            {
                // manually scroll the window then mark the event as handled so it does not zoom
                var scrollOffset = scrollViewer.VerticalOffset - e.Delta * .2;
                scrollViewer.ScrollToVerticalOffset(scrollOffset);
                e.Handled = true;
            }
            else
            {
                // manually scroll (zero offset) to complete the scroll action, then proceed to zooming
                scrollViewer.ScrollToVerticalOffset(scrollViewer.VerticalOffset);
            }
        }

        public void UpdateUi(LogData currentLogData)
        {
            MeasurementPlotViewModels.Clear();
            _logDataProcessing.ProcessLogData(currentLogData);


            var allAxisAcceleration = new List<IMeasurement>();
            foreach (var measurement  in currentLogData.Measurements)
            {
                if (measurement is AccelerationMeasurement)
                    allAxisAcceleration.Add(measurement);
            }   

            if(currentLogData.Any())
                MeasurementPlotViewModels.Add(new MultipleSignalMeasurementPlotViewModel(allAxisAcceleration));

            /*
            foreach (var measurement in currentLogData.Measurements)
                if (measurement is PositionMeasurement positionMeasurement)
                    MeasurementPlotViewModels.Add(new SignalMeasurementPlotViewModel(positionMeasurement));
                else if (measurement is AccelerationMeasurement accelerationMeasurement)
                    MeasurementPlotViewModels.Add(new SignalMeasurementPlotViewModel(accelerationMeasurement));*/
        }

        public Task HandleAsync(ScrollingStateToggledEvent message, CancellationToken cancellationToken)
        {
            throw new System.NotImplementedException();//todo look this up
        }
    }
}