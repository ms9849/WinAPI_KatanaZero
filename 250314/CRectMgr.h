#pragma once
#include "Define.h"
#include "CRect.h"

class CRect;

class CRectMgr
{
public :
	CRectMgr();
	~CRectMgr();

public:
	void	Add_Rect(CRect* _pRect) { m_Rectlist.push_back(_pRect); }

public:
	bool	Get_Show() { return m_bShowRect; }
	void	Set_Show() { m_bShowRect = !m_bShowRect; }

public:
	void	Initialize();
	void	Update();
	void	Late_Update();
	void	Render(HDC hDC);
	void	Release();

	const list<CRect*>& Get_RectList() { return m_Rectlist; }
public:
	static CRectMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CRectMgr;
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
	static CRectMgr*	m_pInstance;

	RECT rectSize{};
	list<CRect*>	m_Rectlist;
	bool		m_bShowRect;
};

