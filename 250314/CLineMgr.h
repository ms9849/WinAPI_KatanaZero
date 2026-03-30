#pragma once
#include "Define.h"
#include "CLine.h"

class CLineMgr
{
public:
	enum DIRECTION { LEFT, RIGHT, END};

private:
	CLineMgr();
	~CLineMgr();


public:
	void	Save_Line(const TCHAR* _filePath);
	void	Load_Line(const TCHAR* _filePath);
	const list<CLine*>& Get_LineList() const { return m_Linelist; }

public:
	void	Set_Show() { m_bShowLine = !m_bShowLine; }
	void	Add_Line(CLine* _pLine) { m_Linelist.push_back(_pLine); }
public:
	void	Initialize();
	int		Update();
	void	Late_Update();
	void	Render(HDC hDC);
	void	Release();

public:
	static CLineMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CLineMgr;
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
	static CLineMgr* m_pInstance;

	list<CLine*>	m_Linelist;
	LINEPOINT		m_tPoint[END];
	bool	m_bShowLine;
};

