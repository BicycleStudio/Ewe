#include "DSoundFacade.h"
#define CHECK_HRESULT_FATAL(hres,msg) { if(FAILED(hres)) { log->fatal(msg); return false; } }
#define CHECK_HRESULT_WARN(hres,msg) { if(FAILED(hres)) log->warn(msg); }
#define SAFE_RELEASE(d3dPonter) { if(d3dPonter) { d3dPonter->Release(); d3dPonter = 0; } }
static const int samplePerSecond = 22050;
static const int bitzPerSample = 16;
static const int channels = 1;
static const std::string testFile = "c:/test/sound01.wav";

using sound::direct_sound::Facade;

Facade::Facade() {
  log = new utils::Logger(typeid(*this).name());
  _dSound = 0;
  _primaryBuffer = 0;
}

Facade::~Facade() {
  delete log;
}

bool Facade::_initialize(int hwnd) {
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

  WAVEFORMATEX waveFormat;
  waveFormat.wFormatTag = WAVE_FORMAT_PCM;
  waveFormat.nSamplesPerSec = samplePerSecond;
  waveFormat.wBitsPerSample = bitzPerSample;
  waveFormat.nChannels = channels;
  waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
  waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
  waveFormat.cbSize = 0;

  if (!_setPrimaryBufferFormat(waveFormat)) return false;
  _primaryBuffer->Play(0, 0, DSBPLAY_LOOPING);

#pragma region TemporaryForTest
  Audio audio;
  if (!audio.initialize(testFile,_dSound)) {
    log->warn("Can't initialize audio!");
    log->info("Can't initialize audio!");
    return false;
  }
  _audios.push_back(audio);
#pragma endregion

  return true;
}

void Facade::_shutdown() {
  for (auto& b : _audios) {
    b.shutdown();
  }
  CHECK_HRESULT_WARN(_primaryBuffer->Stop(), "Can't Stop primaryBuffer.");
  SAFE_RELEASE(_primaryBuffer);
  SAFE_RELEASE(_dSound);
  log->info("shutdown...");
}

void Facade::_pause() {
  for (auto& a : _audios) 
    CHECK_HRESULT_WARN(a.pause(), "Can't pause audio for pause.");
}

void Facade::_resume() {
  for (auto& a : _audios)
    CHECK_HRESULT_WARN(a.run(), "Can't run audio for resume.");
}

bool Facade::_setPrimaryBufferFormat(WAVEFORMATEX& waveFormat) {
  CHECK_HRESULT_FATAL(_primaryBuffer->SetFormat(&waveFormat), "Can't setFormat for primaryBuffer.");
  
  return true;
}

