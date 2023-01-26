using System.Collections.ObjectModel;
using System.Linq;
using Caliburn.Micro;
using LadaPlotter.Resources.Data;

namespace LadaPlotter.UI.TabElements.DataTab.DataTabControl;

public class AnimatedDataTabControlViewModel
{
    private readonly IEventAggregator _eventAggregator;

    public AnimatedDataTabControlViewModel(IEventAggregator eventAggregator)
    {
        _eventAggregator = eventAggregator;
    }

    public void UpdateUi(LogData currentLogData)
    {
        OpenTabsCollection.Add(new DataTabItemViewModel(_eventAggregator, currentLogData.Name));
        
        if (SelectedTab is null)
            SelectedTab = OpenTabsCollection.First();
    }

    private DataTabItemViewModel _selectedTab;

    public DataTabItemViewModel SelectedTab
    {
        get => _selectedTab;
        set
        {
            if (_selectedTab != null)
                _selectedTab.ShowBoarder = false;

            _selectedTab = value;
            _selectedTab.ShowBoarder = true;
        }
    }

    public ObservableCollection<DataTabItemViewModel> OpenTabsCollection { get; set; } = new();
}