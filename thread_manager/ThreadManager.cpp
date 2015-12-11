#include "ThreadManager.h"

static const int threadManagerSleep = 100;

using std::make_shared;
using std::queue;
using std::thread;
using std::this_thread::sleep_for;
using std::chrono::milliseconds;

using command_manager::Command;
using command_manager::CommandType;
using command_manager::ID;

namespace thread_manager {

  ThreadManager::ThreadManager() {
    this->_commands = make_shared<queue<Command>>();
    this->_commandManager.addQueue(
      ID::THREAD_MANAGER,
      this->_commands
      );
  }

  void ThreadManager::add(ThreadSubject * tc) {
    tc->bind(&_commandManager);

    this->_subjects.push_back(tc);
    this->_commandManager.addQueue(tc->id(), tc->getQueueLink());
  }

  void ThreadManager::start() {
    for(auto& c : _subjects)
      _threads.push_back(thread([&c]() { c->start(); }));
  }

  void ThreadManager::stop() {
    for (auto& c : _subjects) c->stop();
    for (auto& t : _threads) t.join();

    _subjects.clear();
    _threads.clear();
  }

  void ThreadManager::pause() {
    for (auto& c : _subjects) c->pause();
  }

  void ThreadManager::resume() {
    for (auto& c : _subjects) c->resume();
  }

  void ThreadManager::listen() {
    while (true) {
      auto a = milliseconds(threadManagerSleep);
      sleep_for(a);

      _commandManager.process();

      while (this->_commands->size() > 0) {
        auto& c = this->_commands->front();
        switch (c.commandType) {

        case CommandType::DESTROY_ALL:
          this->stop();
          return;

        case CommandType::KILL:
          _sendKillWindow();
          break;

        case CommandType::PAUSE:
          this->pause();
          break;

        case CommandType::RESUME:
          this->resume();
          break;

        default: break;
        }
        this->_commands->pop();
      }
    }
  }

  void ThreadManager::_sendKillWindow() {
    Command cmndKill = Command(ID::THREAD_MANAGER, ID::WINDOW_FACADE, CommandType::KILL);
    _commandManager.push(cmndKill);
  }

}