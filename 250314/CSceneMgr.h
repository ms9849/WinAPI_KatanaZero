#pragma once
#include "CScene.h"
#include "CMenu.h"
#include "CStage.h"
#include "CStage2.h"
#include "CStage3.h"
#include "CStage4.h"
#include "CEndScene.h"
#include "CSoundMgr.h"
#include "CBmpMgr.h"

class CSceneMgr
{
public:
	enum SCENEID { SC_MENU, SC_STAGE, SC_STAGE_2, SC_STAGE_3, SC_STAGE_4, SC_ENDING, SC_END };
public:
	CSceneMgr();
	virtual ~CSceneMgr();

	CSceneMgr(const CSceneMgr& rhs) = delete;
	CSceneMgr& operator=(CSceneMgr& rhs) = delete;
	
public:
	CScene* Get_CurScene() { return m_pScene; } // 현재 가지고 있는 신 정보를 출력. 생성 시간은 상위 클래스에 둘까?

public:
	void Replay_Filter(HDC hDC);
	void End_Filter(HDC hDC);

public:
	void	Scene_Change(SCENEID _eId);
	void	Update();
	void	Late_Update();
	void	Render(HDC hDC);
	void	Release();

	static CSceneMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CSceneMgr;
		
		return m_pInstance;
	}

	static void Destroy_Instance()
	{
		if (!m_pInstance) return;
		else delete m_pInstance;
	}

protected:
	static CSceneMgr* m_pInstance;
	CScene* m_pScene; // 현재 가지고 있는 씬. 이걸 끌어다 써서 Hud 만들면 될 것 같은데?

	SCENEID m_eCurScene;
	SCENEID m_ePreScene;

	ULONGLONG m_fFilterTimer;
	int m_iFilterCounter;
};

