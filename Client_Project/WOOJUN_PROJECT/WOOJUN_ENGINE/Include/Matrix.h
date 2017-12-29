#pragma once
#include "Engine_Macro.h"
#include "DxMath.h"

WOOJUN_BEGIN

typedef union DLL _tagDxMatrix
{
	XMMATRIX	mat;
	struct
	{
		float _11, _12, _13, _14;
		float _21, _22, _23, _24;
		float _31, _32, _33, _34;
		float _41, _42, _43, _44;
	};
	float m[4][4];

	_tagDxMatrix();
	_tagDxMatrix(const XMMATRIX& _mat);
	_tagDxMatrix(float _f[4][4]);
	_tagDxMatrix(const _tagDxMatrix& _mat);

	void* operator new(size_t size)
	{
		return _aligned_malloc(size, 16);
	}

	void operator delete(void* p)
	{
		_aligned_free(p);
	}

	void operator =(const _tagDxMatrix& _mat);
	void operator =(const XMMATRIX& _mat);
	void operator =(float _f[4][4]);

	void operator *=(const _tagDxMatrix& _mat);
	void operator *=(const XMMATRIX& _mat);
	void operator *=(float _f[4][4]);

	_tagDxMatrix operator *(const _tagDxMatrix& _mat);
	_tagDxMatrix operator *(const XMMATRIX& _mat);
	_tagDxMatrix operator *(float _f[4][4]);

	XMMATRIX Identity();
	XMMATRIX Transpose();
	XMMATRIX Inverse();
}MATRIX, *pMATRIX;

WOOJUN_END