#pragma once
#include "Engine_Macro.h"
#include "DxMath.h"

WOOJUN_BEGIN

typedef struct DLL _tagDxVector2 : public XMFLOAT2
{
	_tagDxVector2();
	_tagDxVector2(float _x, float _y);
	_tagDxVector2(float _f[2]);
	_tagDxVector2(const _tagDxVector2& vec);

	void operator =(const _tagDxVector2& vec);	
	void operator =(float _f[2]);

	// +
	_tagDxVector2 operator +(const _tagDxVector2& _v) const;
	_tagDxVector2 operator +(float _f[2]) const;
	_tagDxVector2 operator +(float fValue) const;
	_tagDxVector2 operator +(int iValue) const;
	_tagDxVector2 operator +(int i[2]) const;

	// +=
	void operator +=(const _tagDxVector2& _v);
	void operator +=(float _f[2]);
	void operator +=(float fValue);
	void operator +=(int iValue);
	void operator +=(int i[2]);

	// -
	_tagDxVector2 operator -(const _tagDxVector2& _v) const;
	_tagDxVector2 operator -(float _f[2]) const;
	_tagDxVector2 operator -(float fValue) const;
	_tagDxVector2 operator -(int iValue) const;
	_tagDxVector2 operator -(int i[2]) const;

	// -=
	void operator -=(const _tagDxVector2& _v);
	void operator -=(float _f[2]);
	void operator -=(float fValue);
	void operator -=(int iValue);
	void operator -=(int i[2]);

	// *
	_tagDxVector2 operator *(const _tagDxVector2& _v) const;
	_tagDxVector2 operator *(float _f[2]) const;
	_tagDxVector2 operator *(float fValue) const;
	_tagDxVector2 operator *(int iValue) const;
	_tagDxVector2 operator *(int i[2]) const;

	// *=
	void operator *=(const _tagDxVector2& _v);
	void operator *=(float _f[2]);
	void operator *=(float fValue);
	void operator *=(int iValue);
	void operator *=(int i[2]);

	// /
	_tagDxVector2 operator /(const _tagDxVector2& _v) const;
	_tagDxVector2 operator /(float _f[2]) const;
	_tagDxVector2 operator /(float fValue) const;
	_tagDxVector2 operator /(int iValue) const;
	_tagDxVector2 operator /(int i[2]) const;

	// /=
	void operator /=(const _tagDxVector2& _v);
	void operator /=(float _f[2]);
	void operator /=(float fValue);
	void operator /=(int iValue);
	void operator /=(int i[2]);

	// ++
	void operator ++();
	void operator --();

	// ==
	bool operator ==(const _tagDxVector2& _v) const;
	bool operator ==(float _f[2]) const;
	bool operator ==(int i[2]) const;

	// !=
	bool operator !=(const _tagDxVector2& _v) const;
	bool operator !=(float _f[2]) const;
	bool operator !=(int i[2]) const;
	
	float Length() const;
	_tagDxVector2 Normalize() const;
}DxVector2, *pDxVector2;


typedef union DLL _tagVector2
{
	XMVECTOR	v;
	struct
	{
		float	x, y, z, w;
	};
	struct
	{
		float	f[4];
	};

	_tagVector2();
	_tagVector2(float _x, float _y);
	_tagVector2(float _f[2]);
	_tagVector2(const _tagDxVector2& vec);
	_tagVector2(const _tagVector2& vec);
	
	void* operator new(size_t size)
	{
		return _aligned_malloc(size, 16);
	}

	void operator delete(void* p)
	{
		_aligned_free(p);
	}

	void operator =(const _tagDxVector2& vec);
	void operator =(const _tagVector2& vec);
	void operator =(float _f[2]);

	// +
	_tagVector2 operator +(const _tagDxVector2& _v) const;
	_tagVector2 operator +(const _tagVector2& _v) const;
	_tagVector2 operator +(float _f[2]) const;
	_tagVector2 operator +(float fValue) const;
	_tagVector2 operator +(int iValue) const;
	_tagVector2 operator +(int i[2]) const;

	// +=
	void operator +=(const _tagDxVector2& _v);
	void operator +=(const _tagVector2& _v);
	void operator +=(float _f[2]);
	void operator +=(float fValue);
	void operator +=(int iValue);
	void operator +=(int i[2]);

	// -
	_tagVector2 operator -(const _tagDxVector2& _v) const;
	_tagVector2 operator -(const _tagVector2& _v) const;
	_tagVector2 operator -(float _f[2]) const;
	_tagVector2 operator -(float fValue) const;
	_tagVector2 operator -(int iValue) const;
	_tagVector2 operator -(int i[2]) const;

	// -=
	void operator -=(const _tagDxVector2& _v);
	void operator -=(const _tagVector2& _v);
	void operator -=(float _f[2]);
	void operator -=(float fValue);
	void operator -=(int iValue);
	void operator -=(int i[2]);

	// *
	_tagVector2 operator *(const _tagDxVector2& _v) const;
	_tagVector2 operator *(const _tagVector2& _v) const;
	_tagVector2 operator *(float _f[2]) const;
	_tagVector2 operator *(float fValue) const;
	_tagVector2 operator *(int iValue) const;
	_tagVector2 operator *(int i[2]) const;

	// *=
	void operator *=(const _tagDxVector2& _v);
	void operator *=(const _tagVector2& _v);
	void operator *=(float _f[2]);
	void operator *=(float fValue);
	void operator *=(int iValue);
	void operator *=(int i[2]);

	// /
	_tagVector2 operator /(const _tagDxVector2& _v) const;
	_tagVector2 operator /(const _tagVector2& _v) const;
	_tagVector2 operator /(float _f[2]) const;
	_tagVector2 operator /(float fValue) const;
	_tagVector2 operator /(int iValue) const;
	_tagVector2 operator /(int i[2]) const;

	// /=
	void operator /=(const _tagDxVector2& _v);
	void operator /=(const _tagVector2& _v);
	void operator /=(float _f[2]);
	void operator /=(float fValue);
	void operator /=(int iValue);
	void operator /=(int i[2]);

	// ++
	void operator ++();
	void operator --();

	// ==
	bool operator ==(const _tagDxVector2& _v) const;
	bool operator ==(const _tagVector2& _v) const;
	bool operator ==(float _f[2]) const;
	bool operator ==(int i[2]) const;

	// !=
	bool operator !=(const _tagDxVector2& _v) const;
	bool operator !=(const _tagVector2& _v) const;
	bool operator !=(float _f[2]) const;
	bool operator !=(int i[2]) const;

	float operator [](int idx) const;

	float Length() const;
	_tagVector2 Normalize() const;
}Vector2, *pVector2;

WOOJUN_END