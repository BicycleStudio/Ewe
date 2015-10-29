#include "DShowFacade.h"

#define CHECK_HRESULT_FATAL(hres,msg) { if(FAILED(hres)) { log->fatal(msg); return false; } }
#define CHECK_RESULT_WARN(res,msg) { if(!res) log->warn(msg); }
#define SAFE_RELEASE(pointer) { if(pointer) { pointer->Release(); pointer = 0; } }

// use your favourite file )
static const char* testFile1 = "c:/test/Rob zombie - Dracula.mp3";

using sound::direct_show::Facade;

Facade::Facade() {
  log = new utils::Logger(typeid(*this).name());
}

Facade::~Facade() {
  delete log;
}

void Facade::_shutdown() {
  for (auto& a : _audios)
    a.shutdown();

  CoUninitialize();
}

void Facade::_pause() {
  if (_initialized) {
    for (auto& a : _audios)
      CHECK_RESULT_WARN(a.pause(), "mediaControl of audio  pause.");
  }
}

void Facade::_resume() {
  if (_initialized) {
    for (auto& a : _audios)
      CHECK_RESULT_WARN(a.run(), "mediaControl of audio run.");
  }
}

bool Facade::_initialize(int hwnd) {
  CHECK_HRESULT_FATAL(CoInitializeEx(NULL, COINIT_MULTITHREADED), "Can't CoInitializeEx with multithread!");

#pragma region test functionality
  { Audio audio; CHECK_RESULT_WARN(audio.initialize(testFile1, 0), "Can't initialize audio."); _audios.push_back(audio); }
#pragma endregion

  return true;
}