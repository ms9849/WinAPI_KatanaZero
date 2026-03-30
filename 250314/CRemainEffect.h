#pragma once
#include "CEffect.h"
class CRemainEffect : public CEffect
{
public:
	CRemainEffect();
	CRemainEffect(float _fX, float _fY, const TCHAR* _pFrameKey) : CEffect(_fX, _fY, _pFrameKey), m_fCreatedTime(GetTickCount64()) {}
	CRemainEffect(float _fX, float _fY, const TCHAR* _pFrameKey, FRAME _fFrame) : CEffect(_fX, _fY, _pFrameKey, _fFrame), m_fCreatedTime(GetTickCount64()) {}
	virtual ~CRemainEffect();

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

private:
	ULONGLONG m_fCreatedTime;
};

