#include "pch.h"
#include "CRect.h"
#include "CScrollMgr.h"

CRect::CRect()
{
}

CRect::~CRect()
{
}

void CRect::Initialize()
{
}

int CRect::Update()
{
	return NOEVENT;
}

void CRect::Late_Update()
{
}

void CRect::Render(HDC hDC)
{
	/*
		HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);

	// 이전 펜/브러시 저장
	HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);

	int		iScrollX = (INT)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (INT)CScrollMgr::Get_Instance()->Get_ScrollY();

	Rectangle(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, m_tRect.right + iScrollX, m_tRect.bottom + iScrollY);

	SelectObject(hDC, hOldPen);
	SelectObject(hDC, hOldBrush);

	// 생성한 펜 제거
	DeleteObject(hPen);
	*/
}

void CRect::Release()
{
}
