#include "DSoundAudio.h"

#define SAFE_RELEASE(d3dPonter) { if(d3dPonter) { d3dPonter->Release(); d3dPonter = 0; } }

static const int samplePerSecond = 22050;
static const int bitzPerSample = 16;
static const int channels = 1;

using sound::direct_sound::Audio;

Audio::Audio() {
  _buffer = 0;
}

void Audio::shutdown() {
  _buffer->Stop();
  SAFE_RELEASE(_buffer);
}

bool Audio::pause() {
  HRESULT hr = _buffer->Stop();
  if (FAILED(hr)) return false;
  return true;
}

bool Audio::run() {
  HRESULT hr = _buffer->Play(0,0,0);
  if (FAILED(hr)) return false;
  return true;
}

bool Audio::stop() {
  HRESULT hr = _buffer->Stop();
  if (FAILED(hr)) return false;
  return true;
}

bool Audio::initialize(std::string fileName, int forDSound) {
  WAVEFORMATEX waveFormat;
  waveFormat.wFormatTag = WAVE_FORMAT_PCM;
  waveFormat.nSamplesPerSec = samplePerSecond;
  waveFormat.wBitsPerSample = bitzPerSample;
  waveFormat.nChannels = channels;
  waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
  waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
  waveFormat.cbSize = 0;

  int seconds = 10;

  if (!_create(reinterpret_cast<IDirectSound8*>(forDSound), seconds, waveFormat))
    return false;

  if (!_updateRandom())
    return false;

  return true;
}

bool Audio::_create(IDirectSound8* dSound, DSBUFFERDESC& bdesc) {
  IDirectSoundBuffer* tempBuffer;

  if (FAILED(dSound->CreateSoundBuffer(&bdesc, &tempBuffer, NULL))) {
    return false;
  }
  else {
    if (FAILED(tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&_buffer))) {
      tempBuffer->Release();
      return false;
    }
    else {
      tempBuffer->Release();
      return true;
    }
  }
}

bool Audio::_create(IDirectSound8* dSound, int seconds, WAVEFORMATEX& wfex) {
  DSBUFFERDESC bdesc;
  ZeroMemory(&bdesc, sizeof(DSBUFFERDESC));

  bdesc.dwSize = sizeof(DSBUFFERDESC);
  bdesc.dwFlags = DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN;
  bdesc.dwBufferBytes = wfex.nAvgBytesPerSec * seconds;
  bdesc.lpwfxFormat = &wfex;

  return _create(dSound, bdesc);
}

bool Audio::_create(IDirectSound8* dSound, int seconds, WAVEFORMATEX& wfex, DWORD flags) {
  DSBUFFERDESC bdesc;
  ZeroMemory(&bdesc, sizeof(DSBUFFERDESC));

  bdesc.dwSize = sizeof(DSBUFFERDESC);
  bdesc.dwFlags = flags;
  bdesc.dwBufferBytes = wfex.nAvgBytesPerSec * seconds;
  bdesc.lpwfxFormat = &wfex;

  return _create(dSound, bdesc);
}

bool Audio::_updateRandom() {
  char* data; DWORD size;
  if (FAILED(_buffer->Lock(0, 0, (void**)&data, (DWORD*)&size, NULL, 0, DSBLOCK_ENTIREBUFFER))) 
    return false;

  for (DWORD i = 0; i < size; i++) data[i] = rand() % 65536;

  if (FAILED(_buffer->Unlock((void*)data, size, NULL, 0))) 
    return false;

  return true;
}