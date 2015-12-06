#ifndef DIRECTX_PSHADER_H_
#define DIRECTX_PSHADER_H_

#include "DXShader.h"

using std::string;

namespace graphic {

  namespace direct_x {

    class PShader : public Shader {
    public:
      PShader();
      ~PShader();

      bool compileFromFile(ID3D11Device*, string fileName, string funcName) override final;
      void shutdown() override final;
      void set(ID3D11DeviceContext*) override final;

    private:
      ID3D11PixelShader*       _shader;
    };

  }
}

#endif // DIRECTX_PSHADER_H_