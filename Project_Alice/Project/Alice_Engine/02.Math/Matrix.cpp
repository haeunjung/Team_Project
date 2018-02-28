#include "Matrix.h"

WOOJUN_USING

// 항등행렬로 생성한다
_tagDxMatrix::_tagDxMatrix() :
	_11(1.f), _12(0.f), _13(0.f), _14(0.f),
	_21(0.f), _22(1.f), _23(0.f), _24(0.f),
	_31(0.f), _32(0.f), _33(1.f), _34(0.f),
	_41(0.f), _42(0.f), _43(0.f), _44(1.f)
{
}

_tagDxMatrix::_tagDxMatrix(const XMMATRIX & _mat)
{
	mat = _mat;
}

_tagDxMatrix::_tagDxMatrix(float _f[4][4])
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			m[i][j] = _f[i][j];
		}
	}
}

_tagDxMatrix::_tagDxMatrix(const _tagDxMatrix & _mat)
{
	mat = _mat.mat;
}

void _tagDxMatrix::operator=(const _tagDxMatrix & _mat)
{
	mat = _mat.mat;
}

void _tagDxMatrix::operator=(const XMMATRIX & _mat)
{
	mat = _mat;
}

void _tagDxMatrix::operator=(float _f[4][4])
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			m[i][j] = _f[i][j];
		}
	}
}

void _tagDxMatrix::operator*=(const _tagDxMatrix & _mat)
{
	mat *= _mat.mat;
}

void _tagDxMatrix::operator*=(const XMMATRIX & _mat)
{
	mat *= _mat;
}

void _tagDxMatrix::operator*=(float _f[4][4])
{
	float	_m[4][4];

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			float	f = 0.f;
			for (int k = 0; k < 4; ++k)
			{
				f += m[i][k] * _f[k][j];
			}

			_m[i][j] = f;
		}
	}

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			m[i][j] = _m[i][j];
		}
	}
}

_tagDxMatrix _tagDxMatrix::operator*(const _tagDxMatrix & _mat)
{
	return mat * _mat.mat;
}

_tagDxMatrix _tagDxMatrix::operator*(const XMMATRIX & _mat)
{
	return mat * _mat;
}

_tagDxMatrix _tagDxMatrix::operator*(float _f[4][4])
{
	_tagDxMatrix	mat1;
	float	_m[4][4];

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			float	f = 0.f;
			for (int k = 0; k < 4; ++k)
			{
				f += m[i][k] * _f[k][j];
			}

			_m[i][j] = f;
		}
	}

	mat1 = _m;

	return mat1.mat;
}

XMMATRIX _tagDxMatrix::Identity()
{
	return XMMatrixIdentity();
}

XMMATRIX _tagDxMatrix::Transpose()
{
	return XMMatrixTranspose(mat);
}

XMMATRIX _tagDxMatrix::Inverse()
{
	return XMMatrixInverse(&XMMatrixDeterminant(mat), mat);
}
