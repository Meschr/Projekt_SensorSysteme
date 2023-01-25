using Caliburn.Micro;

namespace LadaPlotter.UI.TabElements.DataTab.DataTabControl;

public class DataTabItemViewModel : PropertyChangedBase
{
    private readonly IEventAggregator _eventAggregator;
    private string _tabItemName;

    public DataTabItemViewModel(IEventAggregator eventAggregator, string name)
    {
        _eventAggregator = eventAggregator;
        _tabItemName = name;
        //display the empty plots when no LogData is loaded
        DataTabContent = new LogDataPlotViewModel(_eventAggregator);
  }

    public LogDataPlotViewModel DataTabContent { get; set; }
        
    public string TabItemName
    {
        get => _tabItemName;
        set
        {
            _tabItemName = value;
            NotifyOfPropertyChange(() => _tabItemName);
        }
    }
}