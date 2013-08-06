#pragma once

class Vec3 : public D3DXVECTOR3
{
public:
	inline float Length() {return D3DXVec3Length(this);}
	inline Vec3* Normalize() 
	{
		return static_cast<Vec3*>(D3DXVec3Normalize(this, this));
	}
	inline float Dot(const Vec3& b) {return D3DXVec3Dot(this, &b);}
	inline Vec3 Cross(const Vec3& b) const
	{
		Vec3 out;
		D3DXVec3Cross(&out, this, &b);
		return out;
	}

	Vec3(D3DXVECTOR3& v3)
	{
		x = v3.x;
		y = v3.y;
		z = v3.z;
	}

	Vec3() : D3DXVECTOR3()
	{
		x = 0;
		y = 0;
		z = 0;
	}

	Vec3(const float _x, const float _y, const float _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}

	Vec3(const double _x, const double _y, const double _z)
	{
		x = (float)_x;
		y = (float)_y;
		z = (float)_z;
	}

};