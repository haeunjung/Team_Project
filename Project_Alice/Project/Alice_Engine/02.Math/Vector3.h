#pragma once

#include "../Engine_Macro.h"
#include "DxMath.h"

WOOJUN_BEGIN

typedef struct DLL _tagDxVector3 : public XMFLOAT3
{
	_tagDxVector3();
	_tagDxVector3(float _x, float _y, float _z);
	_tagDxVector3(float _f[3]);
	_tagDxVector3(int _i[3]);
	_tagDxVector3(const _tagDxVector3& vec);
	_tagDxVector3(const union _tagVector3& vec);
	_tagDxVector3(const XMFLOAT3& vec);

	void operator =(const _tagDxVector3& vec);
	void operator =(const _tagVector3& vec);	
	void operator =(const XMFLOAT3& vec);
	void operator =(float _f[3]);	

	// +
	_tagDxVector3 operator +(const _tagDxVector3& _v) const;
	_tagDxVector3 operator +(float _f[3]) const;
	_tagDxVector3 operator +(float fValue) const;
	_tagDxVector3 operator +(int iValue) const;
	_tagDxVector3 operator +(int i[3]) const;

	// +=
	void operator +=(const _tagDxVector3& _v);
	void operator +=(float _f[3]);
	void operator +=(float fValue);
	void operator +=(int iValue);
	void operator +=(int i[3]);

	// -
	_tagDxVector3 operator -(const _tagDxVector3& _v) const;
	_tagDxVector3 operator -(float _f[3]) const;
	_tagDxVector3 operator -(float fValue) const;
	_tagDxVector3 operator -(int iValue) const;
	_tagDxVector3 operator -(int i[3]) const;

	// -=
	void operator -=(const _tagDxVector3& _v);
	void operator -=(float _f[3]);
	void operator -=(float fValue);
	void operator -=(int iValue);
	void operator -=(int i[3]);

	// *
	_tagDxVector3 operator *(const _tagDxVector3& _v) const;
	_tagDxVector3 operator *(float _f[3]) const;
	_tagDxVector3 operator *(float fValue) const;
	_tagDxVector3 operator *(int iValue) const;
	_tagDxVector3 operator *(int i[3]) const;

	// *=
	void operator *=(const _tagDxVector3& _v);
	void operator *=(float _f[3]);
	void operator *=(float fValue);
	void operator *=(int iValue);
	void operator *=(int i[3]);

	// /
	_tagDxVector3 operator /(const _tagDxVector3& _v) const;
	_tagDxVector3 operator /(float _f[3]) const;
	_tagDxVector3 operator /(float fValue) const;
	_tagDxVector3 operator /(int iValue) const;
	_tagDxVector3 operator /(int i[3]) const;

	// /=
	void operator /=(const _tagDxVector3& _v);
	void operator /=(float _f[3]);
	void operator /=(float fValue);
	void operator /=(int iValue);
	void operator /=(int i[3]);

	// ++
	void operator ++();
	void operator --();

	// ==
	bool operator ==(const _tagDxVector3& _v) const;
	bool operator ==(float _f[3]) const ;
	bool operator ==(int i[3]) const ;

	// !=
	bool operator !=(const _tagDxVector3& _v) const ;
	bool operator !=(float _f[3]) const;
	bool operator !=(int i[3]) const ;

	//float operator [](int idx) const ;

	float Length() const;
	float Distance(const _tagDxVector3& _v) const;
	_tagDxVector3 Normalize() const;
	float Dot(const _tagDxVector3& _v) const;
	float Dot(float _f[3]) const;
	float Dot(int i[3]) const;
	_tagDxVector3 Cross(const _tagDxVector3& _v) const;
	_tagDxVector3 Cross(float _f[3]) const;
	_tagDxVector3 Cross(int _i[3]) const;
	float GetAngle(const _tagDxVector3& _v) const;
	XMVECTOR Convert() const;
	_tagDxVector3 TransformNormal(const union _tagDxMatrix& _mat) const;
	_tagDxVector3 TransformCoord(const union _tagDxMatrix& _mat) const;
	float Max() const;
	float Min() const;
	_tagDxVector3 Lerp(const _tagDxVector3& _v, float _fPercent);
}DxVector3, *pDxVector3;

static DxVector3 Vec3Zero = { 0.0f, 0.0f, 0.0f };
static DxVector3 Vec3One = { 1.0f, 1.0f, 1.0f };

typedef union DLL _tagVector3
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

	_tagVector3();
	_tagVector3(float _x, float _y, float _z);
	_tagVector3(float _f[3]);
	_tagVector3(const _tagDxVector3& vec);
	_tagVector3(const _tagVector3& vec);
	_tagVector3(const XMVECTOR& vec);

	void* operator new(size_t size)
	{
		return _aligned_malloc(size, 16);
	}

	void operator delete(void* p)
	{
		_aligned_free(p);
	}

	void operator =(const _tagDxVector3& vec);
	void operator =(const _tagVector3& vec);
	void operator =(const XMVECTOR& vec);
	void operator =(float _f[3]);

	// +
	_tagVector3 operator +(const _tagDxVector3& _v) const;
	_tagVector3 operator +(const _tagVector3& _v) const;
	_tagVector3 operator +(float _f[3]) const;
	_tagVector3 operator +(float fValue) const;
	_tagVector3 operator +(int iValue) const;
	_tagVector3 operator +(int i[3]) const;

	// +=
	void operator +=(const _tagDxVector3& _v);
	void operator +=(const _tagVector3& _v);
	void operator +=(float _f[3]);
	void operator +=(float fValue);
	void operator +=(int iValue);
	void operator +=(int i[3]);

	// -
	_tagVector3 operator -(const _tagDxVector3& _v) const;
	_tagVector3 operator -(const _tagVector3& _v) const;
	_tagVector3 operator -(float _f[3]) const;
	_tagVector3 operator -(float fValue) const;
	_tagVector3 operator -(int iValue) const;
	_tagVector3 operator -(int i[3]) const;

	// -=
	void operator -=(const _tagDxVector3& _v);
	void operator -=(const _tagVector3& _v);
	void operator -=(float _f[3]);
	void operator -=(float fValue);
	void operator -=(int iValue);
	void operator -=(int i[3]);

	// *
	_tagVector3 operator *(const _tagDxVector3& _v) const;
	_tagVector3 operator *(const _tagVector3& _v) const;
	_tagVector3 operator *(float _f[3]) const;
	_tagVector3 operator *(float fValue) const;
	_tagVector3 operator *(int iValue) const;
	_tagVector3 operator *(int i[3]) const;

	// *=
	void operator *=(const _tagDxVector3& _v);
	void operator *=(const _tagVector3& _v);
	void operator *=(float _f[3]);
	void operator *=(float fValue);
	void operator *=(int iValue);
	void operator *=(int i[3]);

	// /
	_tagVector3 operator /(const _tagDxVector3& _v) const;
	_tagVector3 operator /(const _tagVector3& _v) const;
	_tagVector3 operator /(float _f[3]) const;
	_tagVector3 operator /(float fValue) const;
	_tagVector3 operator /(int iValue) const;
	_tagVector3 operator /(int i[3]) const;

	// /=
	void operator /=(const _tagDxVector3& _v);
	void operator /=(const _tagVector3& _v);
	void operator /=(float _f[3]);
	void operator /=(float fValue);
	void operator /=(int iValue);
	void operator /=(int i[3]);

	// ++
	void operator ++();
	void operator --();

	// ==
	bool operator ==(const _tagDxVector3& _v) const;
	bool operator ==(const _tagVector3& _v) const;
	bool operator ==(float _f[3]) const;
	bool operator ==(int i[3]) const;

	// !=
	bool operator !=(const _tagDxVector3& _v) const;
	bool operator !=(const _tagVector3& _v) const;
	bool operator !=(float _f[3]) const;
	bool operator !=(int i[3]) const;

	float operator [](int idx) const;

	float Length() const;

	_tagVector3 Normalize() const;

	float Dot(const _tagDxVector3& _v) const;
	float Dot(const _tagVector3& _v) const;
	float Dot(float _f[3]) const;
	float Dot(int i[3]) const;

	_tagVector3 Cross(const _tagDxVector3& _v) const;
	_tagVector3 Cross(const _tagVector3& _v) const;
	_tagVector3 Cross(float _f[3]) const;
	_tagVector3 Cross(int i[3]) const;

	_tagVector3 TransformNormal(const union _tagDxMatrix& mat);
	_tagVector3 TransformCoord(const union _tagDxMatrix& mat);
}Vector3, *pVector3;

WOOJUN_END