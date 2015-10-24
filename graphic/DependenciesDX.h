#ifndef DEPENDENCIES_DX_H_
#define DEPENDENCIES_DX_H_

// Safe disable dx default warnings
#pragma warning(push)
#pragma warning(disable:4005)
#include <D3DX11.h>
#include <D3Dcompiler.h>
#include <xnamath.h>
#pragma warning(pop)

#pragma comment (lib, "D3DX11.lib")
#pragma comment (lib, "D3D11.lib")

#endif // DEPENDENCIES_DX_H_