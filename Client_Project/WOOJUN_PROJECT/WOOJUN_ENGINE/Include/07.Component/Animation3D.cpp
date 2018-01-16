#include "Animation3D.h"

WOOJUN_USING

bool CAnimation3D::Init()
{
	return true;
}

void CAnimation3D::Input(float _fTime)
{
}

void CAnimation3D::Update(float _fTime)
{
}

void CAnimation3D::LateUpdate(float _fTime)
{
	
}

void CAnimation3D::Collision(float _fTime)
{
}

void CAnimation3D::Render(float _fTime)
{
}

CAnimation3D * CAnimation3D::Clone()
{
	return new CAnimation3D(*this);
}

CAnimation3D::CAnimation3D() 
{
	
}

CAnimation3D::CAnimation3D(const CAnimation3D & _Animation3D) :
	CComponent(_Animation3D)
{
	
}

CAnimation3D::~CAnimation3D()
{
	
}