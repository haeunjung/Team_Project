#pragma once

WOOJUN_BEGIN

class DLL CThreadSync
{
private:
	friend class CThreadMgr;
private:
	static CRITICAL_SECTION		m_CS;
public:
	CThreadSync()
	{
		// 임계영역 들어간다
		EnterCriticalSection(&m_CS);
	}
	~CThreadSync()
	{
		// 임계영역 나간다
		LeaveCriticalSection(&m_CS);
	}
};

template <typename T, int SIZE = 100>
class DLL CThreadQueue
{
private:
	T		m_Data[SIZE];
	int		m_iHead;
	int		m_iTail;
public:
	void Push(const T& _Data)
	{
		CThreadSync	Sync;

		int iTail = (m_iTail + 1) & SIZE;

		if (iTail == m_iHead)
		{
			return;
		}

		m_Data[iTail] = _Data;
		m_iTail = iTail;
	}

	bool Pop(T& _Data)
	{
		CThreadSync	Sync;

		// 꼬리랑 머리랑 같으면 비어있는거지
		if (m_iHead == m_iTail)
		{
			return false;
		}

		int iHead = (m_iHead + 1) & SIZE;

		_Data = m_Data[iHead];
		m_iHead = iHead;
	}
public:
	CThreadQueue()
	{
		m_iHead = 0;
		m_iTail = 0;
	}

	~CThreadQueue()
	{
	}
};

WOOJUN_END