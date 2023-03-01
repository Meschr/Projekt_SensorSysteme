using System;
using System.Collections.Generic;
using System.IO;
using System.Threading;
using System.Threading.Tasks;
using Caliburn.Micro;
using LadaPlotter.Resources.Data;
using LadaPlotter.Resources.Events;
using LadaPlotter.Resources.Logic;
using LadaPlotter.UI.TabElements.DataTab.DataTabControl;
using LadaPlotter.UI.TabElements.DataTab.FileList;
using LadaPlotter.UI.TabElements.DataTab.Toolbox;

namespace LadaPlotter.UI.TabElements.DataTab
{
    public class DataTabViewModel : PropertyChangedBase , IHandle<LogDataChangedEvent>
    {
        private readonly IEventAggregator _eventAggregator;

        private DataListViewModel _dataListViewModel;
        private DataToolboxViewModel _dataToolboxViewModel;
        private AnimatedDataTabControlViewModel _animatedDataTabControlViewModel;

        private List<LogData> _loadedLogData = new List<LogData>();

        public DataTabViewModel(IEventAggregator eventAggregator)
        {
            _eventAggregator = eventAggregator;
            _eventAggregator.Subscribe(this);
            _dataListViewModel = new DataListViewModel(eventAggregator);
            _dataToolboxViewModel = new DataToolboxViewModel(eventAggregator);
            _animatedDataTabControlViewModel = new AnimatedDataTabControlViewModel(eventAggregator);
        }


        public async void LoadDataFromFile()
        {
            var fileDialog = new Microsoft.Win32.OpenFileDialog() { Filter = "txt files (*.txt)|*.txt|All files (*.*)|*.*" };
            var result = fileDialog.ShowDialog();
            if (result != false)
            {
                var path = fileDialog.FileName;
                await HeavyMethodAsync(path);

                //todo make async 
                if (!CheckForExistingFileInMeasurementDirectory(Path.GetFileName(fileDialog.FileName)))
                {
                    var logDirectory = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData),
                        "Ladadogger\\MeasurementData\\");

                    File.Copy(fileDialog.FileName, logDirectory + Path.GetFileName(fileDialog.FileName));
                }
            }
        }

        internal async Task HeavyMethodAsync(string in_path)
        {
            var logDataReader = new LogDataReaderFromFile();
            var logData = await logDataReader.Read(in_path);
            _loadedLogData.Add(logData);
            AnimatedDataTabControlViewModel.UpdateUi(logData); //todo refactor LogDataReader
        }

        public void Handle(LogDataChangedEvent message)
        {
            var isAlreadyLoaded = false;
            var currentLogData = new LogData("");
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
                AnimatedDataTabControlViewModel.UpdateUi(currentLogData);
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

        public AnimatedDataTabControlViewModel AnimatedDataTabControlViewModel
        {
            get => _animatedDataTabControlViewModel;
            set
            {
                _animatedDataTabControlViewModel = value;
                NotifyOfPropertyChange();
            }
        }

        public Task HandleAsync(LogDataChangedEvent message, CancellationToken cancellationToken)
        {
            var isAlreadyLoaded = false;
            var currentLogData = new LogData("");
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
                AnimatedDataTabControlViewModel.UpdateUi(currentLogData);
            }
            else
            {
                HeavyMethodAsync(message.Filepath);
            }
            return Task.CompletedTask;
        }

        bool CheckForExistingFileInMeasurementDirectory(string fileNameWithExtension)
        {
            var logDirectory = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData),
                "Ladadogger\\MeasurementData");

            if (File.Exists(logDirectory + "\\" + fileNameWithExtension))
            {
                return true;
            }

            return false;
        }
    }
}
