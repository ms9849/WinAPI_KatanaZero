#include "pch.h"
#include "CMouse.h"
#include "CBmpMgr.h"

CMouse::CMouse()
{
}

CMouse::~CMouse()
{
}

void CMouse::Initialize()
{
	m_tInfo = { WINCX / 2.f, WINCY / 2.f, 50.f, 50.f }; // 770 나누기 11은 70.......

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cursor.bmp", L"Cursor");
	ShowCursor(false);
}

int CMouse::Update()
{
	POINT pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	m_tInfo.fX = pt.x;
	m_tInfo.fY = pt.y;
	__super::Update_Rect();
	return 0;
}

void CMouse::Late_Update()
{
}

void CMouse::Render(HDC hDC)
{
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Cursor");

	GdiTransparentBlt(hDC,/// 복사 받을 dc
		m_tRect.left,		// 복사 받을 위치 좌표 left
		m_tRect.top,					// 복사 받을 위치 좌표 top
		(int)m_tInfo.fCX,				// 복사 받을 가로 사이즈
		(int)m_tInfo.fCY,				// 복사 받을 세로 사이즈
		hMemDC,							// 복사할 이미지 dc
		0,
		0,// 복사할 이미지의 left, top
		(int)m_tInfo.fCX,				// 복사할 이미지의 가로
		(int)m_tInfo.fCY,				// 복사할 이미지의 세로
		RGB(255, 0, 255));
}

void CMouse::Release()
{
}
