#pragma once
#include "05.Scene/Layer.h"

WOOJUN_USING

class CMinion;
class CMinionMgr
{
private:
	list<CMinion*> m_listMinion;
	list<CMinion*> m_listRespawn;
public:
	bool CheckMinion(CMinion* _pMinion);
	bool PushMinion(CMinion* _pMinion);
	bool EraseMinion(CMinion* _pMinion); 
	void PushRespawnList(CMinion* _pMinion);
	void EraseRespawnList(CMinion* _pMinion);
	void MinionListDistCheck();
	void Update(float _fTime);

	DECLARE_SINGLE(CMinionMgr)
};

