#ifndef DIRECTX_MATERIAL_H_
#define DIRECTX_MATERIAL_H_

#include "DependenciesDX.h"
#include "shader\DXShader.h"
#include "DXModel.h"
#include <Logger.h>

using std::vector;

namespace graphic {

  namespace direct_x {

    class Material {
      utils::Logger* log;
    public:
      Material();
      ~Material();

      bool initialize(ID3D11Device*, string fileName);
      void shutdown();
      void set(ID3D11DeviceContext*);

      UINT getStride();

      vector<Model*>      models;
    private:
      vector<Shader*>     _shaders;
      ID3D11InputLayout*  _inputayout;
      UINT                _vertexStride;
    };

  }

}

#endif // DIRECTX_MATERIAL_H_
