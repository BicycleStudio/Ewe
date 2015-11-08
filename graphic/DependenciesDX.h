#ifndef DEPENDENCIES_DX_H_
#define DEPENDENCIES_DX_H_

// Safe disable dx default warnings
#pragma warning(push)
#pragma warning(disable:4005)
#include <D3DX11.h>
#include <D3D11.h>
#include <xnamath.h>
#include <d3dcompiler.h>
#pragma warning(pop)

#pragma comment (lib, "D3D11.lib")
#pragma comment (lib, "D3DX11.lib")

#endif // DEPENDENCIES_DX_H_