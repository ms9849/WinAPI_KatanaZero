#pragma once
#include "CEffect.h"
class CStainEffect : public CEffect
{
public:
	CStainEffect();
	CStainEffect(float _fX, float _fY, const TCHAR* _pFrameKey) : CEffect(_fX, _fY, _pFrameKey), m_fCreatedTime(GetTickCount64()) {}
	CStainEffect(float _fX, float _fY, const TCHAR* _pFrameKey, float _fAngle) : CEffect(_fX, _fY, _pFrameKey), m_fAngle(_fAngle), m_fCreatedTime(GetTickCount64()) {}
	virtual ~CStainEffect();

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

private:
	ULONGLONG m_fCreatedTime;
	float m_fAngle;
	Image* m_pImage;
};

