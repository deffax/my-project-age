#pragma once
//========================================================================
// Geometry.h : Collection of code for 3D math and 3D shapes
//



const float  GCC_PI = 3.14159265358979f;
const float  GCC_2PI = 2 * GCC_PI;

////////////////////////////////////////////////////
//
// Utility classes for vectors and matrices 
//
////////////////////////////////////////////////////

typedef D3DXVECTOR2 Vec2;

////////////////////////////////////////////////////
//
// Vec3 Description
//
//
////////////////////////////////////////////////////

class Vec3 : public D3DXVECTOR3 
{
public:
	inline float Length() { return D3DXVec3Length(this); }
	inline Vec3 *Normalize() { return static_cast<Vec3 *>(D3DXVec3Normalize(this, this)); }
	inline float Dot(const Vec3 &b) { return D3DXVec3Dot(this, &b); }
	inline Vec3 Cross(const Vec3 &b) const;

	Vec3(D3DXVECTOR3 &v3) { x = v3.x; y = v3.y; z = v3.z; }
	Vec3() : D3DXVECTOR3() { x = 0; y = 0; z = 0; }
	Vec3(const float _x, const float _y, const float _z) { x=_x; y=_y; z=_z; }
    Vec3(const double _x, const double _y, const double _z) { x = (float)_x; y = (float)_y; z = (float)_z; }
	inline Vec3(const class Vec4 &v4);
};

inline Vec3 Vec3::Cross(const Vec3 &b) const
{
	Vec3 out;
	D3DXVec3Cross(&out, this, &b);
	return out;
}



////////////////////////////////////////////////////
//
// Vec4 Description
//
//
////////////////////////////////////////////////////


class Vec4 : public D3DXVECTOR4
{
public:
	inline float Length() { return D3DXVec4Length(this); }
	inline Vec4 *Normalize() { return static_cast<Vec4 *>(D3DXVec4Normalize(this, this)); }
	// If you want the cross product, use Vec3::Cross
	inline float Dot(const Vec4 &b) { return D3DXVec4Dot(this, &b); }

	Vec4(D3DXVECTOR4 &v4)  { x = v4.x; y = v4.y; z = v4.z; w = v4.w; }
	Vec4() : D3DXVECTOR4() { }
	Vec4(const float _x, const float _y, const float _z, const float _w) { x=_x; y=_y; z=_z; w=_w; }
	Vec4(const Vec3 &v3) { x = v3.x; y = v3.y; z = v3.z; w = 1.0f; }

};

inline Vec3::Vec3(const Vec4 &v4) { x = v4.x; y = v4.y; z = v4.z; }

extern Vec3 g_Up;
extern Vec3 g_Right;
extern Vec3 g_Forward;

extern Vec4 g_Up4;
extern Vec4 g_Right4;
extern Vec4 g_Forward4;


////////////////////////////////////////////////////
//
// Vec3List Description
// Vec4List Description
//
//   An STL list of Vectors
//
////////////////////////////////////////////////////

typedef std::list<Vec3> Vec3List;
typedef std::list<Vec4> Vec4List;


////////////////////////////////////////////////////
//
// Quaternion Description
//
//
////////////////////////////////////////////////////




class Quaternion : public D3DXQUATERNION
{
public:

	// Modifiers
	void Normalize() { D3DXQuaternionNormalize(this, this); };
	void Slerp(const Quaternion &begin, const Quaternion &end, float cooef)
	{
		// performs spherical linear interpolation between begin & end 
		// NOTE: set cooef between 0.0f-1.0f
		D3DXQuaternionSlerp(this, &begin, &end, cooef);
	}

	// Accessors
	void GetAxisAngle(Vec3 &axis, float &angle) const
	{
		D3DXQuaternionToAxisAngle(this, &axis, &angle); 
	}

	// Initializers
	void Build(const class Mat4x4 &mat); 

	void BuildRotYawPitchRoll(
			const float yawRadians, 
			const float pitchRadians, 
			const float rollRadians)
	{
		D3DXQuaternionRotationYawPitchRoll(this, yawRadians, pitchRadians, rollRadians);
	}

	void BuildAxisAngle(const Vec3 &axis, const float radians)
	{
		D3DXQuaternionRotationAxis(this, &axis, radians);
	}
	
	Quaternion(D3DXQUATERNION &q) : D3DXQUATERNION(q) { }
	Quaternion() { }

	static const Quaternion g_Identity;
};

inline Quaternion operator * (const Quaternion &a, const Quaternion &b) 
{
	// for rotations, this is exactly like concatenating
	// matrices - the new quat represents rot A followed by rot B.
	Quaternion out;
	D3DXQuaternionMultiply(&out, &a, &b);
	return out;
}



////////////////////////////////////////////////////
//
// Mat4x4 Description
//
//
////////////////////////////////////////////////////



class Mat4x4 : public D3DXMATRIX
{
public:
	// Modifiers
	inline void SetPosition(Vec3 const &pos);
	inline void SetPosition(Vec4 const &pos);
	inline void SetScale(Vec3 const &scale);

	// Accessors and Calculation Methods
	inline Vec3 GetPosition() const;
	inline Vec3 GetDirection() const;
	inline Vec3 GetUp() const;
	inline Vec3 GetRight() const;
	inline Vec3 GetYawPitchRoll() const;
	inline Vec3 GetScale() const;
	inline Vec4 Xform(Vec4 &v) const;
	inline Vec3 Xform(Vec3 &v) const;
	inline Mat4x4 Inverse() const;

	Mat4x4(D3DXMATRIX &mat) { memcpy(&m, &mat.m, sizeof(mat.m)); }

	Mat4x4() : D3DXMATRIX() { }

	static const Mat4x4 g_Identity;

	// Initialization methods
	inline void BuildTranslation(const Vec3 &pos);
	inline void BuildTranslation(const float x, const float y, const float z );
	inline void BuildRotationX(const float radians) { D3DXMatrixRotationX(this, radians); }
	inline void BuildRotationY(const float radians) { D3DXMatrixRotationY(this, radians); }
	inline void BuildRotationZ(const float radians) { D3DXMatrixRotationZ(this, radians); }
	inline void BuildYawPitchRoll(const float yawRadians, const float pitchRadians, const float rollRadians)
		{ D3DXMatrixRotationYawPitchRoll(this, yawRadians, pitchRadians, rollRadians); }
	inline void BuildRotationQuat(const Quaternion &q) { D3DXMatrixRotationQuaternion(this, &q); }
	inline void BuildRotationLookAt(const Vec3 &eye, const Vec3 &at, const Vec3 &up) { D3DXMatrixLookAtRH(this, &eye, &at, &up); }
	inline void BuildScale(const float x, const float y, const float z );
};



inline void Mat4x4::SetPosition(Vec3 const &pos)
{
	m[3][0] = pos.x;
	m[3][1] = pos.y;
	m[3][2] = pos.z;
	m[3][3] = 1.0f;
}

inline void Mat4x4::SetPosition(Vec4 const &pos)
{
	m[3][0] = pos.x;
	m[3][1] = pos.y;
	m[3][2] = pos.z;
	m[3][3] = pos.w;
}

inline void Mat4x4::SetScale(Vec3 const &scale)
{
	m[1][1] = scale.x;
	m[2][2] = scale.y;
	m[3][3] = scale.z;
}


inline Vec3 Mat4x4::GetPosition() const
{
	return Vec3(m[3][0], m[3][1], m[3][2]);
}

inline Vec3 Mat4x4::GetDirection() const
{
	// Note - the following code can be used to double check the vector construction above.
	Mat4x4 justRot = *this;
	justRot.SetPosition(Vec3(0.f,0.f,0.f));
	Vec3 forward = justRot.Xform(g_Forward);
	return forward;
}

inline Vec3 Mat4x4::GetRight() const
{
	// Note - the following code can be used to double check the vector construction above.
	Mat4x4 justRot = *this;
	justRot.SetPosition(Vec3(0.f,0.f,0.f));
	Vec3 right = justRot.Xform(g_Right);
	return right;
}

inline Vec3 Mat4x4::GetUp() const
{
	// Note - the following code can be used to double check the vector construction above.
	Mat4x4 justRot = *this;
	justRot.SetPosition(Vec3(0.f,0.f,0.f));
	Vec3 up = justRot.Xform(g_Up);
	return up;
}




inline Vec3 Mat4x4::GetYawPitchRoll() const
{
   float yaw, pitch, roll;
	
   pitch = asin(-_32);

   double threshold = 0.001; // Hardcoded constant - burn him, he's a witch
   double test = cos(pitch);

   if(test > threshold) 
   {
      roll = atan2(_12, _22);
      yaw = atan2(_31, _33);
   }
   else 
   {
      roll = atan2(-_21, _11);
      yaw = 0.0;
   }

	return (Vec3(yaw, pitch, roll));
}


inline Vec3 Mat4x4::GetScale() const
{
	return Vec3(m[0][0], m[1][1], m[2][2]);
}

inline Vec4 Mat4x4::Xform(Vec4 &v) const
{
	Vec4 temp;
	D3DXVec4Transform(&temp, &v, this);
	return temp;
}

inline Vec3 Mat4x4::Xform(Vec3 &v) const
{
	Vec4 temp(v);
	Vec4 out;
	D3DXVec4Transform(&out, &temp, this);
	return Vec3(out.x, out.y, out.z);
}

inline Mat4x4 Mat4x4::Inverse() const
{
	Mat4x4 out;
	D3DXMatrixInverse(&out, NULL, this);
	return out;
}

inline void Mat4x4::BuildTranslation(const Vec3 &pos)
{
	*this = Mat4x4::g_Identity;
	m[3][0] = pos.x;
	m[3][1] = pos.y;
	m[3][2] = pos.z;
}

inline void Mat4x4::BuildTranslation(const float x, const float y, const float z )
{
	*this = Mat4x4::g_Identity;
	m[3][0] = x;
	m[3][1] = y;
	m[3][2] = z;
}

inline void Mat4x4::BuildScale(const float x, const float y, const float z )
{
	*this = Mat4x4::g_Identity;
	m[1][1] = x;
	m[2][2] = y;
	m[3][3] = z;
}


inline Mat4x4 operator * (const Mat4x4 &a, const Mat4x4 &b) 
{
	Mat4x4 out;
	D3DXMatrixMultiply(&out, &a, &b);

	return out;
}

inline void Quaternion::Build(const Mat4x4 &mat) 
{
	D3DXQuaternionRotationMatrix(this, &mat); 
}


////////////////////////////////////////////////////
//
// D3D9 Vertex Type Definitions
//
//  TRANSFORMED_VERTEX Description
//  UNTRANSFORMED_VERTEX Description
//  UNTRANSFORMED_LIT_VERTEX Description
//  D3D9Vertex_UnlitColored Description
//  D3D9Vertex_ColoredTextured Description
//  D3D9Vertex_Colored Description
//
//  Note: There's been a slight change from the book in this code.
//        Instead of #define D3DFVF_BlahBlah they are static constants;
//        find them at the top of Geometry.cpp
//
// See Game Coding Complete, 3rd Edition, Chapter 13, page 440-444
////////////////////////////////////////////////////

// A structure for our custom vertex type. We added texture coordinates
struct D3D9Vertex_Colored
{
    D3DXVECTOR3 position; // The position
    D3DCOLOR    color;    // The color

	static const DWORD FVF;
};



