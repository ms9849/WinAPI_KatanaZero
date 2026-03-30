#pragma once
#include "Define.h"
#include "CEffect.h"
#include "CFollowEffect.h"
#include "CRemainEffect.h"

class CEffectMgr
{
public:
	CEffectMgr();
	~CEffectMgr();

	CEffectMgr(const CEffectMgr& rhs) = delete;
	CEffectMgr& operator=(CEffectMgr& rhs) = delete;

public:
	void Start_Timer(); //Initialize 할 때 실행할 함수. 내부적으로 쿨다운을 가지는 이펙트들의 타이머를 초기화한다.

public:
	ULONGLONG Get_SlideTimer() { return m_tSlideEffectTimer; }
	void Set_SlideTimer() { m_tSlideEffectTimer = GetTickCount64(); }

	ULONGLONG Get_TrailTimer() { return m_tTrailEffectTimer; }
	void Set_TrailTimer() { m_tTrailEffectTimer = GetTickCount64(); }
public:
	void Add_Effect(CEffect* _pEffect) { m_EffectList.push_back(_pEffect); }
	void Add_Front_Effect(CEffect* _pEffect) { m_FrontEffectList.push_back(_pEffect); }
public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC hDC);
	void Render_Front(HDC hDC);
	void Release();
	
public:
	static CEffectMgr* Get_Instance()
	{
		if (nullptr == m_pInstance) m_pInstance = new CEffectMgr;
		return m_pInstance;
	}

	static void Destroy_Instance()
	{
		if (nullptr == m_pInstance) return;
		
		delete m_pInstance;
		m_pInstance = nullptr;
	}

private:
	static CEffectMgr* m_pInstance;
	
	list<CEffect*> m_EffectList;
	list<CEffect*> m_FrontEffectList;

	ULONGLONG m_tSlideEffectTimer;
	ULONGLONG m_tTrailEffectTimer;
};

