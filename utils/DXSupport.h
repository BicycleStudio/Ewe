#ifndef DX_SUPPORT_H_
#define DX_SUPPORT_H_

namespace direct_x {
  #define CHECK_HRESULT(hres,logAction)   { if(FAILED(hres)) { logAction; return false; } }
  #define SAFE_RELEASE(d3dPonter)         { if(d3dPonter) { d3dPonter->Release(); d3dPonter = 0; } }
}

#endif // DX_SUPPORT_H_