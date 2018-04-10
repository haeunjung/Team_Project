#include "Vector3.h"
#include "Matrix.h"

WOOJUN_USING

// ==================== DxVector3 ====================
_tagDxVector3::_tagDxVector3()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;

}

_tagDxVector3::_tagDxVector3(float _x, float _y, float _z) :
	XMFLOAT3(_x, _y, _z)
{
}

_tagDxVector3::_tagDxVector3(float _f[3])
{
	x = _f[0];
	y = _f[1];
	z = _f[2];
}

_tagDxVector3::_tagDxVector3(int _i[3])
{
	x = (float)_i[0];
	y = (float)_i[1];
	z = (float)_i[2];
}

_tagDxVector3::_tagDxVector3(const _tagDxVector3& vec)
{
	*this = vec;
}

_tagDxVector3::_tagDxVector3(const _tagVector3 & vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
}

_tagDxVector3::_tagDxVector3(const XMFLOAT3 & vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
}

void _tagDxVector3::operator =(const _tagDxVector3& vec)
{	
	x = vec.x;
	y = vec.y;
	z = vec.z;
}

void _tagDxVector3::operator=(const _tagVector3 & vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
}

void _tagDxVector3::operator=(const XMFLOAT3 & vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
}

void _tagDxVector3::operator =(float _f[3])
{
	x = _f[0];
	y = _f[1];
	z = _f[2];
}

// +
_tagDxVector3 _tagDxVector3::operator+(const _tagDxVector3 & _v) const
{
	_tagDxVector3	v1;

	v1.x = x + _v.x;
	v1.y = y + _v.y;
	v1.z = z + _v.z;

	return v1;
}

_tagDxVector3 _tagDxVector3::operator+(float _f[3]) const
{
	_tagDxVector3	v1;

	v1.x = x + _f[0];
	v1.y = y + _f[1];
	v1.z = z + _f[2];

	return v1;
}

_tagDxVector3 _tagDxVector3::operator+(float fValue) const
{
	_tagDxVector3	v1;

	v1.x = x + fValue;
	v1.y = y + fValue;
	v1.z = z + fValue;

	return v1;
}

_tagDxVector3 _tagDxVector3::operator+(int iValue) const
{
	_tagDxVector3	v1;

	v1.x = x + iValue;
	v1.y = y + iValue;
	v1.z = z + iValue;

	return v1;
}

_tagDxVector3 _tagDxVector3::operator+(int i[3]) const
{
	_tagDxVector3	v1;

	v1.x = x + i[0];
	v1.y = y + i[1];
	v1.z = z + i[2];

	return v1;
}

void _tagDxVector3::operator+=(const _tagDxVector3 & _v)
{
	x += _v.x;
	y += _v.y;
	z += _v.z;
}

void _tagDxVector3::operator+=(float _f[3])
{
	x += _f[0];
	y += _f[1];
	z += _f[2];
}

void _tagDxVector3::operator+=(float fValue)
{
	x += fValue;
	y += fValue;
	z += fValue;
}

void _tagDxVector3::operator+=(int iValue)
{
	x += iValue;
	y += iValue;
	z += iValue;
}

void _tagDxVector3::operator+=(int i[3])
{
	x += i[0];
	y += i[1];
	z += i[2];
}

// -
_tagDxVector3 _tagDxVector3::operator-(const _tagDxVector3 & _v) const
{
	_tagDxVector3	v1;

	v1.x = x - _v.x;
	v1.y = y - _v.y;
	v1.z = z - _v.z;

	return v1;
}

_tagDxVector3 _tagDxVector3::operator-(float _f[3]) const
{
	_tagDxVector3	v1;

	v1.x = x - _f[0];
	v1.y = y - _f[1];
	v1.z = z - _f[2];

	return v1;
}

_tagDxVector3 _tagDxVector3::operator-(float fValue) const
{
	_tagDxVector3	v1;

	v1.x = x - fValue;
	v1.y = y - fValue;
	v1.z = z - fValue;

	return v1;
}

_tagDxVector3 _tagDxVector3::operator-(int iValue) const
{
	_tagDxVector3	v1;

	v1.x = x - iValue;
	v1.y = y - iValue;
	v1.z = z - iValue;

	return v1;
}

_tagDxVector3 _tagDxVector3::operator-(int i[3]) const
{
	_tagDxVector3	v1;

	v1.x = x - i[0];
	v1.y = y - i[1];
	v1.z = z - i[2];

	return v1;
}

void _tagDxVector3::operator-=(const _tagDxVector3 & _v)
{
	x -= _v.x;
	y -= _v.y;
	z -= _v.z;
}

void _tagDxVector3::operator-=(float _f[3])
{
	x -= _f[0];
	y -= _f[1];
	z -= _f[2];
}

void _tagDxVector3::operator-=(float fValue)
{
	x -= fValue;
	y -= fValue;
	z -= fValue;
}

void _tagDxVector3::operator-=(int iValue)
{
	x -= iValue;
	y -= iValue;
	z -= iValue;
}

void _tagDxVector3::operator-=(int i[3])
{
	x -= i[0];
	y -= i[1];
	z -= i[2];
}

// *
_tagDxVector3 _tagDxVector3::operator*(const _tagDxVector3 & _v) const
{
	_tagDxVector3	v1;

	v1.x = x * _v.x;
	v1.y = y * _v.y;
	v1.z = z * _v.z;

	return v1;
}

_tagDxVector3 _tagDxVector3::operator*(float _f[3]) const
{
	_tagDxVector3	v1;

	v1.x = x * _f[0];
	v1.y = y * _f[1];
	v1.z = z * _f[2];

	return v1;
}

_tagDxVector3 _tagDxVector3::operator*(float fValue) const
{
	_tagDxVector3	v1;

	v1.x = x * fValue;
	v1.y = y * fValue;
	v1.z = z * fValue;

	return v1;
}

_tagDxVector3 _tagDxVector3::operator*(int iValue) const
{
	_tagDxVector3	v1;

	v1.x = x * iValue;
	v1.y = y * iValue;
	v1.z = z * iValue;

	return v1;
}

_tagDxVector3 _tagDxVector3::operator*(int i[3]) const
{
	_tagDxVector3	v1;

	v1.x = x * i[0];
	v1.y = y * i[1];
	v1.z = z * i[2];

	return v1;
}

void _tagDxVector3::operator*=(const _tagDxVector3 & _v)
{
	x *= _v.x;
	y *= _v.y;
	z *= _v.z;
}

void _tagDxVector3::operator*=(float _f[3])
{
	x *= _f[0];
	y *= _f[1];
	z *= _f[2];
}

void _tagDxVector3::operator*=(float fValue)
{
	x *= fValue;
	y *= fValue;
	z *= fValue;
}

void _tagDxVector3::operator*=(int iValue)
{
	x *= iValue;
	y *= iValue;
	z *= iValue;
}

void _tagDxVector3::operator*=(int i[3])
{
	x *= i[0];
	y *= i[1];
	z *= i[2];
}

// /
_tagDxVector3 _tagDxVector3::operator/(const _tagDxVector3 & _v) const
{
	_tagDxVector3	v1;

	v1.x = x / _v.x;
	v1.y = y / _v.y;
	v1.z = z / _v.z;

	return v1;
}

_tagDxVector3 _tagDxVector3::operator/(float _f[3]) const
{
	_tagDxVector3	v1;

	v1.x = x / _f[0];
	v1.y = y / _f[1];
	v1.z = z / _f[2];

	return v1;
}

_tagDxVector3 _tagDxVector3::operator/(float fValue) const
{
	_tagDxVector3	v1;

	v1.x = x / fValue;
	v1.y = y / fValue;
	v1.z = z / fValue;

	return v1;
}

_tagDxVector3 _tagDxVector3::operator/(int iValue) const
{
	_tagDxVector3	v1;

	v1.x = x / iValue;
	v1.y = y / iValue;
	v1.z = z / iValue;

	return v1;
}

_tagDxVector3 _tagDxVector3::operator/(int i[3]) const
{
	_tagDxVector3	v1;

	v1.x = x / i[0];
	v1.y = y / i[1];
	v1.z = z / i[2];

	return v1;
}

void _tagDxVector3::operator/=(const _tagDxVector3 & _v)
{
	x /= _v.x;
	y /= _v.y;
	z /= _v.z;
}

void _tagDxVector3::operator/=(float _f[3])
{
	x /= _f[0];
	y /= _f[1];
	z /= _f[2];
}

void _tagDxVector3::operator/=(float fValue)
{
	x /= fValue;
	y /= fValue;
	z /= fValue;
}

void _tagDxVector3::operator/=(int iValue)
{
	x /= iValue;
	y /= iValue;
	z /= iValue;
}

void _tagDxVector3::operator/=(int i[3])
{
	x /= i[0];
	y /= i[1];
	z /= i[2];
}

void _tagDxVector3::operator++()
{
	++x;
	++y;
	++z;
}

void _tagDxVector3::operator--()
{
	--x;
	--y;
	--z;
}

// ==
bool _tagDxVector3::operator==(const _tagDxVector3 & _v) const
{
	return x == _v.x && y == _v.y && z == _v.z;
}

bool _tagDxVector3::operator==(float _f[3]) const
{
	return x == _f[0] && y == _f[1] && z == _f[2];
}

bool _tagDxVector3::operator==(int i[3]) const
{
	return x == i[0] && y == i[1] && z == i[2];
}

bool _tagDxVector3::operator!=(const _tagDxVector3 & _v) const
{
	return x != _v.x || y != _v.y || z != _v.z;
}

bool _tagDxVector3::operator!=(float _f[3]) const
{
	return x != _f[0] || y != _f[1] || z != _f[2];
}

bool _tagDxVector3::operator!=(int i[3]) const
{
	return x != i[0] || y != i[1] || z != i[2];
}

//float _tagDxVector3::operator[](int idx) const
//{
//	return f[idx];
//}

float _tagDxVector3::Length() const
{	
	return sqrtf(x * x + y * y + z * z);
}

float _tagDxVector3::Distance(const _tagDxVector3 & _v) const
{
	return (*this - _v).Length();
}

_tagDxVector3 _tagDxVector3::Normalize() const
{
	if (0 == Length())
	{
		return _tagDxVector3(0.0f, 0.0f, 0.0f);
	}
	return _tagDxVector3(x / Length(), y / Length(), z / Length());
}

float _tagDxVector3::Dot(const _tagDxVector3 & _v) const
{
	_tagDxVector3	v1 = *this * _v;
	return v1.x + v1.y + v1.z;
}

float _tagDxVector3::Dot(float _f[3]) const
{
	_tagDxVector3	v1 = *this * _f;
	return v1.x + v1.y + v1.z;
}

float _tagDxVector3::Dot(int i[3]) const
{
	_tagDxVector3	v1 = *this * i;
	return v1.x + v1.y + v1.z;
}

_tagDxVector3 _tagDxVector3::Cross(const _tagDxVector3 & _v) const
{
	XMVECTOR	v1 = Convert();
	XMVECTOR	v2 = _v.Convert();
	XMVECTOR	v3 = XMVector3Cross(v1, v2);

	_tagDxVector3 vResult = {};
	XMStoreFloat3((XMFLOAT3*)&vResult, v3);

	return vResult;
}

_tagDxVector3 _tagDxVector3::Cross(float _f[3]) const
{
	_tagDxVector3	_v = _f;
	return Cross(_v);
}

_tagDxVector3 _tagDxVector3::Cross(int _i[3]) const
{
	_tagDxVector3	_v = _i;
	return Cross(_v);
}

float _tagDxVector3::GetAngle(const _tagDxVector3 & _v) const
{
	return acosf(Dot(_v));
}

XMVECTOR _tagDxVector3::Convert() const
{
	// XMLoadFloat3 
	// XMFLOAT3를 XMVECTOR로 만들어주는 함수
	return XMLoadFloat3((XMFLOAT3*)this);
}

_tagDxVector3 _tagDxVector3::TransformNormal(const _tagDxMatrix & _mat) const
{
	_tagDxVector3 v1 = XMVector3TransformNormal(Convert(), _mat.mat);
	return v1;
}

_tagDxVector3 _tagDxVector3::TransformCoord(const _tagDxMatrix & _mat) const
{
	_tagDxVector3 v1 = XMVector3TransformCoord(Convert(), _mat.mat);
	return v1;
}

float _tagDxVector3::Max() const
{
	float Temp;
	if (x > y)
	{
		Temp = x;
	}
	else
	{
		Temp = y;
	}

	if (Temp > z)
	{
		return Temp;
	}
	else
	{
		return z;
	}
}

float _tagDxVector3::Min() const
{
	float Temp;
	if (x < y)
	{
		Temp = x;
	}
	else
	{
		Temp = y;
	}

	if (Temp < z)
	{
		return Temp;
	}
	else
	{
		return z;
	}
}


// ==================== Vector3 ====================
_tagVector3::_tagVector3() :
	x(0.f),
	y(0.f),
	z(0.f)
{
}

_tagVector3::_tagVector3(float _x, float _y, float _z) :
	x(_x),
	y(_y),
	z(_z)
{
}

_tagVector3::_tagVector3(float _f[3]) :
	x(_f[0]),
	y(_f[1]),
	z(_f[2])
{
}

_tagVector3::_tagVector3(const _tagVector3& vec)
{
	*this = vec;
}

_tagVector3::_tagVector3(const XMVECTOR & vec)
{
	v = vec;
}

_tagVector3::_tagVector3(const _tagDxVector3& vec)
{
	*this = vec;
}

void _tagVector3::operator =(const _tagVector3& vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
}

void _tagVector3::operator=(const XMVECTOR & vec)
{
	v = vec;
}

void _tagVector3::operator =(const _tagDxVector3& vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
}

void _tagVector3::operator =(float _f[3])
{
	x = _f[0];
	y = _f[1];
	z = _f[2];
}

// +
_tagVector3 _tagVector3::operator+(const _tagDxVector3 & _v) const
{
	_tagVector3	v1;

	v1.x = x + _v.x;
	v1.y = y + _v.y;
	v1.z = z + _v.z;

	return v1;
}

_tagVector3 _tagVector3::operator+(const _tagVector3 & _v) const
{
	_tagVector3	v1;

	v1.v = v + _v.v;

	return v1;
}

_tagVector3 _tagVector3::operator+(float _f[3]) const
{
	_tagVector3	v1;

	v1.x = x + _f[0];
	v1.y = y + _f[1];
	v1.z = z + _f[2];

	return v1;
}

_tagVector3 _tagVector3::operator+(float fValue) const
{
	_tagVector3	v1;

	v1.x = x + fValue;
	v1.y = y + fValue;
	v1.z = z + fValue;

	return v1;
}

_tagVector3 _tagVector3::operator+(int iValue) const
{
	_tagVector3	v1;

	v1.x = x + iValue;
	v1.y = y + iValue;
	v1.z = z + iValue;

	return v1;
}

_tagVector3 _tagVector3::operator+(int i[3]) const
{
	_tagVector3	v1;

	v1.x = x + i[0];
	v1.y = y + i[1];
	v1.z = z + i[2];

	return v1;
}

void _tagVector3::operator+=(const _tagDxVector3 & _v)
{
	x += _v.x;
	y += _v.y;
	z += _v.z;
}

void _tagVector3::operator+=(const _tagVector3 & _v)
{
	v += _v.v;
}

void _tagVector3::operator+=(float _f[3])
{
	x += _f[0];
	y += _f[1];
	z += _f[2];
}

void _tagVector3::operator+=(float fValue)
{
	x += fValue;
	y += fValue;
	z += fValue;
}

void _tagVector3::operator+=(int iValue)
{
	x += iValue;
	y += iValue;
	z += iValue;
}

void _tagVector3::operator+=(int i[3])
{
	x += i[0];
	y += i[1];
	z += i[2];
}

// -
_tagVector3 _tagVector3::operator-(const _tagDxVector3 & _v) const
{
	_tagVector3	v1;

	v1.x = x - _v.x;
	v1.y = y - _v.y;
	v1.z = z - _v.z;

	return v1;
}

_tagVector3 _tagVector3::operator-(const _tagVector3 & _v) const
{
	_tagVector3	v1;

	v1.v = v - _v.v;

	return v1;
}


_tagVector3 _tagVector3::operator-(float _f[3]) const
{
	_tagVector3	v1;

	v1.x = x - _f[0];
	v1.y = y - _f[1];
	v1.z = z - _f[2];

	return v1;
}

_tagVector3 _tagVector3::operator-(float fValue) const
{
	_tagVector3	v1;

	v1.x = x - fValue;
	v1.y = y - fValue;
	v1.z = z - fValue;

	return v1;
}

_tagVector3 _tagVector3::operator-(int iValue) const
{
	_tagVector3	v1;

	v1.x = x - iValue;
	v1.y = y - iValue;
	v1.z = z - iValue;

	return v1;
}

_tagVector3 _tagVector3::operator-(int i[3]) const
{
	_tagVector3	v1;

	v1.x = x - i[0];
	v1.y = y - i[1];
	v1.z = z - i[2];

	return v1;
}

void _tagVector3::operator-=(const _tagDxVector3 & _v)
{
	x -= _v.x;
	y -= _v.y;
	z -= _v.z;
}

void _tagVector3::operator-=(const _tagVector3 & _v)
{
	v -= _v.v;
}

void _tagVector3::operator-=(float _f[3])
{
	x -= _f[0];
	y -= _f[1];
	z -= _f[2];
}

void _tagVector3::operator-=(float fValue)
{
	x -= fValue;
	y -= fValue;
	z -= fValue;
}

void _tagVector3::operator-=(int iValue)
{
	x -= iValue;
	y -= iValue;
	z -= iValue;
}

void _tagVector3::operator-=(int i[3])
{
	x -= i[0];
	y -= i[1];
	z -= i[2];
}

// *
_tagVector3 _tagVector3::operator*(const _tagDxVector3 & _v) const
{
	_tagVector3	v1;

	v1.x = x * _v.x;
	v1.y = y * _v.y;
	v1.z = z * _v.z;

	return v1;
}

_tagVector3 _tagVector3::operator*(const _tagVector3 & _v) const
{
	_tagVector3	v1;

	v1.v = v * _v.v;

	return v1;
}


_tagVector3 _tagVector3::operator*(float _f[3]) const
{
	_tagVector3	v1;

	v1.x = x * _f[0];
	v1.y = y * _f[1];
	v1.z = z * _f[2];

	return v1;
}

_tagVector3 _tagVector3::operator*(float fValue) const
{
	_tagVector3	v1;

	v1.x = x * fValue;
	v1.y = y * fValue;
	v1.z = z * fValue;

	return v1;
}

_tagVector3 _tagVector3::operator*(int iValue) const
{
	_tagVector3	v1;

	v1.x = x * iValue;
	v1.y = y * iValue;
	v1.z = z * iValue;

	return v1;
}

_tagVector3 _tagVector3::operator*(int i[3]) const
{
	_tagVector3	v1;

	v1.x = x * i[0];
	v1.y = y * i[1];
	v1.z = z * i[2];

	return v1;
}

void _tagVector3::operator*=(const _tagDxVector3 & _v)
{
	x *= _v.x;
	y *= _v.y;
	z *= _v.z;
}

void _tagVector3::operator*=(const _tagVector3 & _v)
{
	v *= _v.v;
}

void _tagVector3::operator*=(float _f[3])
{
	x *= _f[0];
	y *= _f[1];
	z *= _f[2];
}

void _tagVector3::operator*=(float fValue)
{
	x *= fValue;
	y *= fValue;
	z *= fValue;
}

void _tagVector3::operator*=(int iValue)
{
	x *= iValue;
	y *= iValue;
	z *= iValue;
}

void _tagVector3::operator*=(int i[3])
{
	x *= i[0];
	y *= i[1];
	z *= i[2];
}

// /
_tagVector3 _tagVector3::operator/(const _tagDxVector3 & _v) const
{
	_tagVector3	v1;

	v1.x = x / _v.x;
	v1.y = y / _v.y;
	v1.z = z / _v.z;

	return v1;
}

_tagVector3 _tagVector3::operator/(const _tagVector3 & _v) const
{
	_tagVector3	v1;

	v1.v = v / _v.v;

	return v1;
}

_tagVector3 _tagVector3::operator/(float _f[3]) const
{
	_tagVector3	v1;

	v1.x = x / _f[0];
	v1.y = y / _f[1];
	v1.z = z / _f[2];

	return v1;
}

_tagVector3 _tagVector3::operator/(float fValue) const
{
	_tagVector3	v1;

	v1.x = x / fValue;
	v1.y = y / fValue;
	v1.z = z / fValue;

	return v1;
}

_tagVector3 _tagVector3::operator/(int iValue) const
{
	_tagVector3	v1;

	v1.x = x / iValue;
	v1.y = y / iValue;
	v1.z = z / iValue;

	return v1;
}

_tagVector3 _tagVector3::operator/(int i[3]) const
{
	_tagVector3	v1;

	v1.x = x / i[0];
	v1.y = y / i[1];
	v1.z = z / i[2];

	return v1;
}

void _tagVector3::operator/=(const _tagDxVector3 & _v)
{
	x /= _v.x;
	y /= _v.y;
	z /= _v.z;
}

void _tagVector3::operator/=(const _tagVector3 & _v)
{
	v /= _v.v;
}

void _tagVector3::operator/=(float _f[3])
{
	x /= _f[0];
	y /= _f[1];
	z /= _f[2];
}

void _tagVector3::operator/=(float fValue)
{
	x /= fValue;
	y /= fValue;
	z /= fValue;
}

void _tagVector3::operator/=(int iValue)
{
	x /= iValue;
	y /= iValue;
	z /= iValue;
}

void _tagVector3::operator/=(int i[3])
{
	x /= i[0];
	y /= i[1];
	z /= i[2];
}

void _tagVector3::operator++()
{
	++x;
	++y;
	++z;
}

void _tagVector3::operator--()
{
	--x;
	--y;
	--z;
}

// ==
bool _tagVector3::operator==(const _tagDxVector3 & _v) const
{
	return x == _v.x && y == _v.y && z == _v.z;
}

bool _tagVector3::operator==(const _tagVector3 & _v) const
{
	return x == _v.x && y == _v.y && z == _v.z;
}

bool _tagVector3::operator==(float _f[3]) const
{
	return x == _f[0] && y == _f[1] && z == _f[2];
}

bool _tagVector3::operator==(int i[3]) const
{
	return x == i[0] && y == i[1] && z == i[2];
}

// !=
bool _tagVector3::operator!=(const _tagDxVector3 & _v) const
{
	return x != _v.x || y != _v.y || z != _v.z;
}

bool _tagVector3::operator!=(const _tagVector3 & _v) const
{
	return x != _v.x || y != _v.y || z != _v.z;
}

bool _tagVector3::operator!=(float _f[3]) const
{
	return x != _f[0] || y != _f[1] || z != _f[2];
}

bool _tagVector3::operator!=(int i[3]) const
{
	return x != i[0] || y != i[1] || z != i[2];
}

float _tagVector3::operator[](int idx) const
{
	return f[idx];
}

float _tagVector3::Length() const
{
	_tagVector3	v1;
	v1.v = XMVector3Length(v);
	return v1.x;
}

_tagVector3 _tagVector3::Normalize() const
{
	_tagVector3 v1;
	v1.v = XMVector3Normalize(v);
	return v1;
}

float _tagVector3::Dot(const _tagDxVector3 & _v) const
{
	_tagVector3	v1 = *this * _v;
	return v1.x + v1.y + v1.z;
}

float _tagVector3::Dot(const _tagVector3 & _v) const
{
	_tagVector3	v1;
	v1.v = XMVector3Dot(v, _v.v);
	return v1.x;
}

float _tagVector3::Dot(float _f[3]) const
{
	_tagVector3	v1 = *this * _f;
	return v1.x + v1.y + v1.z;
}

float _tagVector3::Dot(int i[3]) const
{
	_tagVector3	v1 = *this * i;
	return v1.x + v1.y + v1.z;
}

_tagVector3 _tagVector3::Cross(const _tagDxVector3 & _v) const
{
	return _tagVector3(y * _v.z - z * _v.y, z * _v.x - x * _v.z, x * _v.y - y * _v.x);
}

_tagVector3 _tagVector3::Cross(const _tagVector3 & _v) const
{
	_tagVector3	v1;
	v1.v = XMVector3Cross(v, _v.v);
	return v1;
}

_tagVector3 _tagVector3::Cross(float _f[3]) const
{
	return _tagVector3(y * _f[2] - z * _f[1], z * _f[0] - x * _f[2], x * _f[1] - y * _f[0]);
}

_tagVector3 _tagVector3::Cross(int i[3]) const
{
	return _tagVector3(y * i[2] - z * i[1], z * i[0] - x * i[2], x * i[1] - y * i[0]);
}

_tagVector3 _tagVector3::TransformNormal(const _tagDxMatrix & mat)
{
	_tagVector3 v1 = XMVector3TransformNormal(v, mat.mat);
	return v1;
}

_tagVector3 _tagVector3::TransformCoord(const _tagDxMatrix & mat)
{
	_tagVector3 v1 = XMVector3TransformCoord(v, mat.mat);
	return v1;
}
