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
		// �ϳ��� �����ִ� ���
		if (true == FindIter->second[i]->GetClick())
		{
			bAllFalse = true;

			// �����ִ¾ְ� ���޹��� �����ΰ�� 
			// �¸� �ѳ����Ŵ�
			if (_pRadioButton == FindIter->second[i])
			{
				// 0�� ��� �������·� ����ϱ� ����
				// ���κ��̽����� +1
				FindIndexIter->second = i + 1;
				bAllFalse = true;
				continue;
			}
		}		

		// ������ �� ����
		FindIter->second[i]->RadioButtonOff();
	}

	// ���� ���� ���
	if (false == bAllFalse)
	{
		//��� �������¸�
		// Index�� 0�̴�
		FindIndexIter->second = 0;
	}
}
