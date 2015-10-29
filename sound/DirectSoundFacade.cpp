#include "DirectSoundFacade.h"
#define CHECK_HRESULT_FATAL(hres,msg) { if(FAILED(hres)) { log->fatal(msg); return false; } }
#define CHECK_HRESULT_WARN(hres,msg) { if(FAILED(hres)) log->warn(msg); }
#define SAFE_RELEASE(d3dPonter) { if(d3dPonter) { d3dPonter->Release(); d3dPonter = 0; } }
static const int samplePerSecond = 22050;
static const int bitzPerSample = 16;
static const int channels = 1;

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
  IDirectSoundBuffer8* secondaryBuffer = _createSecondaryBuffer(10, waveFormat);
  if (secondaryBuffer == 0) return false;
  if (!_updateBuffer(secondaryBuffer)) return false;
  secondaryBuffer->Play(0, 0, 0);

  _secondaryBuffers.push_back(secondaryBuffer);
#pragma endregion

  return true;
}

void sound::DirectSoundFacade::_shutdown() {
  for (auto b : _secondaryBuffers) {
    CHECK_HRESULT_WARN(b->Stop(),"Can't Stop secondaryBuffer.");
    SAFE_RELEASE(b);
  }
  CHECK_HRESULT_WARN(_primaryBuffer->Stop(), "Can't Stop primaryBuffer.");
  SAFE_RELEASE(_primaryBuffer);
  SAFE_RELEASE(_dSound);
  log->info("shutdown...");
}

bool sound::DirectSoundFacade::_updateBuffer(IDirectSoundBuffer8* buf) {
  char* data; DWORD size;
  if (FAILED(buf->Lock(0, 0, (void**)&data, (DWORD*)&size, NULL, 0, DSBLOCK_ENTIREBUFFER))) {
    log->fatal("can't lock buffer for update.");
    return false;
  }

  for (DWORD i = 0; i < size; i++) data[i] = rand() % 65536;

  if (FAILED(buf->Unlock((void*)data, size, NULL, 0))) {
    log->fatal("can't unlock buffer for update.");
    return false;
  }

  return true;
}

bool sound::DirectSoundFacade::_setPrimaryBufferFormat(WAVEFORMATEX& waveFormat) {
  CHECK_HRESULT_FATAL(_primaryBuffer->SetFormat(&waveFormat), "Can't setFormat for primaryBuffer.");
  
  return true;
}

IDirectSoundBuffer8* sound::DirectSoundFacade::_createSecondaryBuffer(DSBUFFERDESC& bdesc) {
  IDirectSoundBuffer8* secondaryBuffer;
  IDirectSoundBuffer* tempBuffer;

  if (FAILED(_dSound->CreateSoundBuffer(&bdesc, &tempBuffer, NULL))) {
    log->warn("Can't create temp SoundBuffer.");
    return 0;
  }
  else {
    if (FAILED(tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&secondaryBuffer))) {
      log->warn("Can't create SecondaryBuffer.");
      tempBuffer->Release();
      return 0;
    }
    else {
      tempBuffer->Release();
      return secondaryBuffer;
    }
  }
}

IDirectSoundBuffer8* sound::DirectSoundFacade::_createSecondaryBuffer(int seconds, WAVEFORMATEX& wfex) {
  DSBUFFERDESC bdesc;
  ZeroMemory(&bdesc, sizeof(DSBUFFERDESC));

  bdesc.dwSize = sizeof(DSBUFFERDESC);
  bdesc.dwFlags = DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN;
  bdesc.dwBufferBytes = wfex.nAvgBytesPerSec * seconds;
  bdesc.lpwfxFormat = &wfex;

  return _createSecondaryBuffer(bdesc);
}

IDirectSoundBuffer8* sound::DirectSoundFacade::_createSecondaryBuffer(int seconds, WAVEFORMATEX& wfex, DWORD flags) {
  DSBUFFERDESC bdesc;
  ZeroMemory(&bdesc, sizeof(DSBUFFERDESC));

  bdesc.dwSize = sizeof(DSBUFFERDESC);
  bdesc.dwFlags = flags;
  bdesc.dwBufferBytes = wfex.nAvgBytesPerSec * seconds;
  bdesc.lpwfxFormat = &wfex;

  return _createSecondaryBuffer(bdesc);
}