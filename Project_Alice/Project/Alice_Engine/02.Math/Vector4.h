#pragma once

#include "Engine_Macro.h"
#include "DxMath.h"

WOOJUN_BEGIN

typedef struct DLL _tagDxVector4 : public XMFLOAT4
{
	_tagDxVector4();
	_tagDxVector4(float _x, float _y, float _z, float _w);
	_tagDxVector4(float _f[4]);
	_tagDxVector4(int _i[4]);
	_tagDxVector4(const _tagDxVector4& vec);

	void operator =(const _tagDxVector4& vec);
	void operator =(float _f[4]);
	void operator =(int _i[4]);

	// +
	_tagDxVector4 operator +(const _tagDxVector4& _v) const;
	_tagDxVector4 operator +(float _f[4]) const;
	_tagDxVector4 operator +(float fValue) const;
	_tagDxVector4 operator +(int iValue) const;
	_tagDxVector4 operator +(int i[4]) const;

	// +=
	void operator +=(const _tagDxVector4& _v);
	void operator +=(float _f[4]);
	void operator +=(float fValue);
	void operator +=(int iValue);
	void operator +=(int i[4]);

	// -
	_tagDxVector4 operator -(const _tagDxVector4& _v) const;
	_tagDxVector4 operator -(float _f[4]) const;
	_tagDxVector4 operator -(float fValue) const;
	_tagDxVector4 operator -(int iValue) const;
	_tagDxVector4 operator -(int i[4]) const;

	// -=
	void operator -=(const _tagDxVector4& _v);
	void operator -=(float _f[4]);
	void operator -=(float fValue);
	void operator -=(int iValue);
	void operator -=(int i[4]);

	// *
	_tagDxVector4 operator *(const _tagDxVector4& _v) const;
	_tagDxVector4 operator *(float _f[4]) const;
	_tagDxVector4 operator *(float fValue) const;
	_tagDxVector4 operator *(int iValue) const;
	_tagDxVector4 operator *(int i[4]) const;

	// *=
	void operator *=(const _tagDxVector4& _v);
	void operator *=(float _f[4]);
	void operator *=(float fValue);
	void operator *=(int iValue);
	void operator *=(int i[4]);

	// /
	_tagDxVector4 operator /(const _tagDxVector4& _v) const;
	_tagDxVector4 operator /(float _f[4]) const;
	_tagDxVector4 operator /(float fValue) const;
	_tagDxVector4 operator /(int iValue) const;
	_tagDxVector4 operator /(int i[4]) const;

	// /=
	void operator /=(const _tagDxVector4& _v);
	void operator /=(float _f[4]);
	void operator /=(float fValue);
	void operator /=(int iValue);
	void operator /=(int i[4]);

	// ++
	void operator ++();
	void operator --();

	// ==
	bool operator ==(const _tagDxVector4& _v) const;
	bool operator ==(float _f[4]) const;
	bool operator ==(int i[4]) const;

	// !=
	bool operator !=(const _tagDxVector4& _v) const;
	bool operator !=(float _f[4]) const;
	bool operator !=(int i[4]) const;

	XMVECTOR Convert() const;
}DxVector4, *pDxVector4;


typedef union DLL _tagVector4
{	
	XMVECTOR	v;
	struct
	{
		float	x, y, z, w;
	};
	struct
	{
		float	r, g, b, a;
	};
	struct
	{
		float	f[4];
	};	

	_tagVector4();
	_tagVector4(float _x, float _y, float _z, float _w);
	_tagVector4(float _f[4]);
	_tagVector4(const _tagDxVector4& vec);
	_tagVector4(const _tagVector4& vec);
	_tagVector4(const XMVECTOR& vec);

	void* operator new(size_t size)
	{
		return _aligned_malloc(size, 16);
	}

	void operator delete(void* p)
	{
		_aligned_free(p);
	}

	void operator =(const _tagDxVector4& vec);
	void operator =(const _tagVector4& vec);
	void operator =(const XMVECTOR& vec);
	void operator =(float _f[4]);

	// +
	_tagVector4 operator +(const _tagDxVector4& _v) const;
	_tagVector4 operator +(const _tagVector4& _v) const;
	_tagVector4 operator +(float _f[4]) const;
	_tagVector4 operator +(float fValue) const;
	_tagVector4 operator +(int iValue) const;
	_tagVector4 operator +(int i[4]) const;

	// +=
	void operator +=(const _tagDxVector4& _v);
	void operator +=(const _tagVector4& _v);
	void operator +=(float _f[4]);
	void operator +=(float fValue);
	void operator +=(int iValue);
	void operator +=(int i[4]);

	// -
	_tagVector4 operator -(const _tagDxVector4& _v) const;
	_tagVector4 operator -(const _tagVector4& _v) const;
	_tagVector4 operator -(float _f[4]) const;
	_tagVector4 operator -(float fValue) const;
	_tagVector4 operator -(int iValue) const;
	_tagVector4 operator -(int i[4]) const;

	// -=
	void operator -=(const _tagDxVector4& _v);
	void operator -=(const _tagVector4& _v);
	void operator -=(float _f[4]);
	void operator -=(float fValue);
	void operator -=(int iValue);
	void operator -=(int i[4]);

	// *
	_tagVector4 operator *(const _tagDxVector4& _v) const;
	_tagVector4 operator *(const _tagVector4& _v) const;
	_tagVector4 operator *(float _f[4]) const;
	_tagVector4 operator *(float fValue) const;
	_tagVector4 operator *(int iValue) const;
	_tagVector4 operator *(int i[4]) const;

	// *=
	void operator *=(const _tagDxVector4& _v);
	void operator *=(const _tagVector4& _v);
	void operator *=(float _f[4]);
	void operator *=(float fValue);
	void operator *=(int iValue);
	void operator *=(int i[4]);

	// /
	_tagVector4 operator /(const _tagDxVector4& _v) const;
	_tagVector4 operator /(const _tagVector4& _v) const;
	_tagVector4 operator /(float _f[4]) const;
	_tagVector4 operator /(float fValue) const;
	_tagVector4 operator /(int iValue) const;
	_tagVector4 operator /(int i[4]) const;

	// /=
	void operator /=(const _tagDxVector4& _v);
	void operator /=(const _tagVector4& _v);
	void operator /=(float _f[4]);
	void operator /=(float fValue);
	void operator /=(int iValue);
	void operator /=(int i[4]);

	// ++
	void operator ++();
	void operator --();

	// ==
	bool operator ==(const _tagDxVector4& _v) const;
	bool operator ==(const _tagVector4& _v) const;
	bool operator ==(float _f[4]) const;
	bool operator ==(int i[4]) const;

	// !=
	bool operator !=(const _tagDxVector4& _v) const;
	bool operator !=(const _tagVector4& _v) const;
	bool operator !=(float _f[4]) const;
	bool operator !=(int i[4]) const;

	float operator [](int idx) const;

	_tagVector4 Transform(const union _tagDxMatrix& mat);
}Vector4, *pVector4;

WOOJUN_END