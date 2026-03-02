#ifndef NW4HBM_SND_TASK_MANAGER_H
#define NW4HBM_SND_TASK_MANAGER_H

#include "Task.h"

#include <revolution/mem.h>

#define TASK_NUM 128

namespace nw4hbm {
    namespace snd {
        namespace detail {

            class TaskManager {
            public:
                typedef enum TaskPriority {
                    PRIORITY_LOW = 0,
                    PRIORITY_MIDDLE,
                    PRIORITY_HIGH,
                    PRIORITY_MAX
                } TaskPriority;

            public:
                static TaskManager& GetInstance();

                void AppendTask(Task* task, TaskPriority priority);
                Task* PopTask(TaskPriority priority);

                void Execute();
                bool ExecuteSingle() NO_INLINE;

                void CancelByTaskId(u32 taskId);

                u32 GetTaskBufferSize();
                void* Alloc();
                void Free(void* ptr);

                static u8 mTaskArea[0x2000 + 0x44];

            private:
                TaskManager();

                /* 0x00 */ OSMutex mMutex;
                /* 0x18 */ MEMHeapHandle mHeapHandle;
                /* 0x1C */ Task* mCurrentTask;
                /* 0x20 */ TaskList mTaskList[PRIORITY_MAX];
            };

        }  // namespace detail
    }  // namespace snd
}  // namespace nw4hbm

#endif
