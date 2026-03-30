#pragma once
#include "CEffect.h"

class CFollowEffect : public CEffect
{
public:
	CFollowEffect();
	CFollowEffect(float _fX, float _fY, const TCHAR* _pFrameKey, CObj* _pTarget) : CEffect(_fX, _fY, _pFrameKey), m_pTarget(_pTarget), m_fEffectTime(GetTickCount64()) {}
	virtual ~CFollowEffect();

public:
	void Set_Target(CObj* _pTarget) { m_pTarget = _pTarget; }

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

private:
	CObj* m_pTarget;
	ULONGLONG m_fEffectTime;
};

