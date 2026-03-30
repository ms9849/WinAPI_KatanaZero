#include "pch.h"
#include "CLine.h"
#include "CScrollMgr.h"

CLine::CLine()
{
}

CLine::CLine(LINEPOINT _tLeft, LINEPOINT _tRight)
{
	if (_tLeft.fX > _tRight. fX) 
	{
		LINEPOINT lptmp = _tRight;
		_tRight = _tLeft;
		_tLeft = lptmp;
	}

	m_tLeft = _tLeft;
	m_tRight = _tRight;
}

CLine::CLine(LINEPOINT _tLeft, LINEPOINT _tRight, MAP_OBJ_TYPE _type)
{
	if (_tLeft.fX > _tRight.fX)
	{
		LINEPOINT lptmp = _tRight;
		_tRight = _tLeft;
		_tLeft = lptmp;
	}

	m_tLeft = _tLeft;
	m_tRight = _tRight;
	m_eType = _type;
}

CLine::~CLine()
{
	Release();
}

void CLine::Initialize()
{
}

int CLine::Update()
{
	return NOEVENT;
}

void CLine::Late_Update()
{
}

void CLine::Render(HDC hDC)
{
	/*
		int		iScrollX = (INT)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (INT)CScrollMgr::Get_Instance()->Get_ScrollY();

	MoveToEx(hDC, m_tLeft.fX + iScrollX, m_tLeft.fY + iScrollY, NULL);
	LineTo(hDC, m_tRight.fX + iScrollX, m_tRight.fY + iScrollY);
	*/
}

void CLine::Release()
{
}
