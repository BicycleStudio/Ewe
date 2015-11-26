#ifndef DIRECTX_BUFFER_H_
#define DIRECTX_BUFFER_H_

#include "DependenciesDX.h"
#include <Logger.h>
#include "DXVertex.h"

namespace graphic {

  namespace direct_x {

    class Buffer {
      utils::Logger* log;

    public:
      Buffer();
      ~Buffer();

      void shutdown();
      bool initialize(ID3D11Device*, D3D11_BUFFER_DESC&, D3D11_SUBRESOURCE_DATA&);
      bool initializeVertexBuffer(ID3D11Device*, void* verts, int vertSize, int countVerts);
	  bool initializeIndexBuffer(ID3D11Device*, UINT* inds, int countInds);
	  bool initializeConstantBuffer(ID3D11Device*, UINT scructSize);
      ID3D11Buffer* get();

    private:
      inline D3D11_SUBRESOURCE_DATA subres_data(void* verts);
      inline D3D11_BUFFER_DESC buffer_desc(int vertSize, int countVerts, DWORD flag);
      
      ID3D11Buffer*   _buffer;
    };

  }
}

#endif // DIRECTX_BUFFER_H_