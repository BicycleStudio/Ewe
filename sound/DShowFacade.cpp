#include "DShowFacade.h"

#define CHECK_HRESULT_FATAL(hres,msg) { if(FAILED(hres)) { log->fatal(msg); return false; } }
#define CHECK_HRESULT_WARN(hres,msg) { if(FAILED(hres)) log->warn(msg); }
#define SAFE_RELEASE(pointer) { if(pointer) { pointer->Release(); pointer = 0; } }

// use your favourite file )
static const char* testFile = "c:/test/Rob zombie - Dracula.mp3";
static const char* testFile2 = "c:/test/Blue Stahli - Lets Go.mp3";

using sound::direct_show::Facade;

Facade::Facade() {
  log = new utils::Logger(typeid(*this).name());
  _graphBuilder = 0;
  _mediaControl = 0;
  _mediaEvent = 0;

  _graphBuilder2 = 0;
  _mediaControl2 = 0;
  _mediaEvent2 = 0;
}

Facade::~Facade() {
  delete log;
}

void Facade::_shutdown() {
  SAFE_RELEASE(_mediaEvent2);
  SAFE_RELEASE(_mediaControl2);
  SAFE_RELEASE(_graphBuilder2);

  SAFE_RELEASE(_mediaEvent);
  SAFE_RELEASE(_mediaControl);
  SAFE_RELEASE(_graphBuilder);
  CoUninitialize();
}

void Facade::_pause() {
  if (_initialized) {
    CHECK_HRESULT_WARN(_mediaControl->Pause(), "_mediaControl Pause.");
    CHECK_HRESULT_WARN(_mediaControl2->Pause(), "_mediaControl2 Pause.");
  }
}

void Facade::_resume() {
  if (_initialized) {
    CHECK_HRESULT_WARN(_mediaControl->Run(), "_mediaControl Resume.");
    CHECK_HRESULT_WARN(_mediaControl2->Run(), "_mediaControl Resume.");
  }
}

bool Facade::_initialize(int hwnd) {
  CHECK_HRESULT_FATAL(CoInitializeEx(NULL, COINIT_MULTITHREADED), "Can't CoInitializeEx with multithread!");

  CHECK_HRESULT_FATAL(CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, 
    (void**)&_graphBuilder), "Can't create graphBuilder.");
  CHECK_HRESULT_FATAL(CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder,
    (void**)&_graphBuilder2), "Can't create graphBuilder2.");

  CHECK_HRESULT_FATAL(_graphBuilder->QueryInterface(IID_IMediaControl, (void**)&_mediaControl),
    "Can't get mediaControl from graphBuilder");
  CHECK_HRESULT_FATAL(_graphBuilder->QueryInterface(IID_IMediaEvent, (void**)&_mediaEvent),
    "Can't get mediaEvent from graphBuilder");

  CHECK_HRESULT_FATAL(_graphBuilder2->QueryInterface(IID_IMediaControl, (void**)&_mediaControl2),
    "Can't get mediaControl2 from graphBuilder2");
  CHECK_HRESULT_FATAL(_graphBuilder2->QueryInterface(IID_IMediaEvent, (void**)&_mediaEvent2),
    "Can't get mediaEvent2 from graphBuilder2");

#pragma region test functionality
  {
    size_t size = strlen(testFile2) + 1;
    WCHAR* wFilename = new WCHAR[size];
    size_t outSize;
    mbstowcs_s(&outSize, wFilename, size, testFile2, size - 1);

    HRESULT hr = _graphBuilder->RenderFile(wFilename, 0);

    CHECK_HRESULT_FATAL(hr, "Can't RenderFile.");

    hr = _mediaControl->Run();
  }
  {
    size_t size = strlen(testFile) + 1;
    WCHAR* wFilename = new WCHAR[size];
    size_t outSize;
    mbstowcs_s(&outSize, wFilename, size, testFile, size - 1);

    HRESULT hr = _graphBuilder2->RenderFile(wFilename, 0);

    CHECK_HRESULT_FATAL(hr, "Can't RenderFile2.");

    hr = _mediaControl2->Run();
  }
#pragma endregion

  return true;
}