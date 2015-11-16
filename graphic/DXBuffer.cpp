#include "DXBuffer.h"
#include "DXSupport.h"

using namespace utils::direct_x;
using namespace graphic::direct_x;

Buffer::Buffer() {
  log = new utils::Logger(typeid(*this).name());
  _buffer = nullptr;
}

Buffer::~Buffer() {
  delete log;
}

bool Buffer::initialize(ID3D11Device* device, D3D11_BUFFER_DESC& bd, D3D11_SUBRESOURCE_DATA& data) {
  SAFE_RELEASE(_buffer)
  CHECK_HRESULT(device->CreateBuffer(&bd, &data, &_buffer),
    log->error("CreateBuffer failed."));

  return true;
}

bool Buffer::initializeVertexBuffer(ID3D11Device* device, void* verts, int vertSize, int countVerts){
  return _initialize(device, verts, vertSize, countVerts, D3D11_BIND_VERTEX_BUFFER);
}

bool Buffer::initializeIndexBuffer(ID3D11Device* device, UINT* verts, int countInds){
  return _initialize(device, verts, sizeof(UINT), countInds, D3D11_BIND_INDEX_BUFFER);
}

bool Buffer::_initialize(ID3D11Device* device, void* verts, int vertSize, int countVerts, DWORD flag){
  D3D11_BUFFER_DESC bd;
  ZeroMemory(&bd, sizeof(bd));
  bd.Usage = D3D11_USAGE_DEFAULT;
  bd.ByteWidth = vertSize* countVerts;
  bd.BindFlags = flag;

  D3D11_SUBRESOURCE_DATA data;
  ZeroMemory(&data, sizeof(data));
  data.pSysMem = verts;

  if (!initialize(device, bd, data)) return false;
  return true;
}

void Buffer::shutdown() {
  SAFE_RELEASE(_buffer)
}

ID3D11Buffer* Buffer::get() {
  return _buffer;
}