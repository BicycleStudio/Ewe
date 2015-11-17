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
  auto bd = buffer_desc(vertSize, countVerts, D3D11_BIND_VERTEX_BUFFER);
  auto data = subres_data(verts);
  return initialize(device, bd, data);
}

bool Buffer::initializeIndexBuffer(ID3D11Device* device, UINT* inds, int countInds){
  auto bd = buffer_desc(sizeof(UINT), countInds, D3D11_BIND_INDEX_BUFFER);
  auto data = subres_data(inds);
  return !initialize(device, bd, data);
}

inline D3D11_BUFFER_DESC Buffer::buffer_desc(int vertSize, int countVerts, DWORD flag) {
  D3D11_BUFFER_DESC bd;
  ZeroMemory(&bd, sizeof(bd));
  bd.Usage = D3D11_USAGE_DEFAULT;
  bd.ByteWidth = vertSize* countVerts;
  bd.BindFlags = flag;
  return bd;
}

inline D3D11_SUBRESOURCE_DATA Buffer::subres_data(void* verts) {
  D3D11_SUBRESOURCE_DATA d;
  ZeroMemory(&d, sizeof(d));
  d.pSysMem = verts;
  return d;
}

void Buffer::shutdown() {
  SAFE_RELEASE(_buffer)
}

ID3D11Buffer* Buffer::get() {
  return _buffer;
}