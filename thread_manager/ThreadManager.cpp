#include "ThreadManager.h"

static const int threadManagerSleep = 100;

using std::shared_ptr;
using std::make_shared;
using std::queue;
using std::thread;
using std::this_thread::sleep_for;
using std::chrono::milliseconds;

using command_manager::CommandManager;
using command_manager::Command;
using command_manager::CommandType;
using command_manager::ID;

namespace thread_manager {

  ThreadSubject::ThreadSubject() {
    this->commands_ = make_shared<queue<Command>>();
    this->_willStop = false;
    this->_paused = false;
  }

  void ThreadSubject::processCommands() {
    while (this->commands_->size() > 0) {
      auto& c = this->commands_->front();
      processCommand(c);
      this->commands_->pop();
    }
  }

  void ThreadSubject::bind(CommandManager* commandManager) {
    commandManager_ = commandManager;
  }

  shared_ptr<queue<Command>> ThreadSubject::getQueueLink() {
    return this->commands_;
  }

  void ThreadSubjectWithKill::_sendKill() {
    Command commandKill = Command(
      this->id(), ID::THREAD_MANAGER,
      CommandType::KILL);
    commandManager_->push(commandKill);
  }

  ThreadManager::ThreadManager() {
    this->commands_ = make_shared<queue<Command>>();
    this->commandManager_.addQueue(
      ID::THREAD_MANAGER,
      this->commands_
      );
  }

  void ThreadManager::add(ThreadSubject * tc) {
    tc->bind(&commandManager_);

    this->subjects_.push_back(tc);
    this->commandManager_.addQueue(tc->id(), tc->getQueueLink());
  }

  void ThreadManager::start() {
    for (auto c : subjects_)
      threads_.push_back(thread([&c]() { c->start(); }));
  }

  void ThreadManager::stop() {
    for (auto& c : subjects_) c->stop();
    for (auto& t : threads_) t.join();

    subjects_.clear();
    threads_.clear();
  }

  void ThreadManager::pause() {
    for (auto& c : subjects_) c->pause();
  }

  void ThreadManager::resume() {
    for (auto& c : subjects_) c->resume();
  }

  void ThreadManager::listen() {
    while (true) {
      auto a = milliseconds(threadManagerSleep);
      sleep_for(a);

      commandManager_.process();

      while (this->commands_->size() > 0) {
        auto& c = this->commands_->front();
        switch (c.commandType) {

        case CommandType::KILL:
          this->stop();
          return;

        case CommandType::PAUSE:
          this->pause();
          break;

        case CommandType::RESUME:
          this->resume();
          break;

        default: break;
        }
        this->commands_->pop();
      }
    }
  }

}