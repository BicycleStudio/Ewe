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

void sound::Sound::_processCommand (command_manager::Command& c) {
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

void sound::Sound::stop() {
  _shutdown();
  log->info("Sound thread was stopped");

  this->_willStop = true;
}

void sound::Sound::start() {
  log->info("Sound thread was started");

  while (!this->_willStop) {
    auto a = std::chrono::milliseconds(soundSleep);
    std::this_thread::sleep_for (a);

    // TODO: check all audios for end & remove if;
    _processCommands ();
  }
}

void sound::Sound::pause() {
  log->info("Sound pause");
  this->_paused = true;
  _pause();
}

void sound::Sound::resume() {
  log->info("Sound resume");
  this->_paused = false;
  _resume();
}