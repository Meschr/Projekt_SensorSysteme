using System;
using System.ComponentModel;
using System.Diagnostics;
using Caliburn.Micro;
using LadaPlotter.Resources.Logging;
using LadaPlotter.UI.TabElements.DataTab;
using LadaPlotter.UI.TabElements.DeviceCommunicationTab;

namespace LadaPlotter.UI.Shell;

public class ShellViewModel : PropertyChangedBase
{
    private static readonly ILogger Logger = Logger<ShellViewModel>.Create();
    private readonly IEventAggregator _eventAggregator = new EventAggregator();

    private DeviceCommunicationTabViewModel _deviceCommunicationTabViewModel;
    private DataTabViewModel _localDataTabViewModel;

    public ShellViewModel()
    {
        _deviceCommunicationTabViewModel = new DeviceCommunicationTabViewModel(_eventAggregator);
        _localDataTabViewModel = new DataTabViewModel(_eventAggregator);
    }

    public DeviceCommunicationTabViewModel DeviceCommunicationTabViewModel
    {
        get => _deviceCommunicationTabViewModel;
        set
        {
            _deviceCommunicationTabViewModel = value;
            NotifyOfPropertyChange();
        }
    }

    public DataTabViewModel LocalDataTabViewModel
    {
        get => _localDataTabViewModel;
        set
        {
            _localDataTabViewModel = value;
            NotifyOfPropertyChange();
        }
    }

    public DeviceCommunicationTabViewModel TestTabViewModel
    {
        get => _deviceCommunicationTabViewModel;
        set
        {
            _deviceCommunicationTabViewModel = value;
            NotifyOfPropertyChange();
        }
    }

    public void LogoIcon()
    {
        var targetWebsite = "https://www.youtube.com/watch?v=VU2d_Pld3w8";

        try
        {
            Process.Start(targetWebsite);
        }
        catch (Win32Exception noBrowser)
        {
            Logger.Error(noBrowser, noBrowser.Message);
        }
        catch (Exception other)
        {
            Logger.Error(other, other.Message);
        }
    }
}