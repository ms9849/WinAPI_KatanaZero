#include "pch.h"
#include "CRestartUI.h"
#include "CBmpMgr.h"
CRestartUI::CRestartUI()
{
}

CRestartUI::~CRestartUI()
{
	Release();
}

void CRestartUI::Initialize()
{
}

void CRestartUI::Update()
{
}

void CRestartUI::Late_Update()
{
}

void CRestartUI::Render(HDC hDC)
{
	HDC hMemDC = CreateCompatibleDC(hDC);
	RECT rect{ WINCX / 2 - 125, WINCY / 2 - 100, WINCX / 2 + 125, WINCY / 2 + 100 };
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	HBITMAP hBitmap = CreateCompatibleBitmap(hDC, width, height);
	SelectObject(hMemDC, hBitmap);

	// 반투명 검정으로 채움
	HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
	RECT localRect{ 0, 0, width, height }; // DC 기준 좌표
	FillRect(hMemDC, &localRect, hBrush);

	// Alpha blending
	BLENDFUNCTION bf = { AC_SRC_OVER, 0, 128, 0 }; // 50% 투명
	AlphaBlend(hDC, rect.left, rect.top, width, height,
		hMemDC, 0, 0, width, height, bf);

	// 정리
	DeleteObject(hBrush);
	DeleteObject(hBitmap);
	DeleteDC(hMemDC);
	ReleaseDC(g_hWnd, hDC);


	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Restart_UI");

	GdiTransparentBlt(hDC,/// 복사 받을 dc
		WINCX/2 - 125,		// 복사 받을 위치 좌표 left
		WINCY/2 - 100,					// 복사 받을 위치 좌표 top
		250,				// 복사 받을 가로 사이즈
		200,				// 복사 받을 세로 사이즈
		hMemDC,							// 복사할 이미지 dc
		0,
		0,// 복사할 이미지의 left, top
		250,				// 복사할 이미지의 가로
		200,				// 복사할 이미지의 세로
		RGB(45, 41, 41));
}

void CRestartUI::Release()
{
}
