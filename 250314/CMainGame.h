#pragma once

#include "Define.h"
#include "CHUD.h"
#include "CMouse.h"

class CMainGame
{
public:
	CMainGame();
	~CMainGame();

public:
	void		Initialize();
	void		Update();
	void		Late_Update();
	void		Render();
	void		Release();

public:
	void		Fade_In();
private:
	HDC		m_hDC;

	ULONGLONG		m_dwTime;
	int			m_iFPS;
	TCHAR		m_szFPS[32];

	bool m_bFadeIn = false;
	int m_iFadeFrame = 0;
	ULONGLONG m_fFadeTimer = 0.f;
};

// GetAsyncKeyState() 키 처리 함수