#include "Sound.h"

#include <iostream>

using std::cout;
using namespace sound;
static const int soundSleep = 100;

command_manager::ID Sound::id() {
  return command_manager::ID::SOUND;
}

Sound::Sound() {
  log = new utils::Logger(typeid(*this).name());
  _sleepThread = soundSleep;
}

Sound::~Sound() {
  delete log;
}

void Sound::_processCommand (command_manager::Command& c) {
  using command_manager::CommandType;
  switch (c.commandType) {
  case CommandType::INITIALIZE:
    if(!_initialize(c.argInt[0])) {
      _sendKill();
      return;
    }
    _initialized = true;
    log->info("Sound init [OK]");
    break;
  case CommandType::INIT_BACKGROUND_AUDIO:
    _initBackgroundAudio(c.argStr[0].c_str());
    break;
  default: break;
  }
  return;
}

void Sound::stop() {
  _shutdown();
  log->info("Sound thread was stopped");

  this->_willStop = true;
}

void Sound::processTick() {
  if(_paused) return;

  _processCommands();
}

void Sound::pause() {
  log->info("Sound pause");
  this->_paused = true;
  _pause();
}

void Sound::resume() {
  log->info("Sound resume");
  this->_paused = false;
  _resume();
}