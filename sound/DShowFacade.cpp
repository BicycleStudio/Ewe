#include "DShowFacade.h"

#define CHECK_HRESULT_FATAL(hres,msg) { if(FAILED(hres)) { log->fatal(msg); return false; } }
#define CHECK_RESULT_WARN(res,msg) { if(!res) log->warn(msg); }
#define SAFE_RELEASE(pointer) { if(pointer) { pointer->Release(); pointer = 0; } }

using sound::direct_show::SoundFacade;

SoundFacade::SoundFacade() {
  log = new utils::Logger(typeid(*this).name());
}

SoundFacade::~SoundFacade() {
  delete log;
}

void SoundFacade::_shutdown() {
  for (auto& a : _audios)
    a.shutdown();

  CoUninitialize();
}

void SoundFacade::_pause() {
  if (_initialized) {
    for (auto& a : _audios)
      CHECK_RESULT_WARN(a.pause(), "mediaControl of audio  pause.");
  }
}

void SoundFacade::_resume() {
  if (_initialized) {
    for (auto& a : _audios)
      CHECK_RESULT_WARN(a.run(), "mediaControl of audio run.");
  }
}

bool SoundFacade::_initialize(int hwnd) {
  CHECK_HRESULT_FATAL(CoInitializeEx(NULL, COINIT_MULTITHREADED), "Can't CoInitializeEx with multithread!");

  return true;
}