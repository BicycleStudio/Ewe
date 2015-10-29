#include "DShowAudio.h"

using sound::direct_show::Audio;
#define SAFE_RELEASE(pointer) { if(pointer) { pointer->Release(); pointer = 0; } }

Audio::Audio() {
  _graphBuilder = 0;
  _mediaControl = 0;
  _mediaEvent = 0;
}

Audio::~Audio() {
}

void Audio::shutdown() {
  stop();
  SAFE_RELEASE(_mediaEvent);
  SAFE_RELEASE(_mediaControl);
  SAFE_RELEASE(_graphBuilder);
}

HRESULT Audio::initialize(std::string fileName) {

  HRESULT hres = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&_graphBuilder);
  if (FAILED(hres)) return hres;

  hres = _graphBuilder->QueryInterface(IID_IMediaControl, (void**)&_mediaControl);
  if (FAILED(hres)) return hres;

  hres = _graphBuilder->QueryInterface(IID_IMediaEvent, (void**)&_mediaEvent);
  if (FAILED(hres)) return hres;

  size_t size = strlen(fileName.c_str()) + 1;
  WCHAR* wFilename = new WCHAR[size];
  size_t outSize;
  mbstowcs_s(&outSize, wFilename, size, fileName.c_str(), size - 1);

  hres = _graphBuilder->RenderFile(wFilename, 0);
  return hres;
}

HRESULT Audio::pause() {
  return _mediaControl->Pause();
}

HRESULT Audio::run() {
  return _mediaControl->Run();
}

HRESULT Audio::stop() {
  return _mediaControl->Stop();
}