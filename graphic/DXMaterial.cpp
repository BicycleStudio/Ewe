#include <io.h>
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

bool Material::initialize(ID3D11Device* device, string fileName) {
  if(_access(fileName.c_str(), 0) == -1) {
    log->error("File " + fileName + " does not exist.");
    return false;
  }
  FILE* fp;
  fopen_s(&fp, fileName.c_str(), "r");
  if(fp == NULL) {
    log->error("Can't open file " + fileName + " for reading.");
    return false;
  }
  char hlslFile[MAX_PATH];
  char vsFunc[MAX_PATH];
  char psFunc[MAX_PATH];

  fscanf_s(fp, "%s ", hlslFile, _countof(hlslFile));
  fscanf_s(fp, "%s", vsFunc, _countof(vsFunc));
  fscanf_s(fp, "%s", psFunc, _countof(psFunc));

  fclose(fp);

  VShader* vShader = new VShader();
  if(!vShader->compileFromFile(device, hlslFile, vsFunc)) {
    delete vShader;
    return false;
  }
  PShader* pShader = new PShader();
  if(!pShader->compileFromFile(device, hlslFile, psFunc)) {
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
  models.clear();
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
