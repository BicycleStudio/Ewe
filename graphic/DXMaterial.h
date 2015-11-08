#ifndef DIRECTX_MATERIAL_H_
#define DIRECTX_MATERIAL_H_

#include "DependenciesDX.h"
#include <Logger.h>

namespace graphic {

  namespace direct_x {

    class Material {
      utils::Logger* log;
    public:
      Material();
      ~Material();

      bool initialize(ID3D11Device* device);
      void shutdown();
      void set(ID3D11DeviceContext*);

      UINT getStride();

    private:
      bool Material::_compileShaderFromFile(LPCSTR fileName, const D3D_SHADER_MACRO* defines,
        LPCSTR entryPoint, LPCSTR target, UINT flags1, UINT flags2, ID3DBlob** blobOut);

      ID3D11VertexShader* _vShader;
      ID3D11PixelShader*  _pShader;
      ID3D11InputLayout*  _inputayout;
      UINT                _vertexStride;
    };

  }

}

#endif // DIRECTX_MATERIAL_H_
