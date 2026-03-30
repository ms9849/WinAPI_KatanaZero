#include "pch.h"
#include "CFanBlade.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"

CFanBlade::CFanBlade()
{
}

CFanBlade::~CFanBlade()
{
}

void CFanBlade::Initialize()
{
	m_tInfo.fCX = 72.f;
	m_tInfo.fCY = 190.f;

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/fanblade/fanblade_32x1.bmp", L"Fan_Blade");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/fanblade/fanblade_bg.bmp", L"Fan_Blade_Bg");

	m_pFrameKey = L"Fan_Blade";
	m_tFrame.iStart = 0;
	m_tFrame.iEnd = 31;
	m_tFrame.iMotion = 0;
	m_tFrame.dwTime = GetTickCount64();
	m_tFrame.dwFrameSpeed = 25.f; //추후 time 변수 추가
}

int CFanBlade::Update()
{
	__super::Update_Rect();
	return NOEVENT;
}

void CFanBlade::Late_Update()
{
	__super::Update_Frame();
}

void CFanBlade::Render(HDC hDC)
{
	int iScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();
	//백그라운드 출력
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Fan_Blade_Bg");
	GdiTransparentBlt(hDC,/// 복사 받을 dc
		m_tInfo.fX - m_tInfo.fCX / 2.f - 35 + iScrollX,		// 복사 받을 위치 좌표 left
		m_tInfo.fY - m_tInfo.fCY / 2.f - 30 + iScrollY,		// 복사 받을 위치 좌표 top
		(int)82,				// 복사 받을 가로 사이즈
		(int)224,				// 복사 받을 세로 사이즈
		hMemDC,					// 복사할 이미지 dc
		0,0,					// 복사할 이미지의 left, top
		(int)82,				// 복사할 이미지의 가로
		(int)224,				// 복사할 이미지의 세로
		RGB(255, 0, 255));

	//본체 출력, 알파블렌딩 적용하던가 해야함.
	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
	if (!g_bSlowMod || (m_tFrame.iStart >= 12 && m_tFrame.iStart <= 24))
	{
		GdiTransparentBlt(hDC,/// 복사 받을 dc
			m_tInfo.fX - m_tInfo.fCX / 2.f + iScrollX,		// 복사 받을 위치 좌표 left
			m_tInfo.fY - m_tInfo.fCY / 2.f + iScrollY,		// 복사 받을 위치 좌표 top
			(int)m_tInfo.fCX,				// 복사 받을 가로 사이즈
			(int)m_tInfo.fCY,				// 복사 받을 세로 사이즈
			hMemDC,							// 복사할 이미지 dc
			m_tFrame.iStart * (int)m_tInfo.fCX,
			m_tFrame.iMotion * (int)m_tInfo.fCY,							// 복사할 이미지의 left, top
			(int)m_tInfo.fCX,				// 복사할 이미지의 가로
			(int)m_tInfo.fCY,				// 복사할 이미지의 세로
			RGB(255, 0, 255));
	}
	else if(g_bSlowMod && (m_tFrame.iStart < 12 || m_tFrame.iStart > 24))
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

		DWORD* pixels = (DWORD*)pPixels;
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				DWORD& px = pixels[y * width + x];
				BYTE r = GetRValue(px);
				BYTE g = GetGValue(px);
				BYTE b = GetBValue(px);

				if (!(r == 255 && g == 0 && b == 255))
				{
					// 반투명한 붉은색 (예: 50% 붉은색 섞기)
					float alpha = 0.6f;
					BYTE blendR = (BYTE)(r * (1.0f - alpha) + 255 * alpha); // 255는 붉은색
					BYTE blendG = (BYTE)(g * (1.0f - alpha) + 0 * alpha);
					BYTE blendB = (BYTE)(b * (1.0f - alpha) + 0 * alpha);

					px = (blendR << 16) | (blendG << 8) | blendB;
				}
			}
		}
		GdiTransparentBlt(hDC,/// 복사 받을 dc
			m_tInfo.fX - m_tInfo.fCX / 2.f + iScrollX,		// 복사 받을 위치 좌표 left
			m_tInfo.fY - m_tInfo.fCY / 2.f + iScrollY,					// 복사 받을 위치 좌표 top
			(int)m_tInfo.fCX,				// 복사 받을 가로 사이즈
			(int)m_tInfo.fCY,				// 복사 받을 세로 사이즈
			hMemDC,							// 복사할 이미지 dc
			0,
			0,							// 복사할 이미지의 left, top
			(int)m_tInfo.fCX,				// 복사할 이미지의 가로
			(int)m_tInfo.fCY,				// 복사할 이미지의 세로
			RGB(255, 0, 255));
		// 정리
		SelectObject(hMemDC, hOldBmp);
		DeleteObject(hDibBmp);
		DeleteDC(hMemDC);
	}

	/*
		Rectangle(hDC,
		m_tInfo.fX - m_tInfo.fCX / 2.f + iScrollX, m_tInfo.fY - m_tInfo.fCY / 2.f + iScrollY,
		m_tInfo.fX + m_tInfo.fCX / 2.f + iScrollX, m_tInfo.fY + m_tInfo.fCY / 2.f + iScrollY);
	*/
}

void CFanBlade::Release()
{
}
