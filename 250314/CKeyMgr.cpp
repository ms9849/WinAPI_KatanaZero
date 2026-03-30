#include "pch.h"
#include "CKeyMgr.h"

CKeyMgr* CKeyMgr::m_pInstance = nullptr;

CKeyMgr::CKeyMgr()
{
	ZeroMemory(m_bKeyState, sizeof(m_bKeyState));
}

CKeyMgr::~CKeyMgr()
{
}

bool CKeyMgr::Key_Pressing(int _iKey)
{
	if (GetAsyncKeyState(_iKey) & 0x8000)
		return true;

	return false;
}

// 이전 프레임에 눌린 적이 없고, 지금 막 눌렀을 때
bool CKeyMgr::Key_Down(int _iKey)
{
	if ((!m_bKeyState[_iKey]) && (GetAsyncKeyState(_iKey) & 0x8000))
	{
		m_bKeyState[_iKey] = !m_bKeyState[_iKey];
		return true;
	}

	return false;
}


bool CKeyMgr::Key_Up(int _iKey)
{
	if ((m_bKeyState[_iKey]) && !(GetAsyncKeyState(_iKey) & 0x8000))
	{
		m_bKeyState[_iKey] = !m_bKeyState[_iKey];
		return true;
	}

	return false;
}

void CKeyMgr::Update()
{
	for (int i = 0; i < VK_MAX; ++i)
	{
		if ((m_bKeyState[i]) && !(GetAsyncKeyState(i) & 0x8000))
			m_bKeyState[i] = !m_bKeyState[i];

		if ((!m_bKeyState[i]) && (GetAsyncKeyState(i) & 0x8000))
			m_bKeyState[i] = !m_bKeyState[i];
	}
}
