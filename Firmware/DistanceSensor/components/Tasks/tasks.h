#ifndef _tasks_hf
#define _tasks_hf

#include <atomic>

enum ERestartTasks
{
    restart_fast = 0x1,
    restart_log = 0x2
};
extern std::atomic<int> flagrestart;

#endif // #ifndef _tasks_hf
