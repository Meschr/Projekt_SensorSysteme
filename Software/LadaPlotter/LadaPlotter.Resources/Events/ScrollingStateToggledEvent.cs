namespace LadaPlotter.Resources.Events
{
    public class ScrollingStateToggledEvent
    {
        public ScrollingStateToggledEvent(bool state)
        {
            ToggleSwitchState = state;
        }
        public bool ToggleSwitchState { get; }
    }
}
