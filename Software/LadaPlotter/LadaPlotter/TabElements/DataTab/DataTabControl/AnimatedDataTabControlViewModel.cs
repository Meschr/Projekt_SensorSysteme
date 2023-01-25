using System.Collections.ObjectModel;
using Caliburn.Micro;

namespace LadaPlotter.UI.TabElements.DataTab.DataTabControl;

public class AnimatedDataTabControlViewModel
{
    private readonly IEventAggregator _eventAggregator;

    public AnimatedDataTabControlViewModel(IEventAggregator eventAggregator)
    {
        _eventAggregator = eventAggregator;
        OpenTabsCollection.Add(new DataTabItemViewModel(_eventAggregator, "Test"));
        OpenTabsCollection.Add(new DataTabItemViewModel(_eventAggregator, "Test"));
    }

    public ObservableCollection<DataTabItemViewModel> OpenTabsCollection { get; set; } = new();
}