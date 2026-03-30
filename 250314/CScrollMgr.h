#pragma once
#include "Define.h"
#include "CScrollMemento.h"

class CScrollMgr
{
public:
	CScrollMgr();
	virtual ~CScrollMgr();

	CScrollMgr(const CScrollMgr& rhs) = delete;
	CScrollMgr& operator=(CScrollMgr& rhs) = delete;
public:

	CScrollMemento* Create_SnapShot() {
		return new CScrollMemento(m_fScrollX, m_fScrollY);
	}

	void Rollback(CScrollMemento* pMemento)
	{
		m_fScrollX = pMemento->m_fScrollX;
		m_fScrollY = pMemento->m_fScrollY;
	}


public:
	void Late_Update();
	void Shake(float _fIntensity, float _fTime);
	void Set_ScrollPos(float _fX, float _fY) { m_fScrollX = _fX; m_fScrollY = _fY; }
	void Set_ScrollX(float _fX) { m_fScrollX += _fX; }
	void Set_ScrollMinX(float _fX) { m_fMinX = _fX; }
	void Set_ScrollMaxX(float _fX) { m_fMaxX = _fX; }

	void Set_ScrollY(float _fY) { m_fScrollY += _fY; }
	void Set_ScrollMinY(float _fY) { m_fMinY = _fY; }
	void Set_ScrollMaxY(float _fY) { m_fMaxY = _fY; }

	void Shake(float _fIntensity) {
		//내일 구현하자 지친다..
	}

	float Get_ScrollX() { return m_fScrollX; }
	float Get_ScrollY() { return m_fScrollY; }

	void Scroll_Lock();

	static CScrollMgr* Get_Instance()
	{
		if (!m_pInstance) 
			m_pInstance = new CScrollMgr;
		return m_pInstance;
	}

	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static CScrollMgr* m_pInstance;
	float m_fScrollX;
	float m_fScrollY;

	float m_fBeforeScrollX;
	float m_fBeforeScrollY;

	float m_fMinX;
	float m_fMaxX;

	float m_fMinY;
	float m_fMaxY;

	float m_fTime;
	float m_fStartTime;
	float m_fIntensity;
	bool m_bShaking;
};

