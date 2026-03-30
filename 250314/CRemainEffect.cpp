#include "pch.h"
#include "CRemainEffect.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"
#include "CEffectMgr.h"

CRemainEffect::CRemainEffect()
{
}

CRemainEffect::~CRemainEffect()
{
}

void CRemainEffect::Initialize()
{
	if ((m_pFrameKey == L"Player_Jump" || m_pFrameKey == L"Player_Run" || m_pFrameKey == L"Player_Attack" ||
		m_pFrameKey == L"Player_Roll" || m_pFrameKey == L"Player_WallJump" || m_pFrameKey == L"Player_Fall")
		&& GetTickCount64() - CEffectMgr::Get_Instance()->Get_TrailTimer() >= 20)
	{
		if (m_pFrameKey == L"Player_Jump")
		{
			m_tInfo.fCX = 60;
			m_tInfo.fCY = 84;
		}
		else if (m_pFrameKey == L"Player_WallJump")
		{
			m_tInfo.fCX = 96;
			m_tInfo.fCY = 88;
		}
		else if (m_pFrameKey == L"Player_Run")
		{
			m_tInfo.fCX = 88;
			m_tInfo.fCY = 64;
		}
		else if (m_pFrameKey == L"Player_Roll")
		{
			m_tInfo.fCX = 94;
			m_tInfo.fCY = 64;
		}
		else if (m_pFrameKey == L"Player_Attack")
		{
			m_tInfo.fCX = 120;
			m_tInfo.fCY = 82;
		}
		else if (m_pFrameKey == L"Player_Fall")
		{
			m_tInfo.fCX = 82;
			m_tInfo.fCY = 96;
		}
		m_tFrame.dwFrameSpeed = 10000.f;
		m_bTrail = true;
		CEffectMgr::Get_Instance()->Set_TrailTimer();
	}

	else if (m_pFrameKey == L"Player_Dust" && GetTickCount64() - CEffectMgr::Get_Instance()->Get_SlideTimer() >= 100)
	{
		m_tInfo.fCX = 34.f;
		m_tInfo.fCY = 28.f;
		m_tFrame.iStart = 0;
		m_tFrame.iEnd = 6;
		m_tFrame.iMotion = 0;
		m_tFrame.dwFrameSpeed = 75.f;
		CEffectMgr::Get_Instance()->Set_SlideTimer();
	}

	else if (m_pFrameKey == L"Player_Jumpdust")
	{
		m_tInfo.fCX = 42.f;
		m_tInfo.fCY = 80.f;
		m_tFrame.iStart = 0;
		m_tFrame.iEnd = 4;
		m_tFrame.iMotion = 0;
		m_tFrame.dwFrameSpeed = 75.f;
	}

	else if (m_pFrameKey == L"Player_Landdust")
	{
		m_tInfo.fCX = 80.f;
		m_tInfo.fCY = 16.f;
		m_tFrame.iStart = 0;
		m_tFrame.iEnd = 7;
		m_tFrame.iMotion = 0;
		m_tFrame.dwFrameSpeed = 75.f;
	}

	else if (m_pFrameKey == L"Bullet_Reflect")
	{
		m_tInfo.fCX = 140.f;
		m_tInfo.fCY = 128.f;
		m_tFrame.iStart = 0;
		m_tFrame.iEnd = 4;
		m_tFrame.iMotion = 0;
		m_tFrame.dwFrameSpeed = 75.f;
	}

	else if (m_pFrameKey == L"RIGHT_RUSH")
	{
		m_pFrameKey = L"Player_Rushdust";
		m_tInfo.fCX = 112.f;
		m_tInfo.fCY = 50.f;
		m_tFrame.iStart = 0;
		m_tFrame.iEnd = 9;
		m_tFrame.iMotion = 0;
		m_tFrame.dwFrameSpeed = 50.f;
	}

	else if (m_pFrameKey == L"LEFT_RUSH")
	{
		m_pFrameKey = L"Player_Rushdust";
		m_tInfo.fCX = 112.f;
		m_tInfo.fCY = 46.f;
		m_tFrame.iStart = 9;
		m_tFrame.iEnd = 9;
		m_tFrame.iMotion = 1;
		m_tFrame.dwFrameSpeed = 50.f;
	}
	else if (m_pFrameKey == L"Explosion")
	{
		m_pFrameKey = L"Explosion";
		m_tInfo.fCX = 96.f;
		m_tInfo.fCY = 112.f;
		m_tFrame.iStart = 0;
		m_tFrame.iEnd = 10;
		m_tFrame.iMotion = 0;
		m_tFrame.dwFrameSpeed = 50.f;
	}
	else if (m_pFrameKey == L"Bomb")
	{
		m_pFrameKey = L"Bomb";
		m_tInfo.fCX = 20.f;
		m_tInfo.fCY = 20.f;
		m_tFrame.iStart = 0;
		m_tFrame.iEnd = 0;
		m_tFrame.iMotion = 0;
		m_tFrame.dwFrameSpeed = 500000.f;
	}

	else if (m_pFrameKey == L"Dancing")
	{
		m_pFrameKey = L"Dancing";
		m_tInfo.fCX = 68.f;
		m_tInfo.fCY = 86.f;
		m_tFrame.iStart = 0;
		m_tFrame.iEnd = 11;
		m_tFrame.dwFrameSpeed = 75.f;
	}
	else if (m_pFrameKey == L"Left_Gun_Spark")
	{
		m_pFrameKey = L"Gun_Spark";
		m_tInfo.fCX = 98.f;
		m_tInfo.fCY = 78.f;
		m_tFrame.iStart = 3;
		m_tFrame.iMotion = 1;
		m_tFrame.iEnd = 3;
		m_tFrame.dwFrameSpeed = 50.f;
	}
	else if (m_pFrameKey == L"Right_Gun_Spark")
	{
		m_pFrameKey = L"Gun_Spark";
		m_tInfo.fCX = 98.f;
		m_tInfo.fCY = 78.f;
		m_tFrame.iStart = 0;
		m_tFrame.iMotion = 0;
		m_tFrame.iEnd = 3;
		m_tFrame.dwFrameSpeed = 50.f;
	}
	else if (m_pFrameKey == L"Zoro")
	{
		m_pFrameKey = L"Zoro";
		m_tInfo.fCX = 142.f;
		m_tInfo.fCY = 410.f;
		m_tFrame.iStart = 0;
		m_tFrame.iEnd = 0;
		m_tFrame.iMotion = 0;
		m_tFrame.dwFrameSpeed = 1000000.f;
		}
	else if (m_pFrameKey == L"Wind")
	{
		m_pFrameKey = L"Wind";
		m_tInfo.fCX = 75.f;
		m_tInfo.fCY = 120.f;
		m_tFrame.iStart = 0;
		m_tFrame.iEnd = 5;
		m_tFrame.iMotion = 0;
		m_tFrame.dwFrameSpeed = 150.f;
		}

	m_tFrame.dwTime = GetTickCount64();
}

int CRemainEffect::Update()
{
	return NOEVENT;
}

void CRemainEffect::Late_Update()
{
}

void CRemainEffect::Render(HDC hDC)
{
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	int iScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	if (m_bTrail == true)
	{
		const int width = (int)m_tInfo.fCX;
		const int height = (int)m_tInfo.fCY;

		BITMAPINFO bmi = {};
		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth = width;
		bmi.bmiHeader.biHeight = -height; // top-down
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biBitCount = 32;
		bmi.bmiHeader.biCompression = BI_RGB;

		void* pPixels = nullptr;
		HDC hMemDC = CreateCompatibleDC(hDC);
		HBITMAP hDibBmp = CreateDIBSection(hDC, &bmi, DIB_RGB_COLORS, &pPixels, nullptr, 0);
		HBITMAP hOldBmp = (HBITMAP)SelectObject(hMemDC, hDibBmp);

		BitBlt(
			hMemDC,
			0, 0,
			width, height,
			CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey),
			m_tFrame.iStart * width,
			m_tFrame.iMotion * height,
			SRCCOPY
		);

		// 픽셀 직접 수정
		bool bFlag = false;
		DWORD* pixels = (DWORD*)pPixels;
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				DWORD& px = pixels[y * width + x];
				BYTE r = GetRValue(px);
				BYTE g = GetGValue(px);
				BYTE b = GetBValue(px);

				if (r == 255 && g == 0 && b == 255)
				{
					bFlag = true;
					px = 0x00000000; // 완전 투명 (알파 0)
				}
				else
				{
					r = 21;
					g = 244;
					b = 238;
					BYTE a = 255; // 원하는 투명도 (0~255)
					px = (a << 24) | (r << 16) | (g << 8) | b;
				}
			}
		}
		if (!bFlag) return;
		BLENDFUNCTION blend = {};
		blend.BlendOp = AC_SRC_OVER;
		blend.SourceConstantAlpha = 160 - (GetTickCount64() - m_fCreatedTime); // 픽셀 자체 알파 사용
		blend.AlphaFormat = AC_SRC_ALPHA;

		AlphaBlend(
			hDC,
			(int)(m_tInfo.fX - m_tInfo.fCX / 2.f) + iScrollX,
			(int)(m_tInfo.fY - m_tInfo.fCY / 2.f) + iScrollY,
			width,
			height,
			hMemDC,
			0, 0,
			width,
			height,
			blend
		);

		// 정리
		SelectObject(hMemDC, hOldBmp);
		DeleteObject(hDibBmp);
		DeleteDC(hMemDC);
	}
	else if (m_pFrameKey == L"Player_Rushdust")
	{
		GdiTransparentBlt(
			hDC,
			m_tInfo.fX - (m_tInfo.fCX / 2.f) + iScrollX,
			m_tInfo.fY - 8.f + iScrollY,
			(int)m_tInfo.fCX,
			(int)m_tInfo.fCY,
			hMemDC,
			m_tFrame.iStart * (int)m_tInfo.fCX,
			m_tFrame.iMotion * (int)m_tInfo.fCY,
			(int)m_tInfo.fCX,
			(int)m_tInfo.fCY,
			RGB(255, 0, 255)
		);
	}
	else if(m_pFrameKey != L"Player_Landdust")
	{
		GdiTransparentBlt(
			hDC,
			m_tInfo.fX - (m_tInfo.fCX / 2.f) + iScrollX,
			m_tInfo.fY - (m_tInfo.fCY / 2.f) + iScrollY,
			(int)m_tInfo.fCX,
			(int)m_tInfo.fCY,
			hMemDC,
			m_tFrame.iStart * (int)m_tInfo.fCX,
			m_tFrame.iMotion * (int)m_tInfo.fCY,
			(int)m_tInfo.fCX,
			(int)m_tInfo.fCY,
			RGB(255, 0, 255)
		);
	}
	else if (m_pFrameKey == L"Dancing")
	{
		GdiTransparentBlt(
			hDC,
			m_tInfo.fX - (m_tInfo.fCX / 2.f),
			m_tInfo.fY - (m_tInfo.fCY / 2.f),
			(int)m_tInfo.fCX,
			(int)m_tInfo.fCY,
			hMemDC,
			m_tFrame.iStart * (int)m_tInfo.fCX,
			m_tFrame.iMotion * (int)m_tInfo.fCY,
			(int)m_tInfo.fCX,
			(int)m_tInfo.fCY,
			RGB(255, 0, 255)
		);
	}
	else
	{
		GdiTransparentBlt(
			hDC,
			m_tInfo.fX - (m_tInfo.fCX / 2.f) + iScrollX,
			m_tInfo.fY + (m_tInfo.fCY / 2.f) + iScrollY + 8.f,
			(int)m_tInfo.fCX,
			(int)m_tInfo.fCY,
			hMemDC,
			m_tFrame.iStart * (int)m_tInfo.fCX,
			m_tFrame.iMotion * (int)m_tInfo.fCY,
			(int)m_tInfo.fCX,
			(int)m_tInfo.fCY,
			RGB(255, 0, 255)
		);
	}

	if (isAnimationEnd() && !m_bTrail && !m_bInfinity && m_pFrameKey != L"Bomb" && m_pFrameKey != L"Zoro")
		m_bDead = true;

	else if (m_bTrail && GetTickCount64() - m_fCreatedTime >= 150)
		m_bDead = true;

	else if (m_pFrameKey == L"Bomb" && GetTickCount64() - m_fCreatedTime >= 2000)
		m_bDead = true;

	else if (m_pFrameKey == L"Zoro" && GetTickCount64() - m_fCreatedTime >= 3000)
		m_bDead = true;

	if(!m_bTrail && m_pFrameKey != L"Bomb")
		__super::Update_Frame();
}

void CRemainEffect::Release()
{
}
