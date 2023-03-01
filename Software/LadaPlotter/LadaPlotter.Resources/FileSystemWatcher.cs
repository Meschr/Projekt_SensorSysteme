using LadaPlotter.Resources.Data;
using LadaPlotter.Resources.Logging;

namespace LadaPlotter.Resources;

public class LogFileDirectoryHandler
{
    private readonly ILogger _logger = Logger<LogFileDirectoryHandler>.Create();
    private readonly string _directory;
    private readonly FileSystemWatcher _watcher;

    public LogFileDirectoryHandler(string directoryToWatch)
    {
        _directory = directoryToWatch;
        _watcher = new FileSystemWatcher(directoryToWatch);

        _watcher.NotifyFilter = NotifyFilters.Attributes
                                | NotifyFilters.CreationTime
                                | NotifyFilters.DirectoryName
                                | NotifyFilters.FileName
                                | NotifyFilters.LastAccess
                                | NotifyFilters.LastWrite
                                | NotifyFilters.Security
                                | NotifyFilters.Size;

        _watcher.Filter = "*.txt";
        _watcher.IncludeSubdirectories = true;
        _watcher.EnableRaisingEvents = true;

        _watcher.Error += OnError;
    }

    public event FileSystemEventHandler FileChanged
    {
        add => _watcher.Changed += value;
        remove => _watcher.Changed -= value;
    }

    public event FileSystemEventHandler FileCreated
    {
        add => _watcher.Created += value;
        remove => _watcher.Created -= value;
    }

    public event FileSystemEventHandler FileDeleted
    {
        add => _watcher.Deleted += value;
        remove => _watcher.Deleted -= value;
    }

    public event RenamedEventHandler FileRenamed
    {
        add => _watcher.Renamed += value;
        remove => _watcher.Renamed -= value;
    }

    private void OnError(object sender, ErrorEventArgs e)
    {
        _logger.Error(e.GetException().Message);
    }

    public void RenameFile(string fileName)
    {
        //todo implement
    }

    public void DeleteFile(string fileName)
    {
        //todo implement
    }

    public List<LogData> GetAllFilesInDirectory()
    {
        var logDataInDirectory = new List<LogData>();

        //Get all files with a .txt extension:
        foreach (var filepath in Directory.GetFiles(_directory, "*.txt"))
            logDataInDirectory.Add(new LogData(Path.GetFileNameWithoutExtension(filepath)));

        return logDataInDirectory;
    }
}