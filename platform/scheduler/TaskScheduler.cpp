#include "TaskScheduler.hpp"

TaskScheduler::TaskScheduler() {}

TaskScheduler::~TaskScheduler() {}

int TaskScheduler::allocId() {
    return this->_nextId++;
}

uint64_t TaskScheduler::allocAddr() { return XHLS_IP_CONTROL_ADDR_INPUT_DATA; }

template <typename T>
const char *TaskScheduler::packData(const T &data) {
    const char *raw = std::reinterpret_cast<const char *>(&data);
    return raw;
}

template <typename T>
T TaskScheduler::unpackData(const char *raw) {
    T data;
    std::memcpy(&data, raw, sizeof(T));
    return data;
}

void TaskScheduler::updateState() {
    for (auto &task : this->_tasks) {
        switch (task->state) {
        case TaskState::NEW:
            task->state = TaskState::READY;
            break;
        case TaskState::READY:
            int id = this->_pt.alloc(task->rescount);
            if (id != -1) {
                task->state = TaskState::RUNNING;
                task->pageNodeId = id;
                reconfig_slot(this->XFpgaInstance, task->kernelConfig.fileName.c_str(),
                              task->kernelConfig.fileSize, psGpioInstancePtr, 0, 0);
                XHls_ip *instance = new XHls_ip();
                XHls_ip_Config instanceConfig;
                instanceConfig.DeviceId = 0;
                instanceConfig.Axilites_BaseAddress = this->allocAddr();
                int status = XHls_ip_CfgInitialize(&instance, &instanceConfig);
                if (status != XST_SUCCESS) {
                    xil_printf("  [ERROR]: Failed to init slot.\n\r");
                    return;
                }
                task->instancePtr = instance;
                for (int i = 0; i < task->size.size(); i++) {
                    auto raw = this->packData(task->args[task->completedBatch][i]);
                    Xhls_ip_Set_data(task->instancePtr, task->size[i], raw);
                }
                XHls_ip_Start(task->instancePtr);
            }
            break;
        case TaskState::RUNNING:
            if (task->completedBatch == task->totalBatch) {
                task->state = TaskState::TERMINATED;
            } else if (XHls_ip_IsDone(task->instancePtr)) {
                task->completedBatch++;
                task->transferData();
                if (task->completedBatch < task->totalBatch) {
                    for (int i = 0; i < task->size.size(); i++) {
                        auto raw = this->packData(task->args[task->completedBatch][i]);
                        Xhls_ip_Set_data(task->instancePtr, task->size[i], raw);
                    }
                    XHls_ip_Start(task->instancePtr);
                }
            }
            break;
        case TaskState::WAITING:
            break;
        case TaskState::TERMINATED:
            this->finish(task->id);
            break;
        default:
            break;
        }
    }
}

int TaskScheduler::append(Task *task) {
    task->id = this->allocId();
    task->priority = 1;
    task->state = TaskState::NEW;
    _tasks.push_back(task);
    _lookup[task->id] = _tasks.end();
    return task->id;
}

void TaskScheduler::finish(int taskId) {
    if (_lookup.count(taskId)) {
        Task *task = *_lookup[taskId];
        _tasks.erase(_lookup[taskId]);
        _lookup.erase(taskId);
        _pt.recycle(task->pageNodeId);
    }
}
