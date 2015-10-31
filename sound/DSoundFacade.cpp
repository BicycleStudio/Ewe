#include "DSoundFacade.h"

#define CHECK_HRESULT_FATAL(hres,msg) { if(FAILED(hres)) { log->fatal(msg); return false; } }
#define CHECK_HRESULT_WARN(hres,msg) { if(FAILED(hres)) log->warn(msg); }
#define SAFE_RELEASE(d3dPonter) { if(d3dPonter) { d3dPonter->Release(); d3dPonter = 0; } }

static const int samplePerSecondPrimary = 22050;
static const int bitzPerSamplePrimary = 16;
static const int channelsPrimary = 1;

using sound::direct_sound::SoundFacade;

SoundFacade::SoundFacade() {
  log = new utils::Logger(typeid(*this).name());
  _dSound = 0;
  _primaryBuffer = 0;
}

SoundFacade::~SoundFacade() {
  delete log;
}

bool SoundFacade::_initialize(int hwnd) {
  HWND hwnd_ = reinterpret_cast <HWND> (hwnd);

  CHECK_HRESULT_FATAL(DirectSoundCreate8(NULL, &_dSound, NULL),
    "Can't create DirectSound Interface.");
  
  CHECK_HRESULT_FATAL(_dSound->SetCooperativeLevel(hwnd_, DSSCL_PRIORITY),
    "Can't set cooperative level.");
  
  DSBUFFERDESC bufferDesc;
  ZeroMemory(&bufferDesc, sizeof(DSBUFFERDESC));

  bufferDesc.dwSize = sizeof(DSBUFFERDESC);
  bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;

  CHECK_HRESULT_FATAL(_dSound->CreateSoundBuffer(&bufferDesc, &_primaryBuffer, NULL),
    "Can't create primaryBuffer");

  WavFormat primary = WavFormat(samplePerSecondPrimary, bitzPerSamplePrimary, channelsPrimary);

  if (!_setPrimaryBufferFormat(primary.format)) return false;
  _primaryBuffer->Play(0, 0, DSBPLAY_LOOPING);

  return true;
}

void SoundFacade::_shutdown() {
  for (auto& b : _audios) {
    b.shutdown();
  }
  CHECK_HRESULT_WARN(_primaryBuffer->Stop(), "Can't Stop primaryBuffer.");
  SAFE_RELEASE(_primaryBuffer);
  SAFE_RELEASE(_dSound);
  log->info("shutdown...");
}

void SoundFacade::_pause() {
  for (auto& a : _audios) 
    CHECK_HRESULT_WARN(a.pause(), "Can't pause audio for pause.");
}

void SoundFacade::_resume() {
  for (auto& a : _audios)
    CHECK_HRESULT_WARN(a.run(), "Can't run audio for resume.");
}

bool SoundFacade::_setPrimaryBufferFormat(WAVEFORMATEX& waveFormat) {
  CHECK_HRESULT_FATAL(_primaryBuffer->SetFormat(&waveFormat), "Can't setFormat for primaryBuffer.");
  
  return true;
}

