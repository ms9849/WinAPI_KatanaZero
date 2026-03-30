#include "pch.h"
#include "CFanblade_Fg.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"

CFanblade_Fg::CFanblade_Fg()
{
}

CFanblade_Fg::~CFanblade_Fg()
{
}

void CFanblade_Fg::Initialize()
{
    m_tInfo.fCX = 90.f;
    m_tInfo.fCY = 224.f;
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/fanblade/fanblade_fg.bmp", L"Fan_Blade_Fg");
}

int CFanblade_Fg::Update()
{
    return NOEVENT;
}

void CFanblade_Fg::Late_Update()
{
}

void CFanblade_Fg::Render(HDC hDC)
{
	int iScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Fan_Blade_Fg");
	GdiTransparentBlt(hDC,/// 복사 받을 dc
		m_tInfo.fX - m_tInfo.fCX / 2.f + iScrollX,		// 복사 받을 위치 좌표 left
		m_tInfo.fY - m_tInfo.fCY / 2.f + iScrollY,		// 복사 받을 위치 좌표 top
		(int)90,				// 복사 받을 가로 사이즈
		(int)224,				// 복사 받을 세로 사이즈
		hMemDC,					// 복사할 이미지 dc
		0, 0,					// 복사할 이미지의 left, top
		(int)90,				// 복사할 이미지의 가로
		(int)224,				// 복사할 이미지의 세로
		RGB(255, 0, 255));
}

void CFanblade_Fg::Release()
{
}
