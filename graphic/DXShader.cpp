#include "DXShader.h"

#include <d3dcompiler.h>
#pragma comment (lib, "d3dcompiler.lib")

#define CHECK_HRESULT_ERROR(hres,msg) { if(FAILED(hres)) { log->error(msg); return false; } }
#define SAFE_RELEASE(d3dPonter) { if(d3dPonter) { d3dPonter->Release(); d3dPonter = 0; } }

using namespace graphic::direct_x;

Shader::Shader(){
}

bool Shader::_compileFromFile(ID3D11Device* device, string fileName, string funcName, ID3DBlob** shaderBlob) {
  UINT flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
#if defined( DEBUG ) || defined( _DEBUG )
  flags |= D3DCOMPILE_DEBUG;
#endif
  ID3DBlob* errorBlob = NULL;

  size_t size = strlen(fileName.c_str()) + 1;
  WCHAR* wFilename = new WCHAR[size];
  size_t outSize;
  mbstowcs_s(&outSize, wFilename, size, fileName.c_str(), size - 1);

  HRESULT hr = D3DCompileFromFile(wFilename, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE,
    funcName.c_str(), _target.c_str(), flags, 0, shaderBlob, &errorBlob);
  if (FAILED(hr)) {
    if (errorBlob != NULL) {
      OutputDebugStringA((char*)errorBlob->GetBufferPointer());
      log->error(reinterpret_cast<const char*>(errorBlob->GetBufferPointer()));
    }
    else
      log->error("Path "+fileName+" is wrong.");

    if (errorBlob) errorBlob->Release();
    return false;
  }
  if (errorBlob) errorBlob->Release();
  return true;
}

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
  if (!_compileFromFile(device, fileName, funcName, &_shaderBlob))
    return false;

  CHECK_HRESULT_ERROR(device->CreateVertexShader(_shaderBlob->GetBufferPointer(), _shaderBlob->GetBufferSize(), NULL, &_shader),
    "Create VShader for " + fileName + " failed.");

  return true;
}

bool VShader::createInputLayout(ID3D11Device* device, const D3D11_INPUT_ELEMENT_DESC* layout, UINT arraySize, ID3D11InputLayout** inputLayout) {
  CHECK_HRESULT_ERROR(device->CreateInputLayout(layout, arraySize, _shaderBlob->GetBufferPointer(), _shaderBlob->GetBufferSize(), inputLayout),
    "Create InputLayout for material failed.");

  return true;
}

void VShader::set(ID3D11DeviceContext* context) {
  context->VSSetShader(_shader, 0, 0);
}

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
  if (!_compileFromFile(device, fileName, funcName, &shaderBlob))
    return false;

  HRESULT hres = device->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &_shader);
  SAFE_RELEASE(shaderBlob);

  CHECK_HRESULT_ERROR(hres, "Create PShader for " + fileName + " failed.");

  return true;
}

void PShader::set(ID3D11DeviceContext* context) {
  context->PSSetShader(_shader, 0, 0);
}