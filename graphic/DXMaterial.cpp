#include "DXMaterial.h"
#include "DXSupport.h"
#include "DXVShader.h"
#include "DXPShader.h"

using namespace graphic::direct_x;
using namespace direct_x;

Material::Material() {
  log = new utils::Logger(typeid(*this).name());
  _inputayout = 0;
  _vertexStride = 0;
}

Material::~Material() {
  delete log;
}

bool Material::initialize(ID3D11Device* device) {
  std::string defaultPath = "c:/Prog/default.hlsl";

  VShader* vShader = new VShader();
  if (!vShader->compileFromFile(device, defaultPath, "vsDefault")) {
    delete vShader;
    return false;
  }
  PShader* pShader = new PShader();
  if (!pShader->compileFromFile(device, defaultPath, "psDefault")) {
    delete vShader;
    delete pShader;
    return false;
  }

  const D3D11_INPUT_ELEMENT_DESC layout[] =  {
    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
  };
  _vertexStride = 12;

  if (!vShader->createInputLayout(device, layout, ARRAYSIZE(layout), &_inputayout)) {
    delete vShader;
    delete pShader;
    return false;
  }

  _shaders.push_back(vShader);
  _shaders.push_back(pShader);

  return true;
}

void Material::shutdown() {
  for (auto sh : _shaders) {
    sh->shutdown();
    delete sh;
  }
  _shaders.clear();
  SAFE_RELEASE(_inputayout);
}

void Material::set(ID3D11DeviceContext* context) {
  for (auto sh : _shaders) 
    sh->set(context);
  
  context->IASetInputLayout(_inputayout);
}

UINT Material::getStride() {
  return _vertexStride;
}
