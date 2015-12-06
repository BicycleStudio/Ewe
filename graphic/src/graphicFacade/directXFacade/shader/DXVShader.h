#ifndef DIRECTX_VSHADER_H_
#define DIRECTX_VSHADER_H_

#include "DXShader.h"

using std::string;

namespace graphic {

  namespace direct_x {

    class VShader : public Shader {
    public:
      VShader();
      ~VShader();

      bool compileFromFile(ID3D11Device*, string fileName, string funcName) override final;
      void shutdown() override final;
      void set(ID3D11DeviceContext*) override final;
      bool createInputLayout(ID3D11Device*, const D3D11_INPUT_ELEMENT_DESC*, UINT arraySize, ID3D11InputLayout**);

    private:
      ID3D11VertexShader*   _shader;
      ID3DBlob*             _shaderBlob;
    };

  }
}

#endif // DIRECTX_VSHADER_H_