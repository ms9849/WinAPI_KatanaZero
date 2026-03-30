#pragma once
#include "CUI.h"

class CUIMgr
{
public:
	CUIMgr();
	~CUIMgr();
	
public:
	void Add_UI(CUI* _pUI) { m_vecUI.push_back(_pUI); }
	void Delete_Focused_UI() 
	{ 
		Safe_Delete(m_vecUI.back());
		m_vecUI.pop_back(); 
	}
public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC hDC);
	void Release();

public:
	static CUIMgr* Get_Instance()
	{
		if (m_pInstance == nullptr) m_pInstance = new CUIMgr;
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		if (m_pInstance == nullptr) return;

		delete m_pInstance;
		m_pInstance = nullptr;
	}
public:
	static CUIMgr* m_pInstance;
	vector<CUI*> m_vecUI;
};

