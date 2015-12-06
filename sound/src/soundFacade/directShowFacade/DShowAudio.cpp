#include "DShowAudio.h"
#include <io.h>

#define CHECK_HRESULT_ERROR(hres,msg) { if(FAILED(hres)) { log->error(msg); shutdown(); return false; } }
#define CHECK_HRESULT_WARN(hres,msg) { if(FAILED(hres)) log->warn(msg); }
#define SAFE_RELEASE(pointer) { if(pointer) { pointer->Release(); pointer = 0; } }

using sound::direct_show::Audio;

Audio::Audio() {
  log = new utils::Logger(typeid(*this).name());
  _graphBuilder = 0;
  _mediaControl = 0;
  _mediaEvent = 0;
  _initialized = false;
}

Audio::~Audio() {
  delete log;
}

void Audio::shutdown() {
  if (!_initialized)
    return;

  _initialized = false;
  if (_mediaControl)
    stop();
  SAFE_RELEASE(_mediaEvent);
  SAFE_RELEASE(_mediaControl);
  SAFE_RELEASE(_graphBuilder);
}

bool Audio::initialize(std::string fileName) {
  CHECK_HRESULT_ERROR(CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&_graphBuilder),
    "Can't CoCreateInstance for GraphBuilder.");

  CHECK_HRESULT_ERROR(_graphBuilder->QueryInterface(IID_IMediaControl, (void**)&_mediaControl),
    "Can't get MediaControl from GraphBuilder.");

  CHECK_HRESULT_ERROR(_graphBuilder->QueryInterface(IID_IMediaEvent, (void**)&_mediaEvent),
    "Can't get MediaEvent from GraphBuilder.");

  if (_access(fileName.c_str(), 0) == -1) {
    log->error("File " + fileName + " does not exist.");
    return false;
  }

  size_t size = strlen(fileName.c_str()) + 1;
  WCHAR* wFilename = new WCHAR[size];
  size_t outSize;
  mbstowcs_s(&outSize, wFilename, size, fileName.c_str(), size - 1);

  CHECK_HRESULT_ERROR(_graphBuilder->RenderFile(wFilename, 0),
    "Can't render file " + fileName + ".");
  
  _initialized = true;
  return true;
}

bool Audio::pause() {
  CHECK_HRESULT_ERROR(_mediaControl->Pause(),"Can't pause mediaControl.");

  return true;
}

bool Audio::run() {
  CHECK_HRESULT_ERROR(_mediaControl->Run(), "Can't run mediaControl.");

  return true;
}

bool Audio::stop() {
  CHECK_HRESULT_ERROR(_mediaControl->Stop(), "Can't stop mediaControl.");

  return true;
}

bool Audio::initialized(){
  return _initialized;
}