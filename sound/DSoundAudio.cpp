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

bool Audio::initialize(std::string fileName, IDirectSound8* dsound) {
  WaveHeader* waveHdr = new WaveHeader();

  FILE* fp;
  fopen_s(&fp, fileName.c_str(), "rb");
  fread(waveHdr, 1, sizeof(WaveHeader), fp);
  fclose(fp);

  if (memcmp(waveHdr->riffSig, "RIFF", 4) || memcmp(waveHdr->waveSig, "WAVE", 4) ||
    memcmp(waveHdr->formatSig, "fmt ", 4) || memcmp(waveHdr->dataSig, "data", 4))
    return false;

  WAVEFORMATEX waveFormat;
  waveFormat.wFormatTag = WAVE_FORMAT_PCM;
  waveFormat.nSamplesPerSec = waveHdr->sampleRate;
  waveFormat.wBitsPerSample = waveHdr->bitsPerSample;
  waveFormat.nChannels = waveHdr->channels;
  waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
  waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
  waveFormat.cbSize = 0;

  if (!_create(dsound, waveHdr->dataSize, waveFormat))
    return false;

  if (!_update(fileName,0,waveHdr->dataSize))
    return false;

  return true;
}

bool Audio::_create(IDirectSound8* dSound, DSBUFFERDESC& bdesc) {
  IDirectSoundBuffer* tempBuffer;

  if (FAILED(dSound->CreateSoundBuffer(&bdesc, &tempBuffer, NULL))) 
    return false;
  
  if (FAILED(tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&_buffer))) {
    tempBuffer->Release();
    return false;
  }

  tempBuffer->Release();
  return true;
}

bool Audio::_create(IDirectSound8* dSound, long dataSize, WAVEFORMATEX& wfex) {
  DSBUFFERDESC bdesc;
  ZeroMemory(&bdesc, sizeof(DSBUFFERDESC));

  bdesc.dwSize = sizeof(DSBUFFERDESC);
  bdesc.dwFlags = DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN;
  bdesc.dwBufferBytes = dataSize;
  bdesc.lpwfxFormat = &wfex;

  return _create(dSound, bdesc);
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
  fseek(fp, sizeof(WaveHeader), SEEK_SET);

  BYTE  *ptr1, *ptr2;
  DWORD size1, size2;

  if (FAILED(_buffer->Lock(lockPos, size, (void**)&ptr1, &size1, (void**)&ptr2, &size2, 0))) {
    fclose(fp);
    return false;
  }

  fread(ptr1, 1, size1, fp);
  if (ptr2 != NULL)
    fread(ptr2, 1, size2, fp);

  if (FAILED(_buffer->Unlock(ptr1, size1, ptr2, size2))) {
    fclose(fp);
    return false;
  }

  fclose(fp);
  return true;
}