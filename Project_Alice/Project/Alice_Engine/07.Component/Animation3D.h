#pragma once
#include "Component.h"
#include "../03.Resource/FbxLoader.h"
#include "Animation3DClip.h"

WOOJUN_BEGIN

typedef struct DLL _tagKeyFrame
{
	double	dTime;
	DxVector3	vPos;
	DxVector3	vScale;
	DxVector4	vRot;
}KEYFRAME, *pKEYFRAME;

typedef struct DLL _tagBone
{
	string		strName;
	int			iDepth;
	int			iParentIndex;
	pMATRIX		matOffset;
	pMATRIX		matBone;
	vector<pKEYFRAME>	vecKeyFrame;
}BONE, *pBONE;

class DLL CAnimation3D :
	public CComponent
{
private:
	friend class CGameObject;
	friend class CMesh;
private:
	vector<pBONE>		m_vecBones;
	class CTexture*		m_pBoneTexture;

	unordered_map<string, class CAnimation3DClip*>	m_mapClip;
	class CAnimation3DClip*	m_pCurClip;
	class CAnimation3DClip* m_pNextClip;
	int		m_iAnimationLimitFrame;
	string	m_strDefaultClip;
	string	m_strCurClip;
	string	m_strNextClip;
	bool	m_bChange;
	bool	m_bEnd;
	float	m_fChangeTime;
	float	m_fChangeLimitTime;
	float	m_fAnimationTime;
	float	m_fFrameTime;
	float	m_fAnimationProgress;
public:
	const string& GetDefaultClipName() const;
	unordered_map<string, class CAnimation3DClip*>* GetClips();
	float GetChangeLimitTime() const;
	bool GetAnimationEnd() const;
	float GetAnimationProgress() const;
	int GetAnimationProgressFrame() const;
	pBONE FindBone(const string& _strName) const;
public:
	void AddClipCallback(const string& _strName, int _iFrame, void(*_pFunc)(float));
	template <typename T>
	void AddClipCallback(const string& _strName, int _iFrame, T* _pObj, void(T::*_pFunc)(float))
	{
		CAnimation3DClip*	pClip = FindClip(_strName);

		if (!pClip)
		{
			return;
		}

		pClip->AddCallback(_iFrame, _pObj, _pFunc);
	}

	void AddClipCallback(const string& _strName, float _fProgress, void(*_pFunc)(float));
	template <typename T>						 
	void AddClipCallback(const string& _strName, float _fProgress, T* _pObj, void(T::*_pFunc)(float))
	{
		CAnimation3DClip*	pClip = FindClip(_strName);

		if (!pClip)
		{
			return;
		}

		pClip->AddCallback(_fProgress, _pObj, _pFunc);
	}
public:	
	void ReturnToDefaultClip();
	void ClearClip();
	class CTexture* GetBoneTexture() const;
	void SetChangeTime(float _fTime);
	void AddBone(pBONE _pBone);
	void AddClip(ANIMATION_OPTION _eOption, pFBXANIMATIONCLIP _pClip);
	void AddClip(const string& _strKey, ANIMATION_OPTION _eOption,
		int _iAnimationLimitFrame, int _iStartFrame, int _iEndFrame,
		float _fStartTime, float _fEndTime);
	void AddClip(const string& _strKey, ANIMATION_OPTION _eOption,
		int _iStartFrame, int _iEndFrame);
	void ChangeClipInfo(const string& _strKey, const string& _strReplaceKey, ANIMATION_OPTION _eOption,
		int _iStartFrame, int _iEndFrame);
	class CAnimation3DClip* FindClip(const string& _strKey);
	void SetDefaultClipName(const string& _strName);
	void SetCurClipName(const string& _strName);
	void ChangeClip(const string& _strName);
	bool CreateBoneTexture();
	bool CheckClipName(const string& _strName);

	bool Save(const char* _pFileName, const string& _strPathKey = MESHPATH);
	bool Save(FILE* _pFile);
	bool SaveFromFullPath(const char* _pFileName);	
	bool Load(const char* _pFileName, const string& _strPathKey = MESHPATH);
	bool Load(FILE* _pFile);
	bool LoadFromFullPath(const char* _pFileName);
public:
	bool Init() override ;
	void Input(float _fTime) override;
	void Update(float _fTime) override;
	void LateUpdate(float _fTime) override;
	void Collision(float _fTime) override;
	void Render(float _fTime) override;
	CAnimation3D* Clone() override;
private:
	CAnimation3D();
	CAnimation3D(const CAnimation3D& _Animation3D);
	~CAnimation3D();
};

WOOJUN_END