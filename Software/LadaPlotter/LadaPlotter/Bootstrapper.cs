using System;
using System.Diagnostics;
using System.IO;
using System.Windows;
using Caliburn.Micro;
using LadaPlotter.Resources.Logging;
using LadaPlotter.UI.Shell;

namespace LadaPlotter.UI;

public class Bootstrapper : BootstrapperBase
{
    private static readonly ILogger Logger = Logger<Bootstrapper>.Create();

    public Bootstrapper()
    {
        Initialize();
    }

    protected override void OnStartup(object sender, StartupEventArgs e)
    {
        CheckLogDirectory();
        Logger.Info("Application startup ...");
        Logger.Info($"Machine name: {Environment.MachineName}");
        Logger.Info($"User name: {Environment.UserName}");
        Logger.Info($"Operating System: {Environment.OSVersion}");
        Logger.Info($"Process ID: {Process.GetCurrentProcess().Id}");

        DisplayRootViewForAsync<ShellViewModel>();
    }

    protected override void Configure()
    {
    }

    private void CheckLogDirectory()
    {
        var logDirectory = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData),
            "Ladadogger\\MeasurementData");

        if (!Directory.Exists(logDirectory))
        {
            Directory.CreateDirectory(logDirectory);
            Logger.Info("Created directory for measurement data in" + logDirectory);
        }
    }
}