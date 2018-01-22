#include "Animation3D.h"
#include "Animation3DClip.h"
#include "Renderer.h"
#include "../01.Core/PathMgr.h"
#include "../03.Resource/Texture.h"
#include "../06.GameObject/GameObject.h"

WOOJUN_USING

CAnimation3D::CAnimation3D() :
	m_pCurClip(NULL),
	m_pNextClip(NULL),
	m_pBoneTexture(NULL),
	m_bChange(false),
	m_fChangeLimitTime(0.2f),
	m_fChangeTime(0.0f),
	m_fAnimationTime(0.0f)
{
	SetTag("Animation3D");
	SetTypeName("CAnimation3D");
	SetTypeID<CAnimation3D>();
	m_eComponentType = CT_ANIMATION3D;
}

CAnimation3D::CAnimation3D(const CAnimation3D & _Animation3D) :
	CComponent(_Animation3D)
{
	m_vecBones.clear();
	for (size_t i = 0; i < _Animation3D.m_vecBones.size(); ++i)
	{
		pBONE	pBone = new BONE();
		pBone->strName = _Animation3D.m_vecBones[i]->strName;
		pBone->iDepth = _Animation3D.m_vecBones[i]->iDepth;
		pBone->iParentIndex = _Animation3D.m_vecBones[i]->iParentIndex;
		pBone->matOffset = new MATRIX();
		pBone->matBone = new MATRIX();
		*pBone->matOffset = *_Animation3D.m_vecBones[i]->matOffset;
		*pBone->matBone = *_Animation3D.m_vecBones[i]->matBone;

		for (size_t j = 0; j < _Animation3D.m_vecBones[i]->vecKeyFrame.size(); ++j)
		{
			pKEYFRAME	pFrame = new KEYFRAME();
			*pFrame = *_Animation3D.m_vecBones[i]->vecKeyFrame[j];
			pBone->vecKeyFrame.push_back(pFrame);
		}

		m_vecBones.push_back(pBone);
	}

	m_mapClip.clear();
	unordered_map<string, class CAnimation3DClip*>::const_iterator	iter;
	unordered_map<string, class CAnimation3DClip*>::const_iterator	iterEnd = _Animation3D.m_mapClip.end();

	for (iter = _Animation3D.m_mapClip.begin(); iter != iterEnd; ++iter)
	{
		CAnimation3DClip*	pClip = iter->second->Clone();
		m_mapClip.insert(make_pair(iter->first, pClip));
		SAFE_RELEASE(pClip);
	}

	m_iAnimationLimitFrame = _Animation3D.m_iAnimationLimitFrame;

	m_strDefaultClip = _Animation3D.m_strDefaultClip;
	m_strCurClip = _Animation3D.m_strCurClip;

	m_pCurClip = FindClip(m_strCurClip);
	m_pNextClip = NULL;

	m_bChange = false;
	m_fChangeTime = 0.0f;
	m_fAnimationTime = 0.0f;
	m_fChangeLimitTime = _Animation3D.m_fChangeLimitTime;

	m_fFrameTime = _Animation3D.m_fFrameTime;

	m_pBoneTexture = NULL;

	CreateBoneTexture();
}

CAnimation3D::~CAnimation3D()
{
	for (size_t i = 0; i < m_vecBones.size(); ++i)
	{
		SAFE_DELETE(m_vecBones[i]->matBone);
		SAFE_DELETE(m_vecBones[i]->matOffset);
		Safe_Delete_VecList(m_vecBones[i]->vecKeyFrame);
	}

	SAFE_RELEASE(m_pBoneTexture);
	SAFE_RELEASE(m_pNextClip);
	SAFE_RELEASE(m_pCurClip);
	Safe_Release_Map(m_mapClip);
	Safe_Delete_VecList(m_vecBones);
}

CTexture * CAnimation3D::GetBoneTexture() const
{
	if (m_pBoneTexture)
	{
		m_pBoneTexture->AddRef();
	}

	return m_pBoneTexture;
}

void CAnimation3D::SetChangeTime(float _fTime)
{
	m_fChangeLimitTime = _fTime;
}

void CAnimation3D::AddBone(pBONE _pBone)
{
	m_vecBones.push_back(_pBone);
}

void CAnimation3D::AddClip(ANIMATION_OPTION _eOption, pFBXANIMATIONCLIP _pClip)
{
	CAnimation3DClip*	pAniClip = FindClip(_pClip->strName);

	if (pAniClip)
	{
		SAFE_RELEASE(pAniClip);
		return;
	}
	pAniClip = new CAnimation3DClip();
	pAniClip->SetClipInfo(_eOption, _pClip);

	if (!pAniClip->Init())
	{
		SAFE_RELEASE(pAniClip);
		return;
	}

	switch (_pClip->eTimeMode)
	{
	case FbxTime::eFrames24:
		m_iAnimationLimitFrame = 24;
		break;
	case FbxTime::eFrames30:
		m_iAnimationLimitFrame = 30;
		break;
	case FbxTime::eFrames60:
		m_iAnimationLimitFrame = 60;
		break;
	}

	m_fFrameTime = 1.0f / m_iAnimationLimitFrame;

	if (m_mapClip.empty())
	{
		SetDefaultClipName(_pClip->strName);
		SetCurClipName(_pClip->strName);
		m_pCurClip = pAniClip;
		pAniClip->AddRef();
	}

	m_mapClip.insert(make_pair(_pClip->strName, pAniClip));
}

void CAnimation3D::AddClip(const string & _strKey, ANIMATION_OPTION _eOption, int _iAnimationLimitFrame, int _iStartFrame, int _iEndFrame, float _fStartTime, float _fEndTime)
{
	CAnimation3DClip*	pAniClip = FindClip(_strKey);

	if (pAniClip)
	{
		SAFE_RELEASE(pAniClip);
		return;
	}

	m_iAnimationLimitFrame = _iAnimationLimitFrame;

	pAniClip = new CAnimation3DClip();
	pAniClip->SetClipInfo(_strKey, _eOption, _iAnimationLimitFrame,
		_iStartFrame, _iEndFrame, _fStartTime, _fEndTime);

	if (!pAniClip->Init())
	{
		SAFE_RELEASE(pAniClip);
		return;
	}

	m_fFrameTime = 1.0f / m_iAnimationLimitFrame;

	if (m_mapClip.empty())
	{
		SetDefaultClipName(_strKey);
		SetCurClipName(_strKey);
		m_pCurClip = pAniClip;
		pAniClip->AddRef();
	}

	m_mapClip.insert(make_pair(_strKey, pAniClip));
}

CAnimation3DClip * CAnimation3D::FindClip(const string & _strKey)
{
	unordered_map<string, class CAnimation3DClip*>::iterator	iter = m_mapClip.find(_strKey);

	if (iter == m_mapClip.end())
	{
		return NULL;
	}

	iter->second->AddRef();

	return iter->second;
}

void CAnimation3D::SetDefaultClipName(const string & _strName)
{
	m_strDefaultClip = _strName;
}

void CAnimation3D::SetCurClipName(const string & _strName)
{
	m_strCurClip = _strName;
	SAFE_RELEASE(m_pCurClip);
	m_pCurClip = FindClip(_strName);
}

void CAnimation3D::ChangeClip(const string & _strName)
{
	if (!m_bChange && m_strCurClip == _strName)
	{
		return;
	}
	else if (m_bChange && m_strNextClip == _strName)
	{
		return;
	}

	m_strNextClip = _strName;
	m_pNextClip = FindClip(_strName);

	m_bChange = true;
	m_fChangeTime = 0.0f;
}

bool CAnimation3D::CreateBoneTexture()
{
	SAFE_RELEASE(m_pBoneTexture);

	m_pBoneTexture = CTexture::CreateTexture("BoneTexture", m_vecBones.size() * 4, 1, 1,
		DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_USAGE_DYNAMIC, D3D11_BIND_SHADER_RESOURCE, D3D11_CPU_ACCESS_WRITE);	
	
	if (!m_pBoneTexture)
	{
		return false;
	}

	return true;
}

void CAnimation3D::Save(const char * _pFileName, const string & _strPathKey)
{
	// 풀경로를 만든다
	const char*	pPath = GET_SINGLE(CPathMgr)->FindPathToMultiByte(_strPathKey);

	string	strPath;
	if (pPath)
	{
		strPath = pPath;
	}

	strPath += _pFileName;

	return SaveFromFullPath(strPath.c_str());
}

void CAnimation3D::SaveFromFullPath(const char * _pFileName)
{
	FILE*	pFile = NULL;

	fopen_s(&pFile, _pFileName, "wb");

	if (!pFile)
	{
		return;
	}

	int iSize = m_vecBones.size();

	// 본 갯수 저장
	fwrite(&iSize, 4, 1, pFile);

	for (int i = 0; i < iSize; ++i)
	{
		// 이름 저장
		int iLength = m_vecBones[i]->strName.length();

		fwrite(&iLength, 4, 1, pFile);
		fwrite(m_vecBones[i]->strName.c_str(), 1, iLength, pFile);

		// 깊이 저장
		fwrite(&m_vecBones[i]->iDepth, 4, 1, pFile);
		fwrite(&m_vecBones[i]->iParentIndex, 4, 1, pFile);

		for (int j = 0; j < 4; ++j)
		{
			for (int k = 0; k < 4; ++k)
			{
				fwrite(&m_vecBones[i]->matOffset->m[i][j], 4, 1, pFile);
			}
		}

		for (int j = 0; j < 4; ++j)
		{
			for (int k = 0; k < 4; ++k)
			{
				fwrite(&m_vecBones[i]->matBone->m[i][j], 4, 1, pFile);
			}
		}

		int iKeyCount = m_vecBones[i]->vecKeyFrame.size();

		fwrite(&iKeyCount, 4, 1, pFile);

		for(int j = 0; j < iKeyCount; ++j)
		{

		}
	}

	fclose(pFile);
}

void CAnimation3D::Load(const char * _pFileName, const string & _strPathKey)
{
}

void CAnimation3D::LoadFromFullPath(const char * _pFileName)
{
}

bool CAnimation3D::Init()
{
	return true;
}

void CAnimation3D::Input(float _fTime)
{
}

void CAnimation3D::Update(float _fTime)
{
	vector<MATRIX>		vecBones;

	// 모션이 바뀌고있는 중이라면 기존에 동작되던 모션과
	// 교체될 모션을 보간한다. (자연스럽게 처리하기 위해)
	if (m_bChange)
	{
		m_fChangeTime += _fTime;

		if (m_fChangeTime >= m_fChangeLimitTime)
		{
			m_fChangeTime = m_fChangeLimitTime;
			m_bChange = false;
		}

		float	fAnimationTime = m_fAnimationTime + m_pCurClip->m_tInfo.fStartTime;

		vecBones.reserve(m_vecBones.size());

		// 본 수만큼 반복한다.
		for (size_t i = 0; i < m_vecBones.size(); ++i)
		{
			// 키프레임이 없을경우
			if (m_vecBones[i]->vecKeyFrame.empty())
			{
				vecBones.push_back(*m_vecBones[i]->matBone);
				continue;
			}

			int iFrameIndex = (int)(fAnimationTime * m_iAnimationLimitFrame);
			int iNextFrameIndex = m_pNextClip->m_tInfo.iStartFrame;

			const pKEYFRAME	pCurKey = m_vecBones[i]->vecKeyFrame[iFrameIndex];
			const pKEYFRAME pNextKey = m_vecBones[i]->vecKeyFrame[iNextFrameIndex];

			float	fPercent = m_fChangeTime / m_fChangeLimitTime;

			XMVECTOR	vScale = XMVectorLerp(pCurKey->vScale.Convert(), pNextKey->vScale.Convert(), fPercent);
			XMVECTOR	vPos = XMVectorLerp(pCurKey->vPos.Convert(), pNextKey->vPos.Convert(), fPercent);
			XMVECTOR	vRot = XMQuaternionSlerp(pCurKey->vRot.Convert(), pNextKey->vRot.Convert(), fPercent);
			XMVECTOR	vZero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

			MATRIX		matBone = XMMatrixAffineTransformation(vScale, vZero, vRot, vPos);
			matBone = *m_vecBones[i]->matOffset * matBone;

			vecBones.push_back(matBone);
		}

		if (!m_bChange)
		{
			m_strCurClip = m_strNextClip;

			SAFE_RELEASE(m_pCurClip);
			m_pCurClip = m_pNextClip;
			m_pCurClip->AddRef();

			SAFE_RELEASE(m_pNextClip);
			m_strNextClip = "";
			m_fAnimationTime = 0.0f;
		}
	}
	else
	{
		m_fAnimationTime += _fTime;

		while (m_fAnimationTime >= m_pCurClip->m_tInfo.fTimeLength)
		{
			m_fAnimationTime -= m_pCurClip->m_tInfo.fTimeLength;
		}

		float fAnimationTime = m_fAnimationTime + m_pCurClip->m_tInfo.fStartTime;

		int iStartFrame = m_pCurClip->m_tInfo.iStartFrame;
		int iEndFrame = m_pCurClip->m_tInfo.iEndFrame;

		vecBones.reserve(m_vecBones.size());

		// 본 수만큼 반복한다.
		for (size_t i = 0; i < m_vecBones.size(); ++i)
		{
			// 키프레임이 없을경우
			if (m_vecBones[i]->vecKeyFrame.empty())
			{
				vecBones.push_back(*m_vecBones[i]->matBone);
				continue;
			}

			int iFrameIndex = (int)(fAnimationTime * m_iAnimationLimitFrame);
			if (iFrameIndex >= iEndFrame)
			{
				switch (m_pCurClip->m_tInfo.eOption)
				{
				case AO_LOOP:
					iFrameIndex = iStartFrame;
					break;
				case AO_ONCE_RETURN:
					ChangeClip(m_strDefaultClip);
					break;
				case AO_ONCE_DESTROY:
					m_pGameObject->Death();
					break;
				}
			}

			int iNextFrameIndex = iFrameIndex + 1;
			if (iNextFrameIndex >= iEndFrame)
			{
				iNextFrameIndex = iStartFrame;
			}

			const pKEYFRAME	pCurKey = m_vecBones[i]->vecKeyFrame[iFrameIndex];
			const pKEYFRAME pNextKey = m_vecBones[i]->vecKeyFrame[iNextFrameIndex];

			// 현재 프레임의 시간을 얻어온다.
			double	dFrameTime = pCurKey->dTime;

			float	fPercent = (fAnimationTime - dFrameTime) / m_fFrameTime;

			XMVECTOR	vScale = XMVectorLerp(pCurKey->vScale.Convert(), pNextKey->vScale.Convert(), fPercent);
			XMVECTOR	vPos = XMVectorLerp(pCurKey->vPos.Convert(), pNextKey->vPos.Convert(), fPercent);
			XMVECTOR	vRot = XMQuaternionSlerp(pCurKey->vRot.Convert(), pNextKey->vRot.Convert(), fPercent);
			XMVECTOR	vZero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

			MATRIX		matBone = XMMatrixAffineTransformation(vScale, vZero, vRot, vPos);
			matBone = *m_vecBones[i]->matOffset * matBone;

			vecBones.push_back(matBone);

			int a = 0;
		}
	}

	m_pBoneTexture->UpdateData(&vecBones[0], sizeof(MATRIX) * vecBones.size());
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
