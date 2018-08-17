#include "MinionMgr.h"
#include "../ObjectScript/Minion.h"

DEFINITION_SINGLE(CMinionMgr)

CMinionMgr::CMinionMgr()
{
}

CMinionMgr::~CMinionMgr()
{
	Safe_Release_VecList(m_listMinion);
	Safe_Release_VecList(m_listRespawn);
}

bool CMinionMgr::CheckMinion(CMinion * _pMinion)
{
	list<CMinion*>::iterator iter = m_listMinion.begin();
	list<CMinion*>::iterator iterEnd = m_listMinion.end();

	for (iter; iter != iterEnd; ++iter)
	{
		if ((*iter) == _pMinion)
		{
			return false;
		}
	}

	return true;
}

bool CMinionMgr::PushMinion(CMinion * _pMinion)
{
	if (!CheckMinion(_pMinion))
	{
		return false;
	}

	_pMinion->AddRef();
	m_listMinion.push_back(_pMinion);

	return true;
}

bool CMinionMgr::EraseMinion(CMinion * _pMinion)
{
	if (CheckMinion(_pMinion))
	{
		return false;
	}

	m_listMinion.remove(_pMinion);
	SAFE_RELEASE(_pMinion);

	return true;
}

void CMinionMgr::PushRespawnList(CMinion * _pMinion)
{
	_pMinion->AddRef();
	m_listRespawn.push_back(_pMinion);
}

void CMinionMgr::EraseRespawnList(CMinion * _pMinion)
{
	m_listRespawn.remove(_pMinion);
	SAFE_RELEASE(_pMinion);
}

void CMinionMgr::MinionListDistCheck()
{
	list<CMinion*>::iterator iter = m_listMinion.begin();
	list<CMinion*>::iterator iterEnd = m_listMinion.end();

	CMinion* pMinion = (*iter);
	float fDist = pMinion->DistCheckFromPlayer();
	for (iter; iter != iterEnd; ++iter)
	{		
		if (fDist > (*iter)->DistCheckFromPlayer())
		{
			fDist = (*iter)->DistCheckFromPlayer();
			pMinion = (*iter);
		}
	}

	MONSTER_STATE State = pMinion->GetState();
	if (MS_DEFAULT == State || MS_WALK == State)
	{
		pMinion->TracePlayer();
	}	
}

void CMinionMgr::Update(float _fTime)
{
	list<CMinion*>::iterator iter = m_listRespawn.begin();
	list<CMinion*>::iterator iterEnd = m_listRespawn.end();

	for (iter; iter != iterEnd; ++iter)
	{
		if ((*iter)->RespawnUpdate(_fTime))
			break;
	}
}

void CMinionMgr::Release()
{
	Safe_Release_VecList(m_listMinion);
	Safe_Release_VecList(m_listRespawn);
}
