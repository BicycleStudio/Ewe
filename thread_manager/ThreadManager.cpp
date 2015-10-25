#include "ThreadManager.h"

static const int threadManagerSleep = 100;

thread_manager::ThreadSubject::ThreadSubject() { 
  this->commands_ = std::make_shared<std::queue<command_manager::Command>>();
  this->willStop = false;
}

void thread_manager::ThreadSubject::processCommands ( ) {
  while (this->commands_->size () > 0) {
    auto& c = this->commands_->front();
    processCommand (c);
    this->commands_->pop();
  }
}

void thread_manager::ThreadSubject::bind(command_manager::CommandManager* commandManager) {
  commandManager_ = commandManager;
}

std::shared_ptr<std::queue<command_manager::Command>> thread_manager::ThreadSubject::getQueueLink ( ) {
  return this->commands_;
}

thread_manager::ThreadManager::ThreadManager ( ) {
  this->commands_ = std::make_shared<std::queue<command_manager::Command>>();
  this->commandManager_.addQueue (
    command_manager::ID::THREAD_MANAGER,
    this->commands_
  );
}

void thread_manager::ThreadManager::add(ThreadSubject * tc) {
  tc->bind(&commandManager_);
  
  this->subjects_.push_back (tc);
  this->commandManager_.addQueue (tc->id ( ), tc->getQueueLink ( ));
}

void thread_manager::ThreadManager::start ( ) {
  for (auto c : subjects_)
    threads_.push_back(std::thread([&c] () { c->start(); }));
}

void thread_manager::ThreadManager::stop() {
  for (auto& c : subjects_) c->stop ();
  for (auto& t : threads_) t.join();

  subjects_.clear ();
  threads_.clear();
}

void thread_manager::ThreadManager::pause() {
  for (auto& c : subjects_) c->pause();
}

void thread_manager::ThreadManager::resume() {
  for (auto& c : subjects_) c->resume();
}

void thread_manager::ThreadManager::listen ( ) {
  while (true) {
    auto a = std::chrono::milliseconds(threadManagerSleep);
    std::this_thread::sleep_for (a);

    commandManager_.process ();

    while (this->commands_->size () > 0) {
      auto& c = this->commands_->front();
      switch (c.commandType) {

        case command_manager::CommandType::KILL: 
          this->stop ( );
          return;
        
        case command_manager::CommandType::PAUSE:
          this->pause();
          break;

        case command_manager::CommandType::RESUME:
          this->resume();
          break;

        default: break;
      }
      this->commands_->pop();
    }
  }
}
