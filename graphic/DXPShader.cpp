#include "DXPShader.h"
#include "DXSupport.h"

using namespace graphic::direct_x;
using namespace direct_x;

PShader::PShader() {
  log = new utils::Logger(typeid(*this).name());
  _target = "ps_5_0";
  _shader = 0;
}

PShader::~PShader() {
  delete log;
}

void PShader::shutdown() {
  SAFE_RELEASE(_shader);
}

bool PShader::compileFromFile(ID3D11Device* device, string fileName, string funcName) {
  ID3DBlob* shaderBlob = NULL;
  if(!_compileFromFile(device, fileName, funcName, &shaderBlob))
    return false;

  HRESULT hres = device->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &_shader);
  SAFE_RELEASE(shaderBlob);

  CHECK_HRESULT(hres, log->error("Create PShader for " + fileName + " failed."));

  return true;
}

void PShader::set(ID3D11DeviceContext* context) {
  context->PSSetShader(_shader, 0, 0);
}