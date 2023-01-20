using LadaPlotter.Resources.Data;

namespace LadaPlotter.Resources.Logic;

public interface ILogDataReader
{
    Task<LogData> Read(string path);
}