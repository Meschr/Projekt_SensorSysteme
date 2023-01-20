using System.Windows;
using Caliburn.Micro;
using LadaPlotter.UI.Shell;

namespace LadaPlotter.UI;

public class Bootstrapper : BootstrapperBase
{
    public Bootstrapper()
    {
        Initialize();
    }

    protected override void OnStartup(object sender, StartupEventArgs e)
    {
        DisplayRootViewForAsync<ShellViewModel>();
    }
}