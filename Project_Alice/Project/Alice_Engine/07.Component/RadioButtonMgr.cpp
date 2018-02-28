#include "RadioButtonMgr.h"

WOOJUN_USING

DEFINITION_SINGLE(CRadioButtonMgr)

CRadioButtonMgr::CRadioButtonMgr()
{
}

CRadioButtonMgr::~CRadioButtonMgr()
{
	unordered_map<string, vector<CRadioButton*>>::iterator	iter;

	for (iter = m_RadioButtonMgr.begin(); iter != m_RadioButtonMgr.end(); ++iter)
	{
		Safe_Release_VecList(iter->second);
		iter->second.clear();
	}

	m_RadioButtonMgr.clear();
}

int CRadioButtonMgr::GetRadioButtonMgrIndex(const string _strKey)
{
	unordered_map<string, int>::iterator FindIndexIter = m_RadioButtonMgrIndex.find(_strKey);

	if (m_RadioButtonMgrIndex.end() == FindIndexIter)
	{
		return -1;
	}

	return FindIndexIter->second;
}

void CRadioButtonMgr::CreateRadioButtonMgr(const string & _strKey, CRadioButton* _pRadioButton)
{	
	vector<CRadioButton*>	vecRadioButton;		
	vecRadioButton.push_back(_pRadioButton);	

	m_RadioButtonMgr.insert(make_pair(_strKey, vecRadioButton));
	m_RadioButtonMgrIndex.insert(make_pair(_strKey, 0));
}

void CRadioButtonMgr::PushRadioButton(const string & _strKey, CRadioButton * _pRadioButton)
{
	_pRadioButton->AddRef();

	unordered_map<string, vector<CRadioButton*>>::iterator FindIter = m_RadioButtonMgr.find(_strKey);
	if (m_RadioButtonMgr.end() == FindIter)
	{
		CreateRadioButtonMgr(_strKey, _pRadioButton);
		return;
	}
	
	FindIter->second.push_back(_pRadioButton);
}

void CRadioButtonMgr::ButtonCheck(const string & _strKey, CRadioButton* const _pRadioButton)
{
	unordered_map<string, vector<CRadioButton*>>::iterator FindIter = m_RadioButtonMgr.find(_strKey);
	if (m_RadioButtonMgr.end() == FindIter)
	{
		return;
	}

	unordered_map<string, int>::iterator FindIndexIter = m_RadioButtonMgrIndex.find(_strKey);
	bool bAllFalse = false;

	size_t Size = FindIter->second.size();
	for (size_t i = 0; i < Size; ++i)
	{
		// 하나라도 켜져있는 경우
		if (true == FindIter->second[i]->GetClick())
		{
			bAllFalse = true;

			// 켜져있는애가 전달받은 인자인경우 
			// 걔만 켜놓을거다
			if (_pRadioButton == FindIter->second[i])
			{
				// 0을 모두 꺼진상태로 사용하기 위해
				// 제로베이스에서 +1
				FindIndexIter->second = i + 1;
				bAllFalse = true;
				continue;
			}
		}		

		// 나머지 다 꺼라
		FindIter->second[i]->RadioButtonOff();
	}

	// 전부 꺼진 경우
	if (false == bAllFalse)
	{
		//모두 꺼진상태면
		// Index는 0이다
		FindIndexIter->second = 0;
	}
}
