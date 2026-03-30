#pragma once

#include "Define.h"

class CKeyMgr
{
private:
	CKeyMgr();
	~CKeyMgr();

public:
	bool		Key_Pressing(int _iKey);
	// 눌렀다가 뗐을 때
	bool		Key_Up(int _iKey);
	// 누른 타이밍 딱 한 번 만
	bool		Key_Down(int _iKey);

	void		Update();

public:
	static CKeyMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CKeyMgr;
		}

		return m_pInstance;
	}

	static void	Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static CKeyMgr* m_pInstance;
	bool		m_bKeyState[VK_MAX];
};

