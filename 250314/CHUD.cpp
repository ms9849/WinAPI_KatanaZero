#include "pch.h"
#include "CHUD.h"
#include "CObjMgr.h"
#include "CBmpMgr.h"
#include "CSceneMgr.h"

CHUD::CHUD()
{
}

CHUD::~CHUD()
{
	Release();
}

void CHUD::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Hud/hud_collapse.bmp", L"Hud_Collapse");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Hud/hud_Red_battery.bmp", L"Hud_Red_Battery");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Hud/hud_Blue_battery.bmp", L"Hud_Blue_Battery");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Hud/hud_Timer.bmp", L"Hud_Timer");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Hud/hud_butcher_knife.bmp", L"Hud_Knife");

	// 188 x 22 size
	/*
	여기에 남은 시간 이미지 추가해서 연동할 것.
	
	*/
	m_pPlayer = CObjMgr::Get_Instance()->Get_Player();
}

void CHUD::Update()
{
}

void CHUD::Late_Update()
{
}

void CHUD::Render(HDC hDC)
{
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Hud_Collapse");

	GdiTransparentBlt(hDC,/// 복사 받을 dc
		0,		// 복사 받을 위치 좌표 left
		0,					// 복사 받을 위치 좌표 top
		800,				// 복사 받을 가로 사이즈
		46,				// 복사 받을 세로 사이즈
		hMemDC,							// 복사할 이미지 dc
		0,
		0,// 복사할 이미지의 left, top
		800,				// 복사할 이미지의 가로
		46,				// 복사할 이미지의 세로
		RGB(255, 0, 255));

	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Hud_Timer");
	ULONGLONG fRemainTime = GetTickCount64() - dynamic_cast<CScene*>(CSceneMgr::Get_Instance()->Get_CurScene())->Get_StageStartTime();

	// 0~1200000 사이. 12000으로 나누면 0~100사이긴 한데..
	GdiTransparentBlt(hDC,/// 복사 받을 dc
		354,		// 복사 받을 위치 좌표 left
		6,					// 복사 받을 위치 좌표 top
		188 - 188 *(fRemainTime / 1200) / 100.f,				// 복사 받을 가로 사이즈 0
		22,				// 복사 받을 세로 사이즈
		hMemDC,							// 복사할 이미지 dc
		0,
		0,// 복사할 이미지의 left, top
		188,				// 복사할 이미지의 가로
		22,				// 복사할 이미지의 세로
		RGB(255, 0, 255));

	for (int i = 0; i < 9; ++i)
	{
		HDC hBlueDC = CBmpMgr::Get_Instance()->Find_Image(L"Hud_Blue_Battery");
		HDC hRedDC = CBmpMgr::Get_Instance()->Find_Image(L"Hud_Red_Battery");

		if (45 * i + 50 > dynamic_cast<CPlayer*>(m_pPlayer)->Get_SlowGauge())
		{
			GdiTransparentBlt(hDC,/// 복사 받을 dc
				26 + 12 * i,		// 복사 받을 위치 좌표 left
				13,					// 복사 받을 위치 좌표 top
				8,				// 복사 받을 가로 사이즈
				20,				// 복사 받을 세로 사이즈
				hRedDC,							// 복사할 이미지 dc
				0,
				0,// 복사할 이미지의 left, top
				8,				// 복사할 이미지의 가로
				20,				// 복사할 이미지의 세로
				RGB(255, 0, 255));
		}
		else
		{
			GdiTransparentBlt(hDC,/// 복사 받을 dc
				26 + 12 * i,		// 복사 받을 위치 좌표 left
				13,					// 복사 받을 위치 좌표 top
				8,				// 복사 받을 가로 사이즈
				20,				// 복사 받을 세로 사이즈
				hBlueDC,							// 복사할 이미지 dc
				0,
				0,// 복사할 이미지의 left, top
				8,				// 복사할 이미지의 가로
				20,				// 복사할 이미지의 세로
				RGB(255, 0, 255));
		}
	}

	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Hud_Knife");

	if (dynamic_cast<CPlayer*>(m_pPlayer)->Get_Hand() != nullptr)
	{
		GdiTransparentBlt(hDC,/// 복사 받을 dc
			730,		// 복사 받을 위치 좌표 left
			0,					// 복사 받을 위치 좌표 top
			42,				// 복사 받을 가로 사이즈
			42,				// 복사 받을 세로 사이즈
			hMemDC,							// 복사할 이미지 dc
			0,
			0,// 복사할 이미지의 left, top
			42,				// 복사할 이미지의 가로
			42,				// 복사할 이미지의 세로
			RGB(255, 0, 255));
	}
}

void CHUD::Release()
{
}
