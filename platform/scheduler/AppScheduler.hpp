// ------------------------------------------------------------------------------------------------
// AppScheduler is responsible for DAG topology, dependency resolution, priority and deadline
// propagation, and handing off ready tasks to TaskScheduler.
// ------------------------------------------------------------------------------------------------

#ifndef __APP_SCHEDULER_H__
#define __APP_SCHEDULER_H__

#include <list>
#include <optional>
#include <unordered_map>
#include <vector>

#include "TaskScheduler.hpp"

enum class AppState { NEW, READY, RUNNING, WAITING, TERMINATED };

struct App {
    int id;
    int priority;
    AppState state;
    TaskGraph graph;
    App(int id, int priority, AppState state, TaskGraph &graph)
        : id(id), priority(priority), state(state), graph(graph) {}
};

class AppScheduler {
  private:
    std::list<App *> _apps;
    std::unordered_map<int, std::list<App *>::iterator> _lookup;
    TaskScheduler taskScheduler;
    int _nextId = 1;
    int allocId();
    void updateState();

  public:
    AppScheduler();
    ~AppScheduler();
    int submit(TaskGraph &graph);
    void retire(int appId);
    std::optional<AppState> query(int appId);
};

#endif