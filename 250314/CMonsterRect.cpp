#include "pch.h"
#include "CMonsterRect.h"
#include "CScrollMgr.h"

CMonsterRect::~CMonsterRect()
{
}

void CMonsterRect::Initialize()
{
}

int CMonsterRect::Update()
{
	return 0;
}

void CMonsterRect::Late_Update()
{
}

void CMonsterRect::Render(HDC hDC)
{
	/*
		HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
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
	DeleteObject(hPen);6
	*/
}

void CMonsterRect::Release()
{
}
