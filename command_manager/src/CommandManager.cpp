#include "CommandManager.h"

using namespace command_manager;

void CommandManager::addQueue (ID id, std::shared_ptr<std::queue<Command>> q) {
  this->queues_ [id] = q;
}

void CommandManager::push (Command& a) {
  this->commands_.push(a);
}

void CommandManager::process () {
  while (this->commands_.size() > 0) {
    auto& a = this->commands_.front ( );
    this->queues_ [a.to]->push (a);
    this->commands_.pop ( );
  }
}