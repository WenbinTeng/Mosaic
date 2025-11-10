#include "AppScheduler.hpp"

AppScheduler::AppScheduler() {}

AppScheduler::~AppScheduler() {}

int AppScheduler::allocId() {
    return this->_nextId++;
}

TaskGraph AppScheduler::parseFromYaml(const std::string& yamlPath) {
    TaskGraph graph;
    YAML::Node node = YAML::LoadFile(yamlPath);
    std::unordered_map<int, Task*> dict;
    std::vector<std::vector<int>> depend;

    for (auto taskInfo : node["tasks"]) {
        auto task = new Task;
        task->kernelId       = taskInfo["kernel_id"].as<int>();
        task->kernelFileSize = taskInfo["kernel_file_size"].as<int>();
        task->kernelFileName = taskInfo["kernel_file_name"].as<std::string>();
        task->totalBatch     = taskInfo["total_batch"].as<int>();
        task->completedBatch = 0;
        task->rescount       = ResCount(
            taskInfo["resource_count"]["CLB"].as<int>(),
            taskInfo["resource_count"]["DSP"].as<int>(),
            taskInfo["resource_count"]["BRAM"].as<int>()
        );
        int global_i = 0;
        for (auto argc: taskInfo["arg_count"]) {
            task->size.push_back(argc.as<int>());
        }
        for (auto i : task->size) {
            std::vector<std::any> arg;
            for (int j = 0; j < i; j++) {
                arg.push_back(taskInfo["arg_value"][global_i++].as<int>());
            }
            task->args.push_back(arg);
        }
        dict[task->kernelId] = task;
    }

    for (auto taskInfo : node["tasks"]) {
        int srcId = taskInfo["kernel_id"].as<int>();
        Task *srcTask = dict[srcId];
        for (auto id : taskInfo["depend_task_id"]) {
            int dstId = id.as<int>();
            Task *dstTask = dict[dstId];
            graph.dependList[srcTask].push_back(dstTask);
        }
    }

    return graph;
}

void AppScheduler::updateState() {
    static int last;
    int runningCount = 0;
    for (auto app : _apps) {
        switch (app->state) {
        case AppState::NEW:
            app->state = AppState::READY;
            break;
        case AppState::READY:
            if (last < 4) {
                app->state = AppState::RUNNING;
            }
        case AppState::RUNNING:
            bool terminated = true;
            for (auto task : app->graph.tasks) {
                if (task->state != TaskState::TERMINATED)
                    terminated = false;
                if (task->state == TaskState::NEW) {
                    bool noDepend = true;
                    for (auto dependTask : app->graph.dependList[task]) {
                        if (dependTask->state != TaskState::TERMINATED) {
                            noDepend = false;
                            break;
                        }
                    }
                    if (noDepend) {
                        taskScheduler.append(task);
                    }
                }
                if (task->state == TaskState::RUNNING) {
                    runningCount++;
                }
            }
            if (terminated) {
                app->state = AppState::TERMINATED;
            }
            break;
        case AppState::WAITING:
            break;
        case AppState::TERMINATED:
            this->retire(app->id);
            break;
        default:
            break;
        }
    }
    last = runningCount;
}

int AppScheduler::submit(TaskGraph &graph) {
    App *app = new App(this->allocId(), 1, AppState::NEW, graph);
    _apps.push_back(app);
    _lookup[app->id] = _apps.end();
    return app->id;
}

void AppScheduler::retire(int appId) {
    if (_lookup.count(appId)) {
        App *app = *_lookup[appId];
        _apps.erase(_lookup[appId]);
        _lookup.erase(appId);
        delete app;
    }
}

std::optional<AppState> AppScheduler::query(int appId) {
    if (_lookup.count(appId))
        return (*_lookup[appId])->state;
    return std::nullopt;
}
