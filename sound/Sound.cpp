#include "Sound.h"

#include <iostream>

using std::cout;
static const int soundSleep = 100;

command_manager::ID sound::Sound::id() {
  return command_manager::ID::SOUND;
}

sound::Sound::Sound() {
  log = new utils::Logger(typeid(*this).name());
}

sound::Sound::~Sound() {
  delete log;
}

void sound::Sound::processCommand (command_manager::Command& c) {
  using command_manager::CommandType;
  switch (c.commandType) {
  case CommandType::INITIALIZE:
    if (!_initialize(c.args[0]))
      _sendKill();
    _initialized = true;
    log->info("Sound init [OK]");
    break;
  default: break;
  }
  return;
}

void sound::Sound::stop() {
  log->info("Sound thread was stopped");

  this->_willStop = true;
}

void sound::Sound::start() {
  log->info("Sound thread was started");

  while (!this->_willStop) {
    auto a = std::chrono::milliseconds(soundSleep);
    std::this_thread::sleep_for (a);

    processCommands ();
  }
}

void sound::Sound::pause() {
  log->info("Sound pause");
  this->_paused = true;
}

void sound::Sound::resume() {
  log->info("Sound resume");
  this->_paused = false;
}