#ifndef DIRECTX_SHADER_H_
#define DIRECTX_SHADER_H_

#include "DependenciesDX.h"
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
      Shader();

      string              _target;
      utils::Logger*      log;
		};

    class VShader : public Shader {
    public:
      VShader();
      ~VShader();

      bool compileFromFile(ID3D11Device*, string fileName, string funcName);
      void shutdown();
      bool createInputLayout(ID3D11Device*, const D3D11_INPUT_ELEMENT_DESC*, UINT arraySize, ID3D11InputLayout**);
      void set(ID3D11DeviceContext*);

    private:
      ID3D11VertexShader*   _shader;
      ID3DBlob*             _shaderBlob;
    };

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
#endif // DIRECTX_SHADER_H_