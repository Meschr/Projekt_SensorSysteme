using Caliburn.Micro;
using LadaPlotter.Resources.Data;

namespace LadaPlotter.UI.TabElements.DataTab.DataTabControl;

public class DataTabItemViewModel : PropertyChangedBase
{
    private readonly IEventAggregator _eventAggregator;
    private string _tabItemName;
    private bool _showBoarder;

    public DataTabItemViewModel(IEventAggregator eventAggregator, string name, LogData logData)
    {
        _eventAggregator = eventAggregator;
        _tabItemName = name;
        //display the empty plots when no LogData is loaded
        DataTabContent = new LogDataPlotViewModel(_eventAggregator);
        DataTabContent.UpdateUi(logData);
    }

    public LogDataPlotViewModel DataTabContent { get; set; }

    public bool ShowBoarder
    {
        get => _showBoarder;
        set => Set(ref _showBoarder, value);
    }

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