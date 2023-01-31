using System.Collections.ObjectModel;
using Caliburn.Micro;
using LadaPlotter.UI.TabElements.DataTab;
using LadaPlotter.UI.TabElements.DataTab.DataTabControl;

namespace LadaPlotter.UI.TabElements.DeviceCommunicationTab;

public class DeviceCommunicationTabViewModel
{
    private readonly IEventAggregator _eventAggregator;

    public DeviceCommunicationTabViewModel(IEventAggregator eventAggregator)
    {
        _eventAggregator = eventAggregator;
        //OpenTabsCollection.Add(new DataTabItemViewModel(_eventAggregator,"Test"));
        //OpenTabsCollection.Add(new DataTabItemViewModel(_eventAggregator,"Test"));
    }

    public ObservableCollection<DataTabItemViewModel> OpenTabsCollection { get; set; } = new();
}