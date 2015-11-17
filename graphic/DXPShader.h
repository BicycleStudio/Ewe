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

      bool compileFromFile(ID3D11Device*, string fileName, string funcName);
      void shutdown();
      void set(ID3D11DeviceContext*);

    private:
      ID3D11PixelShader*       _shader;
    };

  }
}

#endif // DIRECTX_PSHADER_H_