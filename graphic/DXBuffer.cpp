#include "DXBuffer.h"

#define CHECK_HRESULT_ERROR(hres,msg) { if(FAILED(hres)) { log->error(msg); return false; } }
#define SAFE_RELEASE(d3dPonter) { if(d3dPonter) { d3dPonter->Release(); d3dPonter = 0; } }

using namespace graphic::direct_x;

Buffer::Buffer() {
  log = new utils::Logger(typeid(*this).name());
  _buffer = 0;
}

Buffer::~Buffer() {
  delete log;
}

bool Buffer::initialize(ID3D11Device* device, D3D11_BUFFER_DESC& bd, D3D11_SUBRESOURCE_DATA& data) {
  shutdown();
  CHECK_HRESULT_ERROR(device->CreateBuffer(&bd, &data, &_buffer),
    "CreateBuffer for indexes failed.");

  return true;
}

bool Buffer::initialize(ID3D11Device* device, UINT* indexes, int countInds) {
  D3D11_BUFFER_DESC bd;
  bd.Usage = D3D11_USAGE_DEFAULT;
  bd.ByteWidth = sizeof(UINT) * countInds;
  bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

  D3D11_SUBRESOURCE_DATA data;
  ZeroMemory(&data, sizeof(data));
  data.pSysMem = indexes;

  if(!initialize(device, bd, data)) return false;
  return true;
}

bool Buffer::initialize(ID3D11Device* device, Vertex* verts, int vertSize, int countVerts) {
  D3D11_BUFFER_DESC bd;
  bd.Usage = D3D11_USAGE_DEFAULT;
  bd.ByteWidth = vertSize * countVerts;
  bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

  D3D11_SUBRESOURCE_DATA data;
  ZeroMemory(&data, sizeof(data));
  data.pSysMem = verts;

  if(!initialize(device, bd, data)) return false;
  return true;
}

void Buffer::shutdown() {
  SAFE_RELEASE(_buffer)
}