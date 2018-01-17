#include "Vector4.h"
#include "Matrix.h"

WOOJUN_USING

// ==================== DxVector4 ====================
_tagDxVector4::_tagDxVector4()	
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	w = 0.0f;
}

_tagDxVector4::_tagDxVector4(float _x, float _y, float _z, float _w) :
	XMFLOAT4(_x, _y, _z, _w)
{
}

_tagDxVector4::_tagDxVector4(float _f[4])	
{
	x = _f[0];
	y = _f[1];
	z = _f[2];
	w = _f[3];
}

_tagDxVector4::_tagDxVector4(int _i[4])
{
	x = (float)_i[0];
	y = (float)_i[1];
	z = (float)_i[2];
	w = (float)_i[3];
}

_tagDxVector4::_tagDxVector4(const _tagDxVector4& vec)
{
	*this = vec;
}

void _tagDxVector4::operator =(const _tagDxVector4& vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
	w = vec.w;
}

void _tagDxVector4::operator =(float _f[4])
{
	x = _f[0];
	y = _f[1];
	z = _f[2];
	w = _f[3];
}

void _tagDxVector4::operator=(int _i[4])
{
	x = (float)_i[0];
	y = (float)_i[1];
	z = (float)_i[2];
	w = (float)_i[3];

}

// +
_tagDxVector4 _tagDxVector4::operator+(const _tagDxVector4 & _v) const
{
	_tagDxVector4	v1;

	v1.x = x + _v.x;
	v1.y = y + _v.y;
	v1.z = z + _v.z;
	v1.w = w + _v.w;

	return v1;
}

_tagDxVector4 _tagDxVector4::operator+(float _f[4]) const
{
	_tagDxVector4	v1;

	v1.x = x + _f[0];
	v1.y = y + _f[1];
	v1.z = z + _f[2];
	v1.w = w + _f[3];

	return v1;
}

_tagDxVector4 _tagDxVector4::operator+(float fValue) const
{
	_tagDxVector4	v1;

	v1.x = x + fValue;
	v1.y = y + fValue;
	v1.z = z + fValue;
	v1.w = w + fValue;

	return v1;
}

_tagDxVector4 _tagDxVector4::operator+(int iValue) const
{
	_tagDxVector4	v1;

	v1.x = x + iValue;
	v1.y = y + iValue;
	v1.z = z + iValue;
	v1.w = w + iValue;

	return v1;
}

_tagDxVector4 _tagDxVector4::operator+(int i[4]) const
{
	_tagDxVector4	v1;

	v1.x = x + i[0];
	v1.y = y + i[1];
	v1.z = z + i[2];
	v1.w = w + i[3];

	return v1;
}

void _tagDxVector4::operator+=(const _tagDxVector4 & _v)
{
	x += _v.x;
	y += _v.y;
	z += _v.z;
	w += _v.w;
}

void _tagDxVector4::operator+=(float _f[4])
{
	x += _f[0];
	y += _f[1];
	z += _f[2];
	w += _f[3];
}

void _tagDxVector4::operator+=(float fValue)
{
	x += fValue;
	y += fValue;
	z += fValue;
	w += fValue;
}

void _tagDxVector4::operator+=(int iValue)
{
	x += iValue;
	y += iValue;
	z += iValue;
	w += iValue;
}

void _tagDxVector4::operator+=(int i[4])
{
	x += i[0];
	y += i[1];
	z += i[2];
	w += i[3];
}

// -
_tagDxVector4 _tagDxVector4::operator-(const _tagDxVector4 & _v) const
{
	_tagDxVector4	v1;

	v1.x = x - _v.x;
	v1.y = y - _v.y;
	v1.z = z - _v.z;
	v1.w = w - _v.w;

	return v1;
}

_tagDxVector4 _tagDxVector4::operator-(float _f[4]) const
{
	_tagDxVector4	v1;

	v1.x = x - _f[0];
	v1.y = y - _f[1];
	v1.z = z - _f[2];
	v1.w = w - _f[3];

	return v1;
}

_tagDxVector4 _tagDxVector4::operator-(float fValue) const
{
	_tagDxVector4	v1;

	v1.x = x - fValue;
	v1.y = y - fValue;
	v1.z = z - fValue;
	v1.w = w - fValue;

	return v1;
}

_tagDxVector4 _tagDxVector4::operator-(int iValue) const
{
	_tagDxVector4	v1;

	v1.x = x - iValue;
	v1.y = y - iValue;
	v1.z = z - iValue;
	v1.w = w - iValue;

	return v1;
}

_tagDxVector4 _tagDxVector4::operator-(int i[4]) const
{
	_tagDxVector4	v1;

	v1.x = x - i[0];
	v1.y = y - i[1];
	v1.z = z - i[2];
	v1.w = w - i[3];

	return v1;
}

void _tagDxVector4::operator-=(const _tagDxVector4 & _v)
{
	x -= _v.x;
	y -= _v.y;
	z -= _v.z;
	w -= _v.w;
}

void _tagDxVector4::operator-=(float _f[4])
{
	x -= _f[0];
	y -= _f[1];
	z -= _f[2];
	w -= _f[3];
}

void _tagDxVector4::operator-=(float fValue)
{
	x -= fValue;
	y -= fValue;
	z -= fValue;
	w -= fValue;
}

void _tagDxVector4::operator-=(int iValue)
{
	x -= iValue;
	y -= iValue;
	z -= iValue;
	w -= iValue;
}

void _tagDxVector4::operator-=(int i[4])
{
	x -= i[0];
	y -= i[1];
	z -= i[2];
	w -= i[3];
}

// *
_tagDxVector4 _tagDxVector4::operator*(const _tagDxVector4 & _v) const
{
	_tagDxVector4	v1;

	v1.x = x * _v.x;
	v1.y = y * _v.y;
	v1.z = z * _v.z;
	v1.w = w * _v.w;

	return v1;
}

_tagDxVector4 _tagDxVector4::operator*(float _f[4]) const
{
	_tagDxVector4	v1;

	v1.x = x * _f[0];
	v1.y = y * _f[1];
	v1.z = z * _f[2];
	v1.w = w * _f[3];

	return v1;
}

_tagDxVector4 _tagDxVector4::operator*(float fValue) const
{
	_tagDxVector4	v1;

	v1.x = x * fValue;
	v1.y = y * fValue;
	v1.z = z * fValue;
	v1.w = w * fValue;

	return v1;
}

_tagDxVector4 _tagDxVector4::operator*(int iValue) const
{
	_tagDxVector4	v1;

	v1.x = x * iValue;
	v1.y = y * iValue;
	v1.z = z * iValue;
	v1.w = w * iValue;

	return v1;
}

_tagDxVector4 _tagDxVector4::operator*(int i[4]) const
{
	_tagDxVector4	v1;

	v1.x = x * i[0];
	v1.y = y * i[1];
	v1.z = z * i[2];
	v1.w = w * i[3];

	return v1;
}

void _tagDxVector4::operator*=(const _tagDxVector4 & _v)
{
	x *= _v.x;
	y *= _v.y;
	z *= _v.z;
	w *= _v.w;
}

void _tagDxVector4::operator*=(float _f[4])
{
	x *= _f[0];
	y *= _f[1];
	z *= _f[2];
	w *= _f[3];
}

void _tagDxVector4::operator*=(float fValue)
{
	x *= fValue;
	y *= fValue;
	z *= fValue;
	w *= fValue;
}

void _tagDxVector4::operator*=(int iValue)
{
	x *= iValue;
	y *= iValue;
	z *= iValue;
	w *= iValue;
}

void _tagDxVector4::operator*=(int i[4])
{
	x *= i[0];
	y *= i[1];
	z *= i[2];
	w *= i[3];
}

// /
_tagDxVector4 _tagDxVector4::operator/(const _tagDxVector4 & _v) const
{
	_tagDxVector4	v1;

	v1.x = x / _v.x;
	v1.y = y / _v.y;
	v1.z = z / _v.z;
	v1.w = w / _v.w;

	return v1;
}

_tagDxVector4 _tagDxVector4::operator/(float _f[4]) const
{
	_tagDxVector4	v1;

	v1.x = x / _f[0];
	v1.y = y / _f[1];
	v1.z = z / _f[2];
	v1.w = w / _f[3];

	return v1;
}

_tagDxVector4 _tagDxVector4::operator/(float fValue) const
{
	_tagDxVector4	v1;

	v1.x = x / fValue;
	v1.y = y / fValue;
	v1.z = z / fValue;
	v1.w = w / fValue;

	return v1;
}

_tagDxVector4 _tagDxVector4::operator/(int iValue) const
{
	_tagDxVector4	v1;

	v1.x = x / iValue;
	v1.y = y / iValue;
	v1.z = z / iValue;
	v1.w = w / iValue;

	return v1;
}

_tagDxVector4 _tagDxVector4::operator/(int i[4]) const
{
	_tagDxVector4	v1;

	v1.x = x / i[0];
	v1.y = y / i[1];
	v1.z = z / i[2];
	v1.w = w / i[3];

	return v1;
}

void _tagDxVector4::operator/=(const _tagDxVector4 & _v)
{
	x /= _v.x;
	y /= _v.y;
	z /= _v.z;
	w /= _v.w;
}

void _tagDxVector4::operator/=(float _f[4])
{
	x /= _f[0];
	y /= _f[1];
	z /= _f[2];
	w /= _f[3];
}

void _tagDxVector4::operator/=(float fValue)
{
	x /= fValue;
	y /= fValue;
	z /= fValue;
	w /= fValue;
}

void _tagDxVector4::operator/=(int iValue)
{
	x /= iValue;
	y /= iValue;
	z /= iValue;
	w /= iValue;
}

void _tagDxVector4::operator/=(int i[4])
{
	x /= i[0];
	y /= i[1];
	z /= i[2];
	w /= i[3];
}

void _tagDxVector4::operator++()
{
	++x;
	++y;
	++z;
	++w;
}

void _tagDxVector4::operator--()
{
	--x;
	--y;
	--z;
	--w;
}

// ==
bool _tagDxVector4::operator==(const _tagDxVector4 & _v) const
{
	return x == _v.x && y == _v.y && z == _v.z && w == _v.w;
}

bool _tagDxVector4::operator==(float _f[4]) const
{
	return x == _f[0] && y == _f[1] && z == _f[2] && w == _f[3];
}

bool _tagDxVector4::operator==(int i[4]) const
{
	return x == i[0] && y == i[1] && z == i[2] && w == i[3];
}

bool _tagDxVector4::operator!=(const _tagDxVector4 & _v) const
{
	return x != _v.x || y != _v.y || z != _v.z || w != _v.w;
}

bool _tagDxVector4::operator!=(float _f[4]) const
{
	return x != _f[0] || y != _f[1] || z != _f[2] || w != _f[3];
}

bool _tagDxVector4::operator!=(int i[4]) const
{
	return x != i[0] || y != i[1] || z != i[2] || w != i[3];
}

XMVECTOR _tagDxVector4::Convert() const
{
	return XMLoadFloat4((XMFLOAT4*)this);
}

// ==================== Vector4 ====================
_tagVector4::_tagVector4() :
	x(0.f),
	y(0.f),
	z(0.f),
	w(0.f)
{
}

_tagVector4::_tagVector4(float _x, float _y, float _z, float _w) :
	x(_x),
	y(_y),
	z(_z),
	w(_w)
{
}

_tagVector4::_tagVector4(float _f[4]) :
	x(_f[0]),
	y(_f[1]),
	z(_f[2]),
	w(_f[3])
{
}

_tagVector4::_tagVector4(const _tagVector4& vec)
{
	*this = vec;
}

_tagVector4::_tagVector4(const XMVECTOR & vec)
{
	v = vec;
}

_tagVector4::_tagVector4(const _tagDxVector4& vec)
{
	*this = vec;
}

void _tagVector4::operator =(const _tagVector4& vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
	w = vec.w;
}

void _tagVector4::operator=(const XMVECTOR & vec)
{
	v = vec;
}

void _tagVector4::operator =(const _tagDxVector4& vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
	w = vec.w;
}

void _tagVector4::operator =(float _f[4])
{
	x = _f[0];
	y = _f[1];
	z = _f[2];
	w = _f[3];
}

// +
_tagVector4 _tagVector4::operator+(const _tagDxVector4 & _v) const
{
	_tagVector4	v1;

	v1.x = x + _v.x;
	v1.y = y + _v.y;
	v1.z = z + _v.z;
	v1.w = w + _v.w;

	return v1;
}

_tagVector4 _tagVector4::operator+(const _tagVector4 & _v) const
{
	_tagVector4	v1;

	v1.v = v + _v.v;

	return v1;
}

_tagVector4 _tagVector4::operator+(float _f[4]) const
{
	_tagVector4	v1;

	v1.x = x + _f[0];
	v1.y = y + _f[1];
	v1.z = z + _f[2];
	v1.w = w + _f[3];

	return v1;
}

_tagVector4 _tagVector4::operator+(float fValue) const
{
	_tagVector4	v1;

	v1.x = x + fValue;
	v1.y = y + fValue;
	v1.z = z + fValue;
	v1.w = w + fValue;

	return v1;
}

_tagVector4 _tagVector4::operator+(int iValue) const
{
	_tagVector4	v1;

	v1.x = x + iValue;
	v1.y = y + iValue;
	v1.z = z + iValue;
	v1.w = w + iValue;

	return v1;
}

_tagVector4 _tagVector4::operator+(int i[4]) const
{
	_tagVector4	v1;

	v1.x = x + i[0];
	v1.y = y + i[1];
	v1.z = z + i[2];
	v1.w = w + i[3];

	return v1;
}

void _tagVector4::operator+=(const _tagDxVector4 & _v)
{
	x += _v.x;
	y += _v.y;
	z += _v.z;
	w += _v.w;
}

void _tagVector4::operator+=(const _tagVector4 & _v)
{
	v += _v.v;
}

void _tagVector4::operator+=(float _f[4])
{
	x += _f[0];
	y += _f[1];
	z += _f[2];
	w += _f[3];
}

void _tagVector4::operator+=(float fValue)
{
	x += fValue;
	y += fValue;
	z += fValue;
	w += fValue;
}

void _tagVector4::operator+=(int iValue)
{
	x += iValue;
	y += iValue;
	z += iValue;
	w += iValue;
}

void _tagVector4::operator+=(int i[4])
{
	x += i[0];
	y += i[1];
	z += i[2];
	w += i[3];
}

// -
_tagVector4 _tagVector4::operator-(const _tagDxVector4 & _v) const
{
	_tagVector4	v1;

	v1.x = x - _v.x;
	v1.y = y - _v.y;
	v1.z = z - _v.z;
	v1.w = w - _v.w;

	return v1;
}

_tagVector4 _tagVector4::operator-(const _tagVector4 & _v) const
{
	_tagVector4	v1;

	v1.v = v - _v.v;

	return v1;
}


_tagVector4 _tagVector4::operator-(float _f[4]) const
{
	_tagVector4	v1;

	v1.x = x - _f[0];
	v1.y = y - _f[1];
	v1.z = z - _f[2];
	v1.w = w - _f[3];

	return v1;
}

_tagVector4 _tagVector4::operator-(float fValue) const
{
	_tagVector4	v1;

	v1.x = x - fValue;
	v1.y = y - fValue;
	v1.z = z - fValue;
	v1.w = w - fValue;

	return v1;
}

_tagVector4 _tagVector4::operator-(int iValue) const
{
	_tagVector4	v1;

	v1.x = x - iValue;
	v1.y = y - iValue;
	v1.z = z - iValue;
	v1.w = w - iValue;

	return v1;
}

_tagVector4 _tagVector4::operator-(int i[4]) const
{
	_tagVector4	v1;

	v1.x = x - i[0];
	v1.y = y - i[1];
	v1.z = z - i[2];
	v1.w = w - i[3];

	return v1;
}

void _tagVector4::operator-=(const _tagDxVector4 & _v)
{
	x -= _v.x;
	y -= _v.y;
	z -= _v.z;
	w -= _v.w;
}

void _tagVector4::operator-=(const _tagVector4 & _v)
{
	v -= _v.v;
}

void _tagVector4::operator-=(float _f[4])
{
	x -= _f[0];
	y -= _f[1];
	z -= _f[2];
	w -= _f[3];
}

void _tagVector4::operator-=(float fValue)
{
	x -= fValue;
	y -= fValue;
	z -= fValue;
	w -= fValue;
}

void _tagVector4::operator-=(int iValue)
{
	x -= iValue;
	y -= iValue;
	z -= iValue;
	w -= iValue;
}

void _tagVector4::operator-=(int i[4])
{
	x -= i[0];
	y -= i[1];
	z -= i[2];
	w -= i[3];
}

// *
_tagVector4 _tagVector4::operator*(const _tagDxVector4 & _v) const
{
	_tagVector4	v1;

	v1.x = x * _v.x;
	v1.y = y * _v.y;
	v1.z = z * _v.z;
	v1.w = w * _v.w;

	return v1;
}

_tagVector4 _tagVector4::operator*(const _tagVector4 & _v) const
{
	_tagVector4	v1;

	v1.v = v * _v.v;

	return v1;
}


_tagVector4 _tagVector4::operator*(float _f[4]) const
{
	_tagVector4	v1;

	v1.x = x * _f[0];
	v1.y = y * _f[1];
	v1.z = z * _f[2];
	v1.w = w * _f[3];

	return v1;
}

_tagVector4 _tagVector4::operator*(float fValue) const
{
	_tagVector4	v1;

	v1.x = x * fValue;
	v1.y = y * fValue;
	v1.z = z * fValue;
	v1.w = w * fValue;

	return v1;
}

_tagVector4 _tagVector4::operator*(int iValue) const
{
	_tagVector4	v1;

	v1.x = x * iValue;
	v1.y = y * iValue;
	v1.z = z * iValue;
	v1.w = w * iValue;

	return v1;
}

_tagVector4 _tagVector4::operator*(int i[4]) const
{
	_tagVector4	v1;

	v1.x = x * i[0];
	v1.y = y * i[1];
	v1.z = z * i[2];
	v1.w = w * i[3];

	return v1;
}

void _tagVector4::operator*=(const _tagDxVector4 & _v)
{
	x *= _v.x;
	y *= _v.y;
	z *= _v.z;
	w *= _v.w;
}

void _tagVector4::operator*=(const _tagVector4 & _v)
{
	v *= _v.v;
}

void _tagVector4::operator*=(float _f[4])
{
	x *= _f[0];
	y *= _f[1];
	z *= _f[2];
	w *= _f[3];
}

void _tagVector4::operator*=(float fValue)
{
	x *= fValue;
	y *= fValue;
	z *= fValue;
	w *= fValue;
}

void _tagVector4::operator*=(int iValue)
{
	x *= iValue;
	y *= iValue;
	z *= iValue;
	w *= iValue;
}

void _tagVector4::operator*=(int i[4])
{
	x *= i[0];
	y *= i[1];
	z *= i[2];
	w *= i[3];
}

// /
_tagVector4 _tagVector4::operator/(const _tagDxVector4 & _v) const
{
	_tagVector4	v1;

	v1.x = x / _v.x;
	v1.y = y / _v.y;
	v1.z = z / _v.z;
	v1.w = w / _v.w;

	return v1;
}

_tagVector4 _tagVector4::operator/(const _tagVector4 & _v) const
{
	_tagVector4	v1;

	v1.v = v / _v.v;

	return v1;
}

_tagVector4 _tagVector4::operator/(float _f[4]) const
{
	_tagVector4	v1;

	v1.x = x / _f[0];
	v1.y = y / _f[1];
	v1.z = z / _f[2];
	v1.w = w / _f[3];

	return v1;
}

_tagVector4 _tagVector4::operator/(float fValue) const
{
	_tagVector4	v1;

	v1.x = x / fValue;
	v1.y = y / fValue;
	v1.z = z / fValue;
	v1.w = w / fValue;

	return v1;
}

_tagVector4 _tagVector4::operator/(int iValue) const
{
	_tagVector4	v1;

	v1.x = x / iValue;
	v1.y = y / iValue;
	v1.z = z / iValue;
	v1.w = w / iValue;

	return v1;
}

_tagVector4 _tagVector4::operator/(int i[4]) const
{
	_tagVector4	v1;

	v1.x = x / i[0];
	v1.y = y / i[1];
	v1.z = z / i[2];
	v1.w = w / i[3];

	return v1;
}

void _tagVector4::operator/=(const _tagDxVector4 & _v)
{
	x /= _v.x;
	y /= _v.y;
	z /= _v.z;
	w /= _v.w;
}

void _tagVector4::operator/=(const _tagVector4 & _v)
{
	v /= _v.v;
}

void _tagVector4::operator/=(float _f[4])
{
	x /= _f[0];
	y /= _f[1];
	z /= _f[2];
	w /= _f[3];
}

void _tagVector4::operator/=(float fValue)
{
	x /= fValue;
	y /= fValue;
	z /= fValue;
	w /= fValue;
}

void _tagVector4::operator/=(int iValue)
{
	x /= iValue;
	y /= iValue;
	z /= iValue;
	w /= iValue;
}

void _tagVector4::operator/=(int i[4])
{
	x /= i[0];
	y /= i[1];
	z /= i[2];
	w /= i[3];
}

void _tagVector4::operator++()
{
	++x;
	++y;
	++z;
	++w;
}

void _tagVector4::operator--()
{
	--x;
	--y;
	--z;
	--w;
}

// ==
bool _tagVector4::operator==(const _tagDxVector4 & _v) const
{
	return x == _v.x && y == _v.y && z == _v.z && w == _v.w;
}

bool _tagVector4::operator==(const _tagVector4 & _v) const
{
	return x == _v.x && y == _v.y && z == _v.z && w == _v.w;
}

bool _tagVector4::operator==(float _f[4]) const
{
	return x == _f[0] && y == _f[1] && z == _f[2] && w == _f[3];
}

bool _tagVector4::operator==(int i[4]) const
{
	return x == i[0] && y == i[1] && z == i[2] && w == i[3];
}

// !=
bool _tagVector4::operator!=(const _tagDxVector4 & _v) const
{
	return x == _v.x && y == _v.y && z == _v.z && w == _v.w;
}

bool _tagVector4::operator!=(const _tagVector4 & _v) const
{
	return x != _v.x || y != _v.y || z != _v.z || w != _v.w;
}

bool _tagVector4::operator!=(float _f[4]) const
{
	return x != _f[0] || y != _f[1] || z != _f[2] || w != _f[3];
}

bool _tagVector4::operator!=(int i[4]) const
{
	return x != i[0] || y != i[1] || z != i[2] || w != i[3];
}

float _tagVector4::operator[](int idx) const
{
	return f[idx];
}

_tagVector4 _tagVector4::Transform(const _tagDxMatrix & mat)
{
	_tagVector4 v1 = XMVector4Transform(v, mat.mat);
	return v1;
}
