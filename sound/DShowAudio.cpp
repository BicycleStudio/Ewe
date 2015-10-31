#include "DShowAudio.h"

using sound::direct_show::Audio;
#define SAFE_RELEASE(pointer) { if(pointer) { pointer->Release(); pointer = 0; } }

Audio::Audio() {
  _graphBuilder = 0;
  _mediaControl = 0;
  _mediaEvent = 0;
}

void Audio::shutdown() {
  stop();
  SAFE_RELEASE(_mediaEvent);
  SAFE_RELEASE(_mediaControl);
  SAFE_RELEASE(_graphBuilder);
}

bool Audio::initialize(std::string fileName) {

  HRESULT hres = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&_graphBuilder);
  if (FAILED(hres)) return false;

  hres = _graphBuilder->QueryInterface(IID_IMediaControl, (void**)&_mediaControl);
  if (FAILED(hres)) return false;

  hres = _graphBuilder->QueryInterface(IID_IMediaEvent, (void**)&_mediaEvent);
  if (FAILED(hres)) return false;

  size_t size = strlen(fileName.c_str()) + 1;
  WCHAR* wFilename = new WCHAR[size];
  size_t outSize;
  mbstowcs_s(&outSize, wFilename, size, fileName.c_str(), size - 1);

  hres = _graphBuilder->RenderFile(wFilename, 0);
  if (FAILED(hres)) return false;
  
  return true;
}

bool Audio::pause() {
  HRESULT hres = _mediaControl->Pause();
  if (FAILED(hres)) return false;

  return true;
}

bool Audio::run() {
  HRESULT hres = _mediaControl->Run();
  if (FAILED(hres)) return false;

  return true;
}

bool Audio::stop() {
  HRESULT hres = _mediaControl->Stop();
  if (FAILED(hres)) return false;

  return true;
}