#ifndef DEPENDENCIES_DX_H_
#define DEPENDENCIES_DX_H_

// Safe disable dx default warnings
#pragma warning(push)
#pragma warning(disable:4005) // allows to define macros twice

#include <D3D11.h>
#include <DirectXMath.h>

using namespace DirectX;

#pragma warning(pop)

#pragma comment (lib, "D3D11.lib")

#endif // DEPENDENCIES_DX_H_