#include "PlayerHitEffect.h"
#include "06.GameObject/GameObject.h"
#include "07.Component/Transform.h"
#include "07.Component/Renderer.h"
#include "07.Component/Animation2D.h"
#include "07.Component/Effect.h"
#include "07.Component/Material.h"

CPlayerHitEffect::CPlayerHitEffect()
{
}

CPlayerHitEffect::CPlayerHitEffect(const CPlayerHitEffect & _PlayerHitEffect) :
	CScript(_PlayerHitEffect)
{
}


CPlayerHitEffect::~CPlayerHitEffect()
{
}

bool CPlayerHitEffect::Init()
{
	CRenderer* pRenderer = m_pGameObject->AddComponent<CRenderer>("EffectRenderer");
	pRenderer->SetMesh("PosMesh");
	pRenderer->SetShader(EFFECT_SHADER);
	pRenderer->SetInputLayout("PosInputLayout");
	pRenderer->SetRenderState(ALPHABLEND);
	SAFE_RELEASE(pRenderer);

	CEffect*	pEffect = m_pGameObject->AddComponent<CEffect>("Effect");
	SAFE_RELEASE(pEffect);

	CAnimation2D*	pAnimation2D = m_pGameObject->AddComponent<CAnimation2D>("EffectAnimation");
	pAnimation2D->AddAnimation2DClip("PlayerHitEffect", A2D_ATLAS, AO_ONCE_DESTROY, 
		7, 1, 1.0f, 0, 0.f, "PlayerHitEffect", 0, L"Effect/Stun.png");
	pAnimation2D->Start();
	SAFE_RELEASE(pAnimation2D);

	m_pTransform->SetWorldScale(2.0f, 2.0f, 2.0f);

	return true;
}

void CPlayerHitEffect::Update(float _fTime)
{
}

CPlayerHitEffect * CPlayerHitEffect::Clone()
{
	return new CPlayerHitEffect(*this);
}

void CPlayerHitEffect::SetPos(const DxVector3 & _vPos)
{
	m_pTransform->SetWorldPos(_vPos);
}


