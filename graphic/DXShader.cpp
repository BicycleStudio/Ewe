#include "DXShader.h"
#include "DXSupport.h"

#include <d3dcompiler.h>
#pragma comment (lib, "d3dcompiler.lib")

using namespace graphic::direct_x;
using namespace utils::direct_x;

bool Shader::_compileFromFile(ID3D11Device* device, string fileName, string funcName, ID3DBlob** shaderBlob) {
  UINT flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
#if defined( DEBUG ) || defined( _DEBUG )
  flags |= D3DCOMPILE_DEBUG;
#endif
  ID3DBlob* errorBlob = NULL;

  size_t size = strlen(fileName.c_str()) + 1;
  WCHAR* wFilename = new WCHAR[size];
  size_t outSize;
  mbstowcs_s(&outSize, wFilename, size, fileName.c_str(), size - 1);

  HRESULT hr = D3DCompileFromFile(wFilename, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE,
    funcName.c_str(), _target.c_str(), flags, 0, shaderBlob, &errorBlob);
  if (FAILED(hr)) {
    if (errorBlob != NULL) {
      OutputDebugStringA((char*)errorBlob->GetBufferPointer());
      log->error(reinterpret_cast<const char*>(errorBlob->GetBufferPointer()));
    }
    else
      log->error("Path "+fileName+" is wrong.");

    if (errorBlob) errorBlob->Release();
    return false;
  }
  if (errorBlob) errorBlob->Release();
  return true;
}
