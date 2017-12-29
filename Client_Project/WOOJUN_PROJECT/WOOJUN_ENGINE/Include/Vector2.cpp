#include "Vector2.h"

WOOJUN_USING

// ==================== DxVector2 ====================
_tagDxVector2::_tagDxVector2()
{
	x = 0.0f;
	y = 0.0f;
}

_tagDxVector2::_tagDxVector2(float _x, float _y) :
	XMFLOAT2(_x, _y)
{
}

_tagDxVector2::_tagDxVector2(float _f[2])	
{
	x = _f[0];
	y = _f[1];
}

_tagDxVector2::_tagDxVector2(const _tagDxVector2& vec)
{
	*this = vec;
}

void _tagDxVector2::operator =(const _tagDxVector2& vec)
{
	x = vec.x;
	y = vec.y;
}

void _tagDxVector2::operator =(float _f[2])
{
	x = _f[0];
	y = _f[1];
}

// +
_tagDxVector2 _tagDxVector2::operator+(const _tagDxVector2 & _v) const
{
	_tagDxVector2	v1;

	v1.x = x + _v.x;
	v1.y = y + _v.y;
	
	return v1;
}

_tagDxVector2 _tagDxVector2::operator+(float _f[2]) const
{
	_tagDxVector2	v1;

	v1.x = x + _f[0];
	v1.y = y + _f[1];
	
	return v1;
}

_tagDxVector2 _tagDxVector2::operator+(float fValue) const
{
	_tagDxVector2	v1;

	v1.x = x + fValue;
	v1.y = y + fValue;
	
	return v1;
}

_tagDxVector2 _tagDxVector2::operator+(int iValue) const
{
	_tagDxVector2	v1;

	v1.x = x + iValue;
	v1.y = y + iValue;
	
	return v1;
}

_tagDxVector2 _tagDxVector2::operator+(int i[2]) const
{
	_tagDxVector2	v1;

	v1.x = x + i[0];
	v1.y = y + i[1];
	
	return v1;
}

void _tagDxVector2::operator+=(const _tagDxVector2 & _v)
{
	x += _v.x;
	y += _v.y;
}

void _tagDxVector2::operator+=(float _f[2])
{
	x += _f[0];
	y += _f[1];
}

void _tagDxVector2::operator+=(float fValue)
{
	x += fValue;
	y += fValue;
}

void _tagDxVector2::operator+=(int iValue)
{
	x += iValue;
	y += iValue;
}

void _tagDxVector2::operator+=(int i[2])
{
	x += i[0];
	y += i[1];
}

// -
_tagDxVector2 _tagDxVector2::operator-(const _tagDxVector2 & _v) const
{
	_tagDxVector2	v1;

	v1.x = x - _v.x;
	v1.y = y - _v.y;

	return v1;
}

_tagDxVector2 _tagDxVector2::operator-(float _f[2]) const
{
	_tagDxVector2	v1;

	v1.x = x - _f[0];
	v1.y = y - _f[1];

	return v1;
}

_tagDxVector2 _tagDxVector2::operator-(float fValue) const
{
	_tagDxVector2	v1;

	v1.x = x - fValue;
	v1.y = y - fValue;

	return v1;
}

_tagDxVector2 _tagDxVector2::operator-(int iValue) const
{
	_tagDxVector2	v1;

	v1.x = x - iValue;
	v1.y = y - iValue;

	return v1;
}

_tagDxVector2 _tagDxVector2::operator-(int i[2]) const
{
	_tagDxVector2	v1;

	v1.x = x - i[0];
	v1.y = y - i[1];

	return v1;
}

void _tagDxVector2::operator-=(const _tagDxVector2 & _v)
{
	x -= _v.x;
	y -= _v.y;
}

void _tagDxVector2::operator-=(float _f[2])
{
	x -= _f[0];
	y -= _f[1];
}

void _tagDxVector2::operator-=(float fValue)
{
	x -= fValue;
	y -= fValue;
}

void _tagDxVector2::operator-=(int iValue)
{
	x -= iValue;
	y -= iValue;
}

void _tagDxVector2::operator-=(int i[2])
{
	x -= i[0];
	y -= i[1];
}

// *
_tagDxVector2 _tagDxVector2::operator*(const _tagDxVector2 & _v) const
{
	_tagDxVector2	v1;

	v1.x = x * _v.x;
	v1.y = y * _v.y;

	return v1;
}

_tagDxVector2 _tagDxVector2::operator*(float _f[2]) const
{
	_tagDxVector2	v1;

	v1.x = x * _f[0];
	v1.y = y * _f[1];

	return v1;
}

_tagDxVector2 _tagDxVector2::operator*(float fValue) const
{
	_tagDxVector2	v1;

	v1.x = x * fValue;
	v1.y = y * fValue;

	return v1;
}

_tagDxVector2 _tagDxVector2::operator*(int iValue) const
{
	_tagDxVector2	v1;

	v1.x = x * iValue;
	v1.y = y * iValue;

	return v1;
}

_tagDxVector2 _tagDxVector2::operator*(int i[2]) const
{
	_tagDxVector2	v1;

	v1.x = x * i[0];
	v1.y = y * i[1];

	return v1;
}

void _tagDxVector2::operator*=(const _tagDxVector2 & _v)
{
	x *= _v.x;
	y *= _v.y;
}

void _tagDxVector2::operator*=(float _f[2])
{
	x *= _f[0];
	y *= _f[1];
}

void _tagDxVector2::operator*=(float fValue)
{
	x *= fValue;
	y *= fValue;
}

void _tagDxVector2::operator*=(int iValue)
{
	x *= iValue;
	y *= iValue;
}

void _tagDxVector2::operator*=(int i[2])
{
	x *= i[0];
	y *= i[1];
}

// /
_tagDxVector2 _tagDxVector2::operator/(const _tagDxVector2 & _v) const
{
	_tagDxVector2	v1;

	v1.x = x / _v.x;
	v1.y = y / _v.y;

	return v1;
}

_tagDxVector2 _tagDxVector2::operator/(float _f[2]) const
{
	_tagDxVector2	v1;

	v1.x = x / _f[0];
	v1.y = y / _f[1];

	return v1;
}

_tagDxVector2 _tagDxVector2::operator/(float fValue) const
{
	_tagDxVector2	v1;

	v1.x = x / fValue;
	v1.y = y / fValue;

	return v1;
}

_tagDxVector2 _tagDxVector2::operator/(int iValue) const
{
	_tagDxVector2	v1;

	v1.x = x / iValue;
	v1.y = y / iValue;

	return v1;
}

_tagDxVector2 _tagDxVector2::operator/(int i[2]) const
{
	_tagDxVector2	v1;

	v1.x = x / i[0];
	v1.y = y / i[1];

	return v1;
}

void _tagDxVector2::operator/=(const _tagDxVector2 & _v)
{
	x /= _v.x;
	y /= _v.y;
}

void _tagDxVector2::operator/=(float _f[2])
{
	x /= _f[0];
	y /= _f[1];
}

void _tagDxVector2::operator/=(float fValue)
{
	x /= fValue;
	y /= fValue;
}

void _tagDxVector2::operator/=(int iValue)
{
	x /= iValue;
	y /= iValue;
}

void _tagDxVector2::operator/=(int i[2])
{
	x /= i[0];
	y /= i[1];
}

void _tagDxVector2::operator++()
{
	++x;
	++y;
}

void _tagDxVector2::operator--()
{
	--x;
	--y;
}

// ==
bool _tagDxVector2::operator==(const _tagDxVector2 & _v) const
{
	return x == _v.x && y == _v.y;
}

bool _tagDxVector2::operator==(float _f[2]) const
{
	return x == _f[0] && y == _f[1];
}

bool _tagDxVector2::operator==(int i[2]) const
{
	return x == i[0] && y == i[1];
}

bool _tagDxVector2::operator!=(const _tagDxVector2 & _v) const
{
	return x != _v.x || y != _v.y;
}

bool _tagDxVector2::operator!=(float _f[2]) const
{
	return x != _f[0] || y != _f[1];
}

bool _tagDxVector2::operator!=(int i[2]) const
{
	return x != i[0] || y != i[1];
}

//float _tagDxVector2::operator[](int idx) const
//{
//	return f[idx];
//}

float _tagDxVector2::Length() const
{
	return sqrtf(x * x + y * y);
}

_tagDxVector2 _tagDxVector2::Normalize() const
{
	return _tagDxVector2(x / Length(), y / Length());
}

// ==================== Vector2 ====================
_tagVector2::_tagVector2() :
	x(0.f),
	y(0.f)
{
}

_tagVector2::_tagVector2(float _x, float _y) :
	x(_x),
	y(_y)
{
}

_tagVector2::_tagVector2(float _f[2]) :
	x(_f[0]),
	y(_f[1])
{
}

_tagVector2::_tagVector2(const _tagVector2& vec)
{
	*this = vec;
}

_tagVector2::_tagVector2(const _tagDxVector2& vec)
{
	*this = vec;
}

void _tagVector2::operator =(const _tagVector2& vec)
{
	x = vec.x;
	y = vec.y;
}

void _tagVector2::operator =(const _tagDxVector2& vec)
{
	x = vec.x;
	y = vec.y;
}

void _tagVector2::operator =(float _f[2])
{
	x = _f[0];
	y = _f[1];
}

// +
_tagVector2 _tagVector2::operator+(const _tagDxVector2 & _v) const
{
	_tagVector2	v1;

	v1.x = x + _v.x;
	v1.y = y + _v.y;

	return v1;
}

_tagVector2 _tagVector2::operator+(const _tagVector2 & _v) const
{
	_tagVector2	v1;

	v1.v = v + _v.v;
	//v1.v = DirectX::operator+(v, _v.v);

	return v1;
}

_tagVector2 _tagVector2::operator+(float _f[2]) const
{
	_tagVector2	v1;

	v1.x = x + _f[0];
	v1.y = y + _f[1];

	return v1;
}

_tagVector2 _tagVector2::operator+(float fValue) const
{
	_tagVector2	v1;

	v1.x = x + fValue;
	v1.y = y + fValue;

	return v1;
}

_tagVector2 _tagVector2::operator+(int iValue) const
{
	_tagVector2	v1;

	v1.x = x + iValue;
	v1.y = y + iValue;

	return v1;
}

_tagVector2 _tagVector2::operator+(int i[2]) const
{
	_tagVector2	v1;

	v1.x = x + i[0];
	v1.y = y + i[1];

	return v1;
}

void _tagVector2::operator+=(const _tagDxVector2 & _v)
{
	x += _v.x;
	y += _v.y;
}

void _tagVector2::operator+=(const _tagVector2 & _v)
{
	v += _v.v;
	//DirectX::operator+=(v, _v.v);
}

void _tagVector2::operator+=(float _f[2])
{
	x += _f[0];
	y += _f[1];
}

void _tagVector2::operator+=(float fValue)
{
	x += fValue;
	y += fValue;
}

void _tagVector2::operator+=(int iValue)
{
	x += iValue;
	y += iValue;
}

void _tagVector2::operator+=(int i[2])
{
	x += i[0];
	y += i[1];
}

// -
_tagVector2 _tagVector2::operator-(const _tagDxVector2 & _v) const
{
	_tagVector2	v1;

	v1.x = x - _v.x;
	v1.y = y - _v.y;

	return v1;
}

_tagVector2 _tagVector2::operator-(const _tagVector2 & _v) const
{
	_tagVector2	v1;

	v1.v = v - _v.v;
	//v1.v = DirectX::operator-(v, _v.v);

	return v1;
}


_tagVector2 _tagVector2::operator-(float _f[2]) const
{
	_tagVector2	v1;

	v1.x = x - _f[0];
	v1.y = y - _f[1];

	return v1;
}

_tagVector2 _tagVector2::operator-(float fValue) const
{
	_tagVector2	v1;

	v1.x = x - fValue;
	v1.y = y - fValue;

	return v1;
}

_tagVector2 _tagVector2::operator-(int iValue) const
{
	_tagVector2	v1;

	v1.x = x - iValue;
	v1.y = y - iValue;

	return v1;
}

_tagVector2 _tagVector2::operator-(int i[2]) const
{
	_tagVector2	v1;

	v1.x = x - i[0];
	v1.y = y - i[1];

	return v1;
}

void _tagVector2::operator-=(const _tagDxVector2 & _v)
{
	x -= _v.x;
	y -= _v.y;
}

void _tagVector2::operator-=(const _tagVector2 & _v)
{
	v -= _v.v;
	//DirectX::operator-=(v, _v.v);
}

void _tagVector2::operator-=(float _f[2])
{
	x -= _f[0];
	y -= _f[1];
}

void _tagVector2::operator-=(float fValue)
{
	x -= fValue;
	y -= fValue;
}

void _tagVector2::operator-=(int iValue)
{
	x -= iValue;
	y -= iValue;
}

void _tagVector2::operator-=(int i[2])
{
	x -= i[0];
	y -= i[1];
}

// *
_tagVector2 _tagVector2::operator*(const _tagDxVector2 & _v) const
{
	_tagVector2	v1;

	v1.x = x * _v.x;
	v1.y = y * _v.y;

	return v1;
}

_tagVector2 _tagVector2::operator*(const _tagVector2 & _v) const
{
	_tagVector2	v1;

	v1.v = v * _v.v;
	//v1.v = DirectX::operator*(v, _v.v);

	return v1;
}


_tagVector2 _tagVector2::operator*(float _f[2]) const
{
	_tagVector2	v1;

	v1.x = x * _f[0];
	v1.y = y * _f[1];

	return v1;
}

_tagVector2 _tagVector2::operator*(float fValue) const
{
	_tagVector2	v1;

	v1.x = x * fValue;
	v1.y = y * fValue;

	return v1;
}

_tagVector2 _tagVector2::operator*(int iValue) const
{
	_tagVector2	v1;

	v1.x = x * iValue;
	v1.y = y * iValue;

	return v1;
}

_tagVector2 _tagVector2::operator*(int i[2]) const
{
	_tagVector2	v1;

	v1.x = x * i[0];
	v1.y = y * i[1];

	return v1;
}

void _tagVector2::operator*=(const _tagDxVector2 & _v)
{
	x *= _v.x;
	y *= _v.y;
}

void _tagVector2::operator*=(const _tagVector2 & _v)
{
	v *= _v.v;
	//DirectX::operator*=(v, _v.v);
}

void _tagVector2::operator*=(float _f[2])
{
	x *= _f[0];
	y *= _f[1];
}

void _tagVector2::operator*=(float fValue)
{
	x *= fValue;
	y *= fValue;
}

void _tagVector2::operator*=(int iValue)
{
	x *= iValue;
	y *= iValue;
}

void _tagVector2::operator*=(int i[2])
{
	x *= i[0];
	y *= i[1];
}

// /
_tagVector2 _tagVector2::operator/(const _tagDxVector2 & _v) const
{
	_tagVector2	v1;

	v1.x = x / _v.x;
	v1.y = y / _v.y;

	return v1;
}

_tagVector2 _tagVector2::operator/(const _tagVector2 & _v) const
{
	_tagVector2	v1;

	v1.v = v / _v.v;
	//v1.v = DirectX::operator/(v, _v.v);

	return v1;
}

_tagVector2 _tagVector2::operator/(float _f[2]) const
{
	_tagVector2	v1;

	v1.x = x / _f[0];
	v1.y = y / _f[1];

	return v1;
}

_tagVector2 _tagVector2::operator/(float fValue) const
{
	_tagVector2	v1;

	v1.x = x / fValue;
	v1.y = y / fValue;

	return v1;
}

_tagVector2 _tagVector2::operator/(int iValue) const
{
	_tagVector2	v1;

	v1.x = x / iValue;
	v1.y = y / iValue;

	return v1;
}

_tagVector2 _tagVector2::operator/(int i[2]) const
{
	_tagVector2	v1;

	v1.x = x / i[0];
	v1.y = y / i[1];

	return v1;
}

void _tagVector2::operator/=(const _tagDxVector2 & _v)
{	
	x /= _v.x;
	y /= _v.y;
}

void _tagVector2::operator/=(const _tagVector2 & _v)
{
	v /= _v.v;
	//DirectX::operator/=(v, _v.v);
}

void _tagVector2::operator/=(float _f[2])
{
	x /= _f[0];
	y /= _f[1];
}

void _tagVector2::operator/=(float fValue)
{
	x /= fValue;
	y /= fValue;
}

void _tagVector2::operator/=(int iValue)
{
	x /= iValue;
	y /= iValue;
}

void _tagVector2::operator/=(int i[2])
{
	x /= i[0];
	y /= i[1];
}

void _tagVector2::operator++()
{
	++x;
	++y;
}

void _tagVector2::operator--()
{
	--x;
	--y;
}

// ==
bool _tagVector2::operator==(const _tagDxVector2 & _v) const
{
	return x == _v.x && y == _v.y;
}

bool _tagVector2::operator==(const _tagVector2 & _v) const
{
	return x == _v.x && y == _v.y;
}

bool _tagVector2::operator==(float _f[2]) const
{
	return x == _f[0] && y == _f[1];
}

bool _tagVector2::operator==(int i[2]) const
{
	return x == i[0] && y == i[1];
}

// !=
bool _tagVector2::operator!=(const _tagDxVector2 & _v) const
{
	return x != _v.x || y != _v.y;
}

bool _tagVector2::operator!=(const _tagVector2 & _v) const
{
	return x != _v.x || y != _v.y;
}

bool _tagVector2::operator!=(float _f[2]) const
{
	return x != _f[0] || y != _f[1];
}

bool _tagVector2::operator!=(int i[2]) const
{
	return x != i[0] || y != i[1];
}

float _tagVector2::operator[](int idx) const
{
	return f[idx];
}

float _tagVector2::Length() const
{
	_tagVector2	v1;
	v1.v = XMVector2Length(v);
	return v1.x;
}

_tagVector2 _tagVector2::Normalize() const
{
	_tagVector2 v1;
	v1.v = XMVector2Normalize(v);
	return v1;
}