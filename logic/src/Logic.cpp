#include "Logic.h"

#include <iostream>

using std::cout;
using namespace logic;
static const int logicSleep = 100;

command_manager::ID Logic::id() {
  return command_manager::ID::LOGIC;
}

Logic::Logic() {
  log = new utils::Logger(typeid(*this).name());
}

Logic::~Logic() {
  delete log;
}

void Logic::_processCommand (command_manager::Command& c) {
  using command_manager::CommandType;
  // TODO: process coommand
  /*
  switch (c.commandType) {
  default: break;
  }
  */
  return;
}

void Logic::stop() {
  log->info("Logic thread was stopped");

  this->_willStop = true;
}

void Logic::start() {
  log->info("Logic thread was started");

  while (!this->_willStop) {
    auto a = std::chrono::milliseconds(logicSleep);
    std::this_thread::sleep_for (a);

    _processCommands ();
  }
}

void Logic::pause() {
  log->info("Logic pause");
  this->_paused = true;
}

void Logic::resume() {
  log->info("Logic resume");
  this->_paused = false;
}