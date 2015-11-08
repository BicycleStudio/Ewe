#include "DXMaterial.h"

#define CHECK_HRESULT_ERROR(hres,msg) { if(FAILED(hres)) { log->error(msg); return false; } }
#define SAFE_RELEASE(d3dPonter) { if(d3dPonter) { d3dPonter->Release(); d3dPonter = 0; } }

using namespace graphic::direct_x;

Material::Material() {
  log = new utils::Logger(typeid(*this).name());
  _vShader = 0;
  _pShader = 0;
  _inputayout = 0;
  _vertexStride = 0;
}

Material::~Material() {
  delete log;
}

bool Material::initialize(ID3D11Device* device) {
  ID3DBlob* BlobVS_ = nullptr;	ID3DBlob* BlobPS_ = nullptr;
  UINT flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;

  std::string defaultShaderPath = "c:/Prog/default.hlsl";

  if(!_compileShaderFromFile(defaultShaderPath.c_str(), nullptr, "vsDefault", "vs_5_0", flags, 0, &BlobVS_)) return false;
  if(!_compileShaderFromFile(defaultShaderPath.c_str(), nullptr, "psDefault", "ps_5_0", flags, 0, &BlobPS_)) return false;

  CHECK_HRESULT_ERROR(device->CreateVertexShader(BlobVS_->GetBufferPointer(), BlobVS_->GetBufferSize(), nullptr, &_vShader),
    "Create VShader for default material failed.");
  CHECK_HRESULT_ERROR(device->CreatePixelShader(BlobPS_->GetBufferPointer(), BlobPS_->GetBufferSize(), nullptr, &_pShader),
    "Create PShader for default material failed.");

  const D3D11_INPUT_ELEMENT_DESC layout[] =  {
    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
  };
  _vertexStride = 12;
  CHECK_HRESULT_ERROR(device->CreateInputLayout(layout, ARRAYSIZE(layout), BlobVS_->GetBufferPointer(), BlobVS_->GetBufferSize(), &_inputayout),
    "Create InputLayout for default material failed.");

  SAFE_RELEASE(BlobVS_);
  SAFE_RELEASE(BlobPS_);

  return true;
}

void Material::shutdown() {
  SAFE_RELEASE(_vShader);
  SAFE_RELEASE(_pShader);
  SAFE_RELEASE(_inputayout);
}

void Material::set(ID3D11DeviceContext* context) {
  context->VSSetShader(_vShader, 0, 0);
  context->PSSetShader(_pShader, 0, 0);
  context->IASetInputLayout(_inputayout);
}

UINT Material::getStride() {
  return _vertexStride;
}

bool Material::_compileShaderFromFile(LPCSTR fileName, const D3D_SHADER_MACRO* defines,
  LPCSTR entryPoint, LPCSTR target, UINT flags1, UINT flags2, ID3DBlob** blobOut) {
  HRESULT hr; ID3DBlob* pErrorBlob =0;
  hr = D3DX11CompileFromFile(fileName, defines, NULL, entryPoint,
    target, flags1, flags2, NULL, blobOut, &pErrorBlob, NULL);
  if(FAILED(hr)) {
    if(pErrorBlob != NULL) {
      OutputDebugStringA((char*) pErrorBlob->GetBufferPointer());
      log->error(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
    }
    else
      log->error("path to default.hlsl is wrong.");
    if(pErrorBlob) pErrorBlob->Release();
    return false;
  }
  if(pErrorBlob) pErrorBlob->Release();

  return true;
}