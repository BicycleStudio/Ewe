#ifndef DIRECTX_INSTANCE_H_
#define DIRECTX_INSTANCE_H_

#include "DependenciesDX.h"
#include "DXBuffer.h"
#include <Logger.h>

namespace graphic {
  namespace direct_x {

    class Material;
    class Model;

    class Instance {
      utils::Logger* log;
    public:
      Instance();
      ~Instance();

      bool initialize(Model* model, ID3D11Device*, int cbSlot);
      void shutdown();

      int getSlot();
      ID3D11Buffer* getBuffer();

    private:
      XMFLOAT4X4  _world;
      Buffer      _cBuffer;
      Model*      _model;
      int         _cBufferSlot;
    };

  }
}
#endif // DIRECTX_INSTANCE_H_