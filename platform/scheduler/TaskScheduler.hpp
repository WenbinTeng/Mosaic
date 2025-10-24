// ------------------------------------------------------------------------------------------------
// TaskScheduler is solely responsible for page binding, restructuring, execution, data transfer,
// and state machine management for individual tasks.
// ------------------------------------------------------------------------------------------------

#ifndef __TASK_SCHEDULER_H__
#define __TASK_SCHEDULER_H__

#include <any>
#include <list>
#include <string>
#include <unordered_map>

#include "PageTreeO1.hpp"
#include "../shell/configure.h"
#include "../shell/xhls_ip.h"

enum class TaskState { NEW, READY, RUNNING, WAITING, TERMINATED };

struct Task {
    int id;
    int priority;
    int appId;
    int kernelId;
    int kernelFileSize;
    std::string kernelFileName;
    int pageNodeId;
    int totalBatch;
    int completedBatch;
    XHls_ip *instancePtr;
    TaskState state;
    ResCount rescount;
    std::vector<int> size;
    std::vector<std::vector<std::any>> args;
    virtual void transferData() {}
    bool operator<(Task &other) {
        return this->priority < other.priority;
    }
};

struct TaskGraph {
    std::vector<Task *> tasks;
    std::unordered_map<Task *, std::vector<Task *>> dependList;
};

class TaskScheduler {
  private:
    std::list<Task *> _tasks;
    std::unordered_map<int, std::list<Task *>::iterator> _lookup;
    PageTreeO1 _pt;
    int _nextId = 1;
    XFpga XFpgaInstance;
    XGpioPs psGpioInstancePtr;
    int allocId();
    uint64_t allocAddr();
    template <typename T> const char *packData(const T &data);
    template <typename T> T unpackData(const char *raw);
    void updateState();

  public:
    TaskScheduler();
    ~TaskScheduler();
    int append(Task *task);
    void finish(int taskId);
};

#endif