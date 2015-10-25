#include "Sound.h"

#include <iostream>

using std::cout;
static const int soundSleep = 100;

command_manager::ID sound::Sound::id() {
  return command_manager::ID::SOUND;
}

sound::Sound::Sound() {
}

void sound::Sound::processCommand (command_manager::Command& c) {
  using command_manager::CommandType;
  switch (c.commandType) {
  case CommandType::PAUSE: cout << "Sound pause"; break;
  case CommandType::RESUME: cout << "Sound resume"; break;
  default: break;
  }
  return;
}

void sound::Sound::stop() {
  cout << "Sound thread was stopped\n";

  this->_willStop = true;
}

void sound::Sound::start() {
  cout << "Sound thread was started\n";

  while (!this->_willStop) {
    auto a = std::chrono::milliseconds(soundSleep);
    std::this_thread::sleep_for (a);

    processCommands ();
  }
}

void sound::Sound::pause() {
  this->_paused = true;
}

void sound::Sound::resume() {
  this->_paused = false;
}