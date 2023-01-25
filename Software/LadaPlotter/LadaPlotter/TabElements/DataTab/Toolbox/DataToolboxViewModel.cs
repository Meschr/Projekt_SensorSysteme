using System.Windows;
using Caliburn.Micro;
using LadaPlotter.Resources.Events;
using MahApps.Metro.Controls;

namespace LadaPlotter.UI.TabElements.DataTab.Toolbox
{
    public class DataToolboxViewModel
    {
        private readonly IEventAggregator _eventAggregator;

        public DataToolboxViewModel(IEventAggregator eventAggregator)
        {
            _eventAggregator = eventAggregator;
        }
        public bool ScrollingEnabled { get; private set; }

        public bool PlotsConnected { get; private set; }

        public void ConnectPlotsToggled(object sender, RoutedEventArgs e)
        {
            var toggleSwitch = sender as ToggleSwitch;
            if (toggleSwitch == null)
                return;


            PlotsConnected = toggleSwitch.IsOn;
        }

        public void EnableScrollToggled(object sender, RoutedEventArgs e)
        {
            var toggleSwitch = sender as ToggleSwitch;
            if (toggleSwitch == null) 
                return;
            _eventAggregator.PublishOnCurrentThreadAsync(new ScrollingStateToggledEvent(toggleSwitch.IsOn));
        }
    }
}
