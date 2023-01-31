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

        _watcher.Changed += OnChanged;
        _watcher.Created += OnCreated;
        _watcher.Deleted += OnDeleted;
        _watcher.Renamed += OnRenamed;
        _watcher.Error += OnError;
    }

    public event EventHandler<FileSystemEventArgs> FileChanged;
    public event EventHandler<FileSystemEventArgs> FileCreated;
    public event EventHandler<FileSystemEventArgs> FileDeleted;
    public event EventHandler<RenamedEventArgs> FileRenamed;


    private void OnChanged(object sender, FileSystemEventArgs e)
    {
        FileChanged.Invoke(sender,e);
    }

    private void OnCreated(object sender, FileSystemEventArgs e)
    {
        FileCreated.Invoke(sender,e);
    }

    private void OnDeleted(object sender, FileSystemEventArgs e)
    {
        FileDeleted.Invoke(sender,e);
    }

    private void OnRenamed(object sender, RenamedEventArgs e)
    {
        FileRenamed.Invoke(sender,e);
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