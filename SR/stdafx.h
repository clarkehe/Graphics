// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifdef _MSC_VER

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <Windows.h>
#include <tchar.h>

#endif

#include <stdio.h>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <assert.h>

// TODO: reference additional headers your program requires here
#include "Vector2D.hpp"
#include "Vector3D.hpp"
#include "Vector4D.hpp"
#include "Matrix2x2.hpp"
#include "Matrix4x4.hpp"
#include "Color.hpp"

typedef Vector2D Vec2f;
typedef Vector3D Vec3f;
typedef Vector3D Vec3;
typedef Vector4D Vec4f;
typedef Matrix4x4 Matrix4;
typedef Matrix4x4 Mat4f;

#include "Light.h"
#include "Mesh.h"
#include "Texture.h"
#include "Vertex.h"
#include "slm.h"

using namespace SoftRender;
