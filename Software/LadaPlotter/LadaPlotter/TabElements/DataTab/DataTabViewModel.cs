using System.Collections.Generic;
using System.Threading;
using System.Threading.Tasks;
using Caliburn.Micro;
using LadaPlotter.Resources.Data;
using LadaPlotter.Resources.Events;
using LadaPlotter.Resources.Logic;

namespace LadaPlotter.UI.TabElements.DataTab
{
    public class DataTabViewModel : PropertyChangedBase , IHandle<LogDataChangedEvent>
    {
        private readonly IEventAggregator _eventAggregator;

        private DataListViewModel _dataListViewModel;
        private DataToolboxViewModel _dataToolboxViewModel;
        private LogDataPlotViewModel _dataPlotViewModel;

        private List<LogData> _loadedLogData = new List<LogData>();

        public DataTabViewModel(IEventAggregator eventAggregator)
        {
            _eventAggregator = eventAggregator;
            _eventAggregator.Subscribe(this);
            _dataListViewModel = new DataListViewModel(eventAggregator);
            _dataToolboxViewModel = new DataToolboxViewModel(eventAggregator);
            _dataPlotViewModel = new LogDataPlotViewModel(eventAggregator);
        }


        public async void LoadDataFromFile()
        {
            var fileDialog = new Microsoft.Win32.OpenFileDialog() { Filter = "txt files (*.txt)|*.txt|All files (*.*)|*.*" };
            var result = fileDialog.ShowDialog();
            if (result != false)
            {
                var path = fileDialog.FileName;
                await HeavyMethodAsync(path);
            }
        }

        internal async Task HeavyMethodAsync(string in_path)
        {
            var logDataReader = new LogDataReaderFromFile();
            var logData = await logDataReader.Read(in_path);
            _loadedLogData.Add(logData);
            DataPlotViewModel.UpdateUi(logData); //todo refactor LogDataReader
        }

        public void Handle(LogDataChangedEvent message)
        {
            var isAlreadyLoaded = false;
            var currentLogData = new LogData();
            foreach (var logData in _loadedLogData)
            {
                if (logData.Name == message.LogDataName)
                {
                    currentLogData = logData;
                    isAlreadyLoaded = true;
                }

            }

            if (isAlreadyLoaded)
            {
                DataPlotViewModel.UpdateUi(currentLogData);
            }
            else
            {
                HeavyMethodAsync(message.Filepath);
            }
        }

        public DataListViewModel LocalDataListViewModel
        {
            get => _dataListViewModel;
            set
            {
                _dataListViewModel= value;
                NotifyOfPropertyChange();
            }
        }

        public DataToolboxViewModel DataToolboxViewModel
        {
            get => _dataToolboxViewModel;
            set
            {
                _dataToolboxViewModel = value;
                NotifyOfPropertyChange();
            }
        }

        public LogDataPlotViewModel DataPlotViewModel
        {
            get => _dataPlotViewModel;
            set
            {
                _dataPlotViewModel = value;
                NotifyOfPropertyChange();
            }
        }

        public Task HandleAsync(LogDataChangedEvent message, CancellationToken cancellationToken)
        {
            throw new System.NotImplementedException(); //todo look this up
        }
    }
}
