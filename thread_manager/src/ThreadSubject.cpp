#include "ThreadSubject.h"

static const int threadManagerSleep = 100;

using std::shared_ptr;
using std::make_shared;
using std::queue;

using namespace thread_manager;
using command_manager::CommandManager;
using command_manager::Command;
using command_manager::CommandType;
using command_manager::ID;

ThreadSubject::ThreadSubject() {
  this->_commands = make_shared<queue<Command>>();
  this->_willStop = false;
  this->_paused = false;
}

void ThreadSubject::_processCommands() {
  while (this->_commands->size() > 0) {
    auto& c = this->_commands->front();
    _processCommand(c);
    this->_commands->pop();
  }
}

void ThreadSubject::bind(CommandManager* commandManager) {
  _commandManager = commandManager;
}

shared_ptr<queue<Command>> ThreadSubject::getQueueLink() {
  return this->_commands;
}

void ThreadSubjectWithKill::_sendKill() {
  Command commandKill = Command(
    this->id(), ID::WINDOW_FACADE,
    CommandType::KILL);
  _commandManager->push(commandKill);
}
