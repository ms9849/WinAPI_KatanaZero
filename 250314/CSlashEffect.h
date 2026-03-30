#pragma once
#include "CEffect.h"

class CSlashEffect : public CEffect
{
public:
	CSlashEffect();
	CSlashEffect(float _fX, float _fY, const TCHAR* _pFrameKey) : CEffect(_fX, _fY, _pFrameKey), m_fCreatedTime(GetTickCount64()) {}
	CSlashEffect(float _fX, float _fY, const TCHAR* _pFrameKey, float _fAngle) : CEffect(_fX, _fY, _pFrameKey), m_fAngle(_fAngle), m_fCreatedTime(GetTickCount64()) {}
	virtual ~CSlashEffect();

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

private:
	float m_fAngle;
	float m_fSpeed;
	ULONGLONG m_fCreatedTime;
	Image* m_pImage;
};

