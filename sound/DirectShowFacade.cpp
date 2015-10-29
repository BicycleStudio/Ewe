#include "DirectShowFacade.h"


sound::DirectShowFacade::DirectShowFacade() {
  log = new utils::Logger(typeid(*this).name());
}

sound::DirectShowFacade::~DirectShowFacade() {
  delete log;
}

void sound::DirectShowFacade::_shutdown() {
}

void sound::DirectShowFacade::_pause() {
}

void sound::DirectShowFacade::_resume() {
}

bool sound::DirectShowFacade::_initialize(int hwnd) {
  return true;
}