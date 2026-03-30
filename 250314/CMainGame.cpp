#include "pch.h"
#include "CMainGame.h"
#include "CSceneMgr.h"
#include "CKeyMgr.h"
#include "CBmpMgr.h"
#include "CObjMgr.h"
#include "CLineMgr.h"
#include "CRectMgr.h"
#include "CAbstractFactory.h"
#include "CScrollMgr.h"
#include "CUIMgr.h"
#include "CReplayMgr.h"
#include "CSoundMgr.h"
#include "CEffectMgr.h"

CMainGame::CMainGame()
{
}

CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Initialize()
{
	m_hDC = GetDC(g_hWnd);
	//백 버퍼 미리 넣어주고
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Back.bmp", L"Back");

	//페이드인, 아웃 연출용, 노이즈도
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/fade_in.bmp", L"Fade_In");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/fade_out.bmp", L"Fade_Out");
	//히트 박스들은 자주 없어졌다 생길거니까.. 이미지 미리 넣어줌
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/Grunt/enemy_grunt_slash_5x2.bmp", L"Grunt_Hit");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/player_slash_basic_8x2.bmp", L"Player_Hit");

	//UI 관련 이미지.
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Ui/RestartUI.bmp", L"Restart_UI");

	//회전용 plg와 reset
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Plg.bmp", L"Plg");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Reset.bmp", L"Reset");

	//사운드 매니저
	CSoundMgr::Get_Instance()->Initialize();
	CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SC_MENU);
}

void CMainGame::Update()
{
	CSceneMgr::Get_Instance()->Update();
}

void CMainGame::Late_Update()
{
	CScrollMgr::Get_Instance()->Late_Update();
	CLineMgr::Get_Instance()->Late_Update();
	CSceneMgr::Get_Instance()->Late_Update();
	CKeyMgr::Get_Instance()->Update();

	//나중에 조건과 값만 바꿔주면 된다. 묶어서 함수화 해도 되고.. switch나 case문으로 바꿔볼 것.

	//메인에서 1스테이지
	if (g_bStageClear && g_iCurStage == 0)
	{
		g_iStageReplayCount = 0;
		CSoundMgr::Get_Instance()->PlaySound(L"stage_clear.wav", SOUND_EFFECT, g_fVolume);
		Fade_In();
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SC_STAGE);
		g_iCurStage++;
		g_bStageClear = false;
	}

	if (g_bStageClear && g_iCurStage == 1)
	{
		g_iStageReplayCount = 0;
		CSoundMgr::Get_Instance()->PlaySound(L"stage_clear.wav", SOUND_EFFECT, g_fVolume);
		Fade_In();
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SC_STAGE_2);
		g_iCurStage++;
		g_bStageClear = false;
	}

	if (g_bStageClear && g_iCurStage == 2)
	{
		g_iStageReplayCount = 0;
		CSoundMgr::Get_Instance()->PlaySound(L"stage_clear.wav", SOUND_EFFECT, g_fVolume);
		Fade_In();
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SC_STAGE_3);
		g_iCurStage++;
		g_bStageClear = false;
	}
	//1스테이지에서 4스테이지 ( 임시로 둔거임 )
	if (g_bStageClear && g_iCurStage == 3)
	{
		g_iStageReplayCount = 0;
		CSoundMgr::Get_Instance()->PlaySound(L"stage_clear.wav", SOUND_EFFECT, g_fVolume);
		Fade_In();
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SC_STAGE_4);
		g_iCurStage++;
		g_bStageClear = false;
	}
	if (g_bStageClear && g_iCurStage == 4)
	{
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SC_ENDING);
		g_iCurStage++;
		g_bStageClear = false;
	}
}

void CMainGame::Render()
{
	m_iFPS++;

	if (m_dwTime + 1000 < GetTickCount64())
	{
		swprintf_s(m_szFPS, L"FPS : %d", m_iFPS);

		SetWindowText(g_hWnd, m_szFPS);

		m_iFPS = 0;
		m_dwTime = GetTickCount64();
	}

	HDC		hBackDC = CBmpMgr::Get_Instance()->Find_Image(L"Back");

	CSceneMgr::Get_Instance()->Render(hBackDC);

	BitBlt(m_hDC,
		0,
		0,
		WINCX, WINCY,
		hBackDC,
		0, 0,
		SRCCOPY);
}

void CMainGame::Release()
{
	CObjMgr::Destroy_Instance();
	CBmpMgr::Destroy_Instance();
	CKeyMgr::Destroy_Instance();
	CLineMgr::Destroy_Instance();
	CSoundMgr::Destroy_Instance();
	CRectMgr::Destroy_Instance();
	CReplayMgr::Destroy_Instance();
	CEffectMgr::Destroy_Instance();
	CSceneMgr::Destroy_Instance();
	CUIMgr::Destroy_Instance();
	CScrollMgr::Destroy_Instance();
	ReleaseDC(g_hWnd, m_hDC);
}

void CMainGame::Fade_In()
{
	HDC		hBackDC = CBmpMgr::Get_Instance()->Find_Image(L"Back");
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Fade_In");
	for (int i = 0; i < 32; ++i)
	{
		Sleep(20);
		GdiTransparentBlt(hBackDC,/// 복사 받을 dc
			-800 + i * 32,		// 복사 받을 위치 좌표 left
			0,					// 복사 받을 위치 좌표 top
			896,				// 복사 받을 가로 사이즈
			600,				// 복사 받을 세로 사이즈
			hMemDC,							// 복사할 이미지 dc
			0,0,
			896,576,			// 복사할 이미지의 세로
			RGB(255, 255, 255));

		BitBlt(m_hDC,
			0,
			0,
			WINCX, WINCY,
			hBackDC,
			0, 0,
			SRCCOPY);
	}
}
