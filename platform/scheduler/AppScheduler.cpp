#include "AppScheduler.hpp"

AppScheduler::AppScheduler() {}

AppScheduler::~AppScheduler() {}

int AppScheduler::allocId() {
    return this->_nextId++;
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
