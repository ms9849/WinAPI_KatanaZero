#pragma once
#include "CObj.h"
#include "CMap.h"

class CHead : public CObj
{
public:
	CHead();
	~CHead();

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

	virtual OBJTYPE Get_Type() const { return HEAD; };
	virtual void	HitActor(CObj* HitActor, HITPOINT _HitInfo);
	virtual void	HitActor(CMap* HitActor, HITPOINT _HitInfo);
	virtual void	HitActor(CObj* HitActor);

private:
	float m_fTime;
	float m_fHeadBanging;
	bool m_bJump;
	Image* m_pImage;
};

