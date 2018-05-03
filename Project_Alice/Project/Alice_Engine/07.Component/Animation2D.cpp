#include "Animation2D.h"
#include "Renderer.h"
#include "Renderer2D.h"
#include "Material.h"
#include "../03.Resource/Texture.h"
#include "../03.Resource/ResMgr.h"
#include "../06.GameObject/GameObject.h"
#include "../04.Rendering/ShaderMgr.h"

WOOJUN_USING

void CAnimation2D::AddAnimation2DClip(const string& _strName, ANIMATION2D_TYPE _eType, ANIMATION_OPTION _eOption,
	int _iFrameMaxX, int _iFrameMaxY, float _fLimitTime,
	int _iLoopCount /*= 0*/, float _fLoopTime /*= 0.0f*/,
	const string& _strTexKey /*= ""*/, int _iTexRegister /*= 0*/, TCHAR* _pFileName /*= NULL*/,
	const string& _strPathKey /*= TEXTUREPATH*/)
{
	pANIMATIONCLIP2D	pClip = new ANIMATIONCLIP2D();

	pClip->strName = _strName;
	pClip->eType = _eType;
	pClip->eOption = _eOption;
	pClip->iFrameMaxX = _iFrameMaxX;
	pClip->iFrameMaxY = _iFrameMaxY;
	pClip->fLimitTime = _fLimitTime;
	pClip->iLoopCount = _iLoopCount;
	pClip->fLoopTime = _fLoopTime;
	pClip->iFrameX = 0;
	pClip->iFrameY = 0;
	pClip->fFrameTime = 0.f;
	pClip->iTexRegister = _iTexRegister;

	if (NULL == _pFileName)
	{
		pClip->pTexture = GET_SINGLE(CResMgr)->FindTexture(_strTexKey);
	}
	else
	{
		pClip->pTexture = GET_SINGLE(CResMgr)->LoadTexture(_strTexKey, _pFileName, _strPathKey);
	}

	m_vecAnimationClip.push_back(pClip);
}

void CAnimation2D::AddAnimation2DClip(const string& _strName, ANIMATION2D_TYPE _eType, ANIMATION_OPTION _eOption,
	int _iFrameMaxX, int _iFrameMaxY, float _fLimitTime,
	int _iLoopCount /*= 0*/, float _fLoopTime /*= 0.0f*/,
	const string& _strTexKey /*= ""*/, int _iTexRegister /*= 0*/, const vector<wstring>* _vecFileName /*= NULL*/,
	const string& _strPathKey /*= TEXTUREPATH*/)
{
	pANIMATIONCLIP2D	pClip = new ANIMATIONCLIP2D();

	pClip->strName = _strName;
	pClip->eType = _eType;
	pClip->eOption = _eOption;
	pClip->iFrameMaxX = _iFrameMaxX;
	pClip->iFrameMaxY = _iFrameMaxY;
	pClip->fLimitTime = _fLimitTime;
	pClip->iLoopCount = _iLoopCount;
	pClip->fLoopTime = _fLoopTime;
	pClip->iFrameX = 0;
	pClip->iFrameY = 0;
	pClip->fFrameTime = 0.f;
	pClip->iTexRegister = _iTexRegister;

	if (NULL == _vecFileName)
	{
		pClip->pTexture = GET_SINGLE(CResMgr)->FindTexture(_strTexKey);
	}
	else
	{
		pClip->pTexture = GET_SINGLE(CResMgr)->LoadTexture(_strTexKey, *_vecFileName, _strPathKey);
	}

	m_vecAnimationClip.push_back(pClip);
}

void CAnimation2D::ChangeAnimation(int _iAnimation)
{
	if (m_iCurrentAnimation == _iAnimation)
	{
		return;
	}

	// 기존 동작모션 초기화
	m_vecAnimationClip[m_iCurrentAnimation]->iFrameX = 0;
	m_vecAnimationClip[m_iCurrentAnimation]->iFrameY = 0;
	m_vecAnimationClip[m_iCurrentAnimation]->fFrameTime = 0.0f;

	m_iCurrentAnimation = _iAnimation;

	CRenderer*	pRenderer = m_pGameObject->FindComponentFromTypeID<CRenderer>();

	CMaterial*	pMaterial = pRenderer->GetMaterial(0, 0);
	pMaterial->SetDiffuseTexture(m_vecAnimationClip[m_iCurrentAnimation]->pTexture,
		m_vecAnimationClip[m_iCurrentAnimation]->iTexRegister);

	SAFE_RELEASE(pMaterial);
	SAFE_RELEASE(pRenderer);
}	

void CAnimation2D::SetDefaultAnimation(int _iAnimation)
{
	m_iDefaultAnimation = _iAnimation;
}

void CAnimation2D::SetShader()
{
	GET_SINGLE(CShaderMgr)->UpdateConstBuffer("Animation2D", &m_tCBuffer,
		CUT_VERTEX | CUT_PIXEL | CUT_GEOMETRY);
}

void CAnimation2D::Start()
{
	CRenderer*	pRenderer = m_pGameObject->FindComponentFromTypeID<CRenderer>();

	CMaterial*	pMaterial = pRenderer->GetMaterial(0, 0);

	pMaterial->SetDiffuseTexture(m_vecAnimationClip[m_iCurrentAnimation]->pTexture,
		m_vecAnimationClip[m_iCurrentAnimation]->iTexRegister);

	SAFE_RELEASE(pMaterial);

	SAFE_RELEASE(pRenderer);
}

bool CAnimation2D::Init()
{
	// Renderer Component가 있다면
	if (true == m_pGameObject->CheckComponentFromTypeID<CRenderer>())
	{
		// Renderer에 Animation2D를 위한 상수버퍼 추가
		CRenderer*	pRenderer = m_pGameObject->FindComponentFromTypeID<CRenderer>();
		pRenderer->AddConstBuffer("Animation2D", 12, sizeof(ANIMATION2DCBUFFER), CUT_VERTEX | CUT_PIXEL | CUT_GEOMETRY);

		SAFE_RELEASE(pRenderer);
	}
	else if (true == m_pGameObject->CheckComponentFromTypeID<CRenderer2D>())
	{
		// Renderer2D에 Animation2D를 위한 상수버퍼 추가
		CRenderer2D*	pRenderer2D = m_pGameObject->FindComponentFromTypeID<CRenderer2D>();
		pRenderer2D->AddConstBuffer("Animation2D", 12, sizeof(ANIMATION2DCBUFFER), CUT_VERTEX | CUT_PIXEL | CUT_GEOMETRY);

		SAFE_RELEASE(pRenderer2D);
	}

	return true;
}

void CAnimation2D::Input(float _fTime)
{
}

void CAnimation2D::Update(float _fTime)
{
}

void CAnimation2D::LateUpdate(float _fTime)
{
	pANIMATIONCLIP2D	pClip = m_vecAnimationClip[m_iCurrentAnimation];
	pClip->fFrameTime += _fTime;

	int iMaxFrame = pClip->iFrameMaxX * pClip->iFrameMaxY;
	float fFrameLimitTime = pClip->fLimitTime / iMaxFrame;

	if (pClip->fFrameTime >= fFrameLimitTime)
	{
		pClip->fFrameTime -= fFrameLimitTime;
		++pClip->iFrameX;

		if (pClip->iFrameX == pClip->iFrameMaxX)
		{
			++pClip->iFrameY;
			pClip->iFrameX = 0;

			if (pClip->iFrameY == pClip->iFrameMaxY)
			{
				pClip->iFrameY = 0;

				switch (pClip->eOption)
				{
				case AO_LOOP:
					break;
				case AO_ONCE_RETURN:
					ChangeAnimation(m_iDefaultAnimation);
					break;
				case AO_ONCE_DESTROY:
					m_pGameObject->Death();
					break;
				case AO_COUNT_RETURN:
					break;
				case AO_COUNT_DESTROY:
					break;
				case AO_TIME_RETURN:
					break;
				case AO_TIME_DESTROY:
					break;				
				}
			}
		}
	}

	m_tCBuffer.eType = pClip->eType;
	m_tCBuffer.iFrameX = pClip->iFrameX;
	m_tCBuffer.iFrameY = pClip->iFrameY;
	m_tCBuffer.iFrameMaxX = pClip->iFrameMaxX;
	m_tCBuffer.iFrameMaxY = pClip->iFrameMaxY;

	// Renderer Component가 있다면
	if (true == m_pGameObject->CheckComponentFromTypeID<CRenderer>())
	{
		// Renderer에 Effect 상수버퍼 업데이트
		CRenderer*	pRenderer = m_pGameObject->FindComponentFromTypeID<CRenderer>();
		pRenderer->UpdateCBuffer("Animation2D", &m_tCBuffer);

		SAFE_RELEASE(pRenderer);
	}
	else if (true == m_pGameObject->CheckComponentFromTypeID<CRenderer2D>())
	{
		// Renderer2D에 Effect 상수버퍼 업데이트
		CRenderer2D*	pRenderer2D = m_pGameObject->FindComponentFromTypeID<CRenderer2D>();
		pRenderer2D->UpdateCBuffer("Animation2D", &m_tCBuffer);

		SAFE_RELEASE(pRenderer2D);
	}
}

void CAnimation2D::Collision(float _fTime)
{
}

void CAnimation2D::Render(float _fTime)
{
}

CAnimation2D * CAnimation2D::Clone()
{
	return new CAnimation2D(*this);
}

CAnimation2D::CAnimation2D() : m_iCurrentAnimation(0), m_iDefaultAnimation(0)
{
	SetTag("Animation2D");
	SetTypeName("CAnimation2D");
	SetTypeID<CAnimation2D>();
	m_eComponentType = CT_ANIMATION2D;
}

CAnimation2D::CAnimation2D(const CAnimation2D & _Animation2D) :
	CComponent(_Animation2D)
{
	m_iCurrentAnimation = _Animation2D.m_iCurrentAnimation;
	m_iDefaultAnimation = _Animation2D.m_iDefaultAnimation;

	m_vecAnimationClip.clear();
	size_t Size = _Animation2D.m_vecAnimationClip.size();
	for (size_t i = 0; i < Size; ++i)
	{
		pANIMATIONCLIP2D	pClip = new ANIMATIONCLIP2D();
				
		*pClip = *_Animation2D.m_vecAnimationClip[i];

		if (NULL != pClip->pTexture)
		{
			pClip->pTexture->AddRef();
		}

		m_vecAnimationClip.push_back(pClip);
	}
}

CAnimation2D::~CAnimation2D()
{
	size_t Size = m_vecAnimationClip.size();
	for (size_t i = 0; i < Size; ++i)
	{
		if (NULL != m_vecAnimationClip[i]->pTexture)
		{
			SAFE_RELEASE(m_vecAnimationClip[i]->pTexture);
		}

		Safe_Delete_VecList(m_vecAnimationClip);
	}
}