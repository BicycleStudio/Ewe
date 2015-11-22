#include "DXInstance.h"
#include "DXSupport.h"
#include "DXMaterial.h"

using namespace graphic::direct_x;
using namespace utils::direct_x;

Instance::Instance() {
  log = new utils::Logger(typeid(*this).name());
  XMStoreFloat4x4(&_world, XMMatrixIdentity());
}

Instance::~Instance() {
  delete log;
}

void Instance::shutdown() {
  _cBuffer.shutdown();
}

bool Instance::initialize(Model* model, ID3D11Device* device, int cbSlot) {
  _model = model;
  _cBufferSlot = cbSlot;
  // TODO: _cBuffer.initializeConstantBuffer(_world);
  return true;
}

