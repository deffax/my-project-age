#include "GameCodeStd.h"
#include "Geometry.h"

const Mat4x4 Mat4x4::g_Identity(D3DXMATRIX(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1));
const Quaternion Quaternion::g_Identity(D3DXQUATERNION(0,0,0,1));


const DWORD D3D9Vertex_Colored::FVF = (D3DFVF_XYZ|D3DFVF_DIFFUSE);