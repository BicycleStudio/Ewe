#ifndef DIRECTX_SHADER_H_
#define DIRECTX_SHADER_H_

#include "..\DependenciesDX.h"
#include <Logger.h>

using std::string;

namespace graphic {

  namespace direct_x {

    class Shader {
    public:
      virtual bool compileFromFile(ID3D11Device*, string fileName, string funcName) = 0;
      virtual void shutdown() = 0;
      virtual void set(ID3D11DeviceContext*) = 0;

    protected:
      bool _compileFromFile(ID3D11Device*, string fileName, string funcName, ID3DBlob** shaderBlob);

      string              _target;
      utils::Logger*      log;
    };

  }
}
#endif // DIRECTX_SHADER_H_