#pragma once
#include "CObj.h"
#include "CSoundMgr.h"
#include "CMap.h"
#include "CItem.h"

class CBullet : public CObj
{
public:
	CBullet();
	CBullet(float _fX, float _fY, float _fAngle) : CObj(_fX, _fY, _fAngle) {}
	virtual ~CBullet();

public:
	void Reverse() 
	{ 
		if (!m_bParried)
		{
			m_fAngle = m_fAngle - 180;
			CSoundMgr::Get_Instance()->PlaySound(L"parring.wav", SOUND_EFFECT, g_fVolume);
		}
	}
	OBJTYPE Get_Type() const override { return OBJTYPE::BULLET; }
	bool Get_Parried() { return m_bParried; }
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

public:
	void	HitActor(CObj* HitActor, HITPOINT _HitInfo);
	void	HitActor(CMap* HitActor, HITPOINT _HitInfo);
	void	HitActor(CObj* HitActor);

private:
	bool m_bParried;
};

