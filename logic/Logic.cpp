#include "Logic.h"

#include <iostream>

using std::cout;
static const int logicSleep = 100;

command_manager::ID logic::Logic::id() {
  return command_manager::ID::LOGIC;
}

logic::Logic::Logic() {
}

void logic::Logic::processCommand (command_manager::Command& c) {
  using command_manager::CommandType;
  switch (c.commandType) {
  case CommandType::PAUSE: cout << "Logic pause"; break;
  case CommandType::RESUME: cout << "Logic resume"; break;
  default: break;
  }
  return;
}

void logic::Logic::stop() {
  cout << "Logic thread was stopped\n";

  this->_willStop = true;
}

void logic::Logic::start() {
  cout << "Logic thread was started\n";

  while (!this->_willStop) {
    auto a = std::chrono::milliseconds(logicSleep);
    std::this_thread::sleep_for (a);

    processCommands ();
  }
}

void logic::Logic::pause() {
  this->_paused = true;
}

void logic::Logic::resume() {
  this->_paused = false;
}