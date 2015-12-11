#include <io.h>

#include "DSoundAudio.h"
#include "WavFormat.h"

#define CHECK_HRESULT_ERROR(hres,msg) { if(FAILED(hres)) { log->error(msg); shutdown(); return false; } }
#define CHECK_HRESULT_WARN(hres,msg) { if(FAILED(hres)) log->warn(msg); }
#define SAFE_RELEASE(d3dPonter) { if(d3dPonter) { d3dPonter->Release(); d3dPonter = 0; } }

static const int samplePerSecond = 22050;
static const int bitzPerSample = 16;
static const int channels = 1;
static const DWORD defaultWavFlags = DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN;

using sound::direct_sound::Audio;

Audio::Audio() {
  log = new utils::Logger(typeid(*this).name());
  _buffer = 0;
}

Audio::~Audio() {
  delete log;
}

void Audio::shutdown() {
  if (_buffer)
    CHECK_HRESULT_WARN(_buffer->Stop(),"Stop in shutdown function failed.");
  SAFE_RELEASE(_buffer);
}

bool Audio::pause() {
  CHECK_HRESULT_ERROR(_buffer->Stop(), "Stop in pause function failed.");
  return true;
}

bool Audio::run() {
  CHECK_HRESULT_ERROR(_buffer->Play(0, 0, 0), "Play in run function failed.");
  return true;
}

bool Audio::stop() {
  CHECK_HRESULT_ERROR(_buffer->Stop(), "Stop in stop function failed.");
  return true;
}

bool Audio::initialize(std::string fileName, IDirectSound8* dsound) {
  if (_access(fileName.c_str(), 0) == -1) {
    log->error("File " + fileName + " does not exist.");
    return false;
  }

  FILE* fp;
  WavHeader* waveHdr = new WavHeader();
  fopen_s(&fp, fileName.c_str(), "rb");
  if (fp == NULL) {
    log->error("Can't open file " + fileName + " for reading.");
    return false;
  }
  fread(waveHdr, 1, sizeof(WavHeader), fp);
  fclose(fp);

  if (memcmp(waveHdr->riffSig, "RIFF", 4) || memcmp(waveHdr->waveSig, "WAVE", 4) ||
    memcmp(waveHdr->formatSig, "fmt ", 4) || memcmp(waveHdr->dataSig, "data", 4)) {
    log->error("wave header of " + fileName + " not match wav format.");
    return false;
  }

  WavFormat wave = WavFormat(waveHdr->sampleRate, waveHdr->bitsPerSample, waveHdr->channels);

  if (!_create(dsound, waveHdr->dataSize, wave.format))
    return false;

  if (!_update(fileName,0,waveHdr->dataSize))
    return false;

  return true;
}

bool Audio::_create(IDirectSound8* dSound, DSBUFFERDESC& bdesc) {
  IDirectSoundBuffer* tempBuffer;

  CHECK_HRESULT_ERROR(dSound->CreateSoundBuffer(&bdesc, &tempBuffer, NULL),
    "Can't create temp sound buffer.");
  
  if (FAILED(tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&_buffer))) {
    log->error("Can't get sound buffer interface from temp sound buffer.");
    tempBuffer->Release();
    return false;
  }

  tempBuffer->Release();
  return true;
}

bool Audio::_create(IDirectSound8* dSound, long dataSize, WAVEFORMATEX& wfex) {
  return _create(dSound, dataSize, wfex, defaultWavFlags);
}

bool Audio::_create(IDirectSound8* dSound, long dataSize, WAVEFORMATEX& wfex, DWORD flags) {
  DSBUFFERDESC bdesc;
  ZeroMemory(&bdesc, sizeof(DSBUFFERDESC));

  bdesc.dwSize = sizeof(DSBUFFERDESC);
  bdesc.dwFlags = flags;
  bdesc.dwBufferBytes = dataSize;
  bdesc.lpwfxFormat = &wfex;

  return _create(dSound, bdesc);
}

bool Audio::_update(std::string fileName, long lockPos, long size) {
  FILE* fp;
  fopen_s(&fp, fileName.c_str(), "rb");
  fseek(fp, sizeof(WavHeader), SEEK_SET);

  BYTE  *ptr1, *ptr2;
  DWORD size1, size2;

  if (FAILED(_buffer->Lock(lockPos, size, (void**)&ptr1, &size1, (void**)&ptr2, &size2, 0))) {
    fclose(fp);
    log->error("Can't lock buffer for update.");
    return false;
  }

  fread(ptr1, 1, size1, fp);
  if (ptr2 != NULL)
    fread(ptr2, 1, size2, fp);

  if (FAILED(_buffer->Unlock(ptr1, size1, ptr2, size2))) {
    fclose(fp);
    log->error("Can't unlock buffer for update.");
    return false;
  }

  fclose(fp);
  return true;
}