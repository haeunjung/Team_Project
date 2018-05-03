#include "HitEffect.h"
#include "06.GameObject/GameObject.h"
#include "07.Component/Transform.h"
#include "07.Component/Renderer.h"
#include "07.Component/Animation2D.h"
#include "07.Component/Effect.h"
#include "07.Component/Material.h"

CHitEffect::CHitEffect()
{
}

CHitEffect::CHitEffect(const CHitEffect & _HitEffect) :
	CScript(_HitEffect)
{
}


CHitEffect::~CHitEffect()
{
}

bool CHitEffect::Init()
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
	pAnimation2D->AddAnimation2DClip("HitEffect", A2D_ATLAS, AO_ONCE_DESTROY, 
		4, 1, 1.f, 0, 0.f, "HitEffect", 0, L"Effect/MonsterHit.png");
	pAnimation2D->Start();
	SAFE_RELEASE(pAnimation2D);

	return true;
}

void CHitEffect::Update(float _fTime)
{
}

CHitEffect * CHitEffect::Clone()
{
	return new CHitEffect(*this);
}

void CHitEffect::SetPos(const DxVector3 & _vPos)
{
	m_pTransform->SetWorldPos(_vPos);
}


