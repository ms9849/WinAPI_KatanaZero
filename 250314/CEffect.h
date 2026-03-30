#pragma once
#include "CObj.h"

class CEffect
{
public:
	CEffect() : m_bDead(false), m_bInfinity(false) {};
	//음.. 크기 설정을 어떻게 해주지?
	//RemainEffect 내부에서?
	//dust 출력이면 fcx =20 fcy = 20 해주던지 해야 하나..?
	CEffect(float _fX, float _fY)
	{
		m_tInfo.fX = _fX;
		m_tInfo.fY = _fY;
	}
	CEffect(float _fX, float _fY, const TCHAR* _pFrameKey) : m_bDead(false), m_bInfinity(false), m_bTrail(false)
	{
		m_tInfo.fX = _fX;
		m_tInfo.fY = _fY;
		m_pFrameKey = _pFrameKey;
	}

	CEffect(float _fX, float _fY, const TCHAR* _pFrameKey, FRAME _fFrame) : m_bDead(false), m_bInfinity(false), m_bTrail(false)
	{
		m_tInfo.fX = _fX;
		m_tInfo.fY = _fY;
		m_pFrameKey = _pFrameKey;
		m_tFrame = _fFrame;
	}

	virtual ~CEffect() {};

public:
	void Set_Infinity(bool _bFlag) { m_bInfinity = _bFlag; }
	//여러 이펙트들 출력할떄마다 기본으로 설정해줘야 할 메서드
	void Set_Size(float _fX, float _fY) { m_tInfo.fCX = _fX; m_tInfo.fCY = _fY; }
	//애니 끝났는지 확인하는 함수. 
	bool isAnimationEnd() { return m_tFrame.iStart == ((m_tFrame.iMotion == 0) ? m_tFrame.iEnd : 0); }
	void Update_Frame();
	bool Get_Dead() { return m_bDead; }

public:
	// 가상함수와 순수 가상함수의 차이

	// 순수 가상함수는 선언한 클래스에서 구현 필요 X
	// 하지만 상속받은 클래스에서는 반드시 구현해줘야 함

	// 가상함수는 선언한 클래스에서 구현 필요 O
	// 상속받은 클래스에서 반드시 구현할 필요가 없음
	virtual void Initialize() PURE;
	virtual int Update() PURE;
	virtual void Late_Update() PURE;
	virtual void Render(HDC hDC) PURE;
	virtual void Release() PURE;

protected:
	INFO		m_tInfo;
	FRAME		m_tFrame;
	const TCHAR* m_pFrameKey;
	
	bool		m_bTrail;
	bool		m_bInfinity;
	bool		m_bDead;
};

