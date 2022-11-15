#ifndef _tasks_hf
#define _tasks_hf

#include <atomic>

#define SamplingFrequency 500 //in Hz

static int ConvertSamplingFrequencyToSamplingTimeInMs(int frequency)
{
    return static_cast<int>(1000/frequency);
}

enum ERestartTasks
{
    restart_fast = 0x1,
    restart_log = 0x2
};
extern std::atomic<int> flagrestart;

#endif // #ifndef _tasks_hf
