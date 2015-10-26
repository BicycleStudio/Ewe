#include "DirectSoundFacade.h"
#define CHECK_HRESULT(hres,msg) { if(FAILED(hres)) { log->fatal(msg); return false; } }

sound::DirectSoundFacade::DirectSoundFacade() {
  log = new utils::Logger(typeid(*this).name());
  _dSound = 0;
  _primaryBuffer = 0;
}
sound::DirectSoundFacade::~DirectSoundFacade() {
  delete log;
}

bool sound::DirectSoundFacade::_initialize(int hwnd) {
  HWND hwnd_ = reinterpret_cast <HWND> (hwnd);

  CHECK_HRESULT(DirectSoundCreate8(NULL, &_dSound, NULL), 
    "Can't create DirectSound Interface.");
  
  return true;
}

void sound::DirectSoundFacade::_shutdown() {

}