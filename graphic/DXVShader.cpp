#include "DXVShader.h"
#include "DXSupport.h"

using namespace graphic::direct_x;
using namespace utils::direct_x;

VShader::VShader() {
  log = new utils::Logger(typeid(*this).name());
  _target = "vs_5_0";
  _shaderBlob = 0;
  _shader = 0;
}

VShader::~VShader() {
  delete log;
}

void VShader::shutdown() {
  SAFE_RELEASE(_shaderBlob);
  SAFE_RELEASE(_shader);
}

bool VShader::compileFromFile(ID3D11Device* device, string fileName, string funcName) {
  SAFE_RELEASE(_shaderBlob);
  if(!_compileFromFile(device, fileName, funcName, &_shaderBlob))
    return false;

  CHECK_HRESULT(device->CreateVertexShader(_shaderBlob->GetBufferPointer(), _shaderBlob->GetBufferSize(), NULL, &_shader),
    log->error("Create VShader for " + fileName + " failed."));

  return true;
}

bool VShader::createInputLayout(ID3D11Device* device, const D3D11_INPUT_ELEMENT_DESC* layout, UINT arraySize, ID3D11InputLayout** inputLayout) {
  CHECK_HRESULT(device->CreateInputLayout(layout, arraySize, _shaderBlob->GetBufferPointer(), _shaderBlob->GetBufferSize(), inputLayout),
    log->error("Create InputLayout for material failed."));

  return true;
}

void VShader::set(ID3D11DeviceContext* context) {
  context->VSSetShader(_shader, 0, 0);
}
