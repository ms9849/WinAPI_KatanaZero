#pragma once

#include "Define.h"
#include "CObjMemento.h"

class CMap;

class CObj
{
protected:
	enum OBJTYPE { OBJ, PLAYER, BULLET, MONSTER, ITEM, HEAD, FANBLADE };
	//아이템은 일단 식칼 하나만 만들어서.. 두 세개 배치할까?
public:
	CObj();
	CObj(float _fX, float _fY, float _fAngle) : m_fAngle(_fAngle) { m_tInfo.fX = _fX; m_tInfo.fY = _fY; }
	virtual ~CObj();

public:
	CObjMemento* Create_SnapShot() {
		return new CObjMemento(m_tSpriteInfo, m_tFrame, m_pFrameKey);
	}
	void Rollback(CObjMemento* pMemento)
	{
		m_tSpriteInfo = pMemento->m_tSpriteInfo;
		m_tFrame = pMemento->m_tFrame;
		m_pFrameKey = pMemento->m_pFrameKey;
	}

public:
	bool isAnimationEnd() { return m_tFrame.iStart == ((m_tFrame.iMotion == 0) ? m_tFrame.iEnd : 0); }
	const INFO* Get_Info() { return &m_tInfo; }
	const RECT* Get_Rect() { return &m_tRect; }
	const INFO* Get_SpriteInfo() { return &m_tSpriteInfo; }
	float Get_Angle() { return m_fAngle; }
	bool Get_Dead() { return m_bDead; }
	FRAME Get_Frame() { return m_tFrame; }

	void Set_Angle(float _fAngle) { m_fAngle = _fAngle; }
	void Set_Pos(float _fX, float _fY) { m_tInfo.fX = _fX; m_tInfo.fY = _fY; }
	void Set_PosX(float _fX) { m_tInfo.fX = _fX; }
	void Set_PosY(float _fY) { m_tInfo.fY = _fY; }
	void Set_Dead(bool _bFlag) { m_bDead = _bFlag; }
	void Set_FrameKey(const TCHAR* pFrameKey) { m_pFrameKey = pFrameKey; }
public:

	virtual void	Initialize()PURE;
	virtual int 	Update()PURE;
	virtual void	Late_Update()PURE;
	virtual void	Render(HDC hDC)PURE;
	virtual void	Release()PURE;

	virtual OBJTYPE Get_Type() const { return OBJ; };
	virtual void	HitActor(CObj* HitActor, HITPOINT _HitInfo) {}
	virtual void	HitActor(CMap* HitActor, HITPOINT _HitInfo) {}
	virtual void	HitActor(CObj* HitActor) {}

public:
	void		Update_Rect();
	void		Update_Sprite();
	void		Update_Frame(); // 원본은 Move Frame

protected:
	INFO		m_tInfo;
	RECT		m_tRect;
	INFO		m_tSpriteInfo;
	FRAME		m_tFrame;

	float		m_fSpeed;
	float		m_fVelocity;
	float		m_fAngle;

	bool		m_bDead;

	const TCHAR* m_pFrameKey;
};

