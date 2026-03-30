#include "pch.h"
#include "CSceneMgr.h"
#include "CUIMgr.h"
#include "CSoundMgr.h"

CSceneMgr* CSceneMgr::m_pInstance = nullptr;

CSceneMgr::CSceneMgr() : m_eCurScene(SC_MENU), m_ePreScene(SC_END), m_pScene(nullptr), m_iFilterCounter(0), m_fFilterTimer(GetTickCount64())
{
}

CSceneMgr::~CSceneMgr()
{
	Release();
}

void CSceneMgr::Scene_Change(SCENEID _eId)
{
	m_eCurScene = _eId;

	if (m_eCurScene != m_ePreScene)
	{
		Safe_Delete(m_pScene);
		CUIMgr::Destroy_Instance();

		switch (m_eCurScene)
		{
		case SC_MENU:
			m_pScene = new CMenu;
			break;

		case SC_STAGE:
			CSoundMgr::Get_Instance()->StopAll();
			CSoundMgr::Get_Instance()->PlaySound(L"stage_start.wav", SOUND_EFFECT, g_fVolume);
			m_pScene = new CStage;
			break;

		case SC_STAGE_2:
			//임시로 뺐음. 같은 음악을 공유하니까..
			//CSoundMgr::Get_Instance()->StopAll();
			CSoundMgr::Get_Instance()->PlaySound(L"stage_start.wav", SOUND_EFFECT, g_fVolume);
			m_pScene = new CStage2;
			break;
		
		case SC_STAGE_3:
			CSoundMgr::Get_Instance()->StopAll();
			CSoundMgr::Get_Instance()->PlaySound(L"stage_start.wav", SOUND_EFFECT, g_fVolume);
			m_pScene = new CStage3;
			break;

		case SC_STAGE_4:
			CSoundMgr::Get_Instance()->StopAll();
			CSoundMgr::Get_Instance()->PlaySound(L"stage_start.wav", SOUND_EFFECT, g_fVolume);
			m_pScene = new CStage4;
			break;

		case SC_ENDING:
			CSoundMgr::Get_Instance()->StopAll();
			CSoundMgr::Get_Instance()->PlaySound(L"stage_ending.ogg", SOUND_EFFECT, g_fVolume);
			m_pScene = new CEndScene;
			break;

		case SC_END:
			break;
			// 추후 엔드씬 추가
		}

		m_pScene->Initialize();
		m_ePreScene = m_eCurScene;
	}
}

void CSceneMgr::Update()
{
	m_pScene->Update();
}

void CSceneMgr::Late_Update()
{
	m_pScene->Late_Update();
}

void CSceneMgr::Render(HDC hDC)
{
	m_pScene->Render(hDC);
	CUIMgr::Get_Instance()->Render(hDC);
	Replay_Filter(hDC);
	End_Filter(hDC);
}

void CSceneMgr::Release()
{
	Safe_Delete(m_pScene);
}

void CSceneMgr::Replay_Filter(HDC hDC)
{
	if (g_bReplaying)
	{
		HDC hMemDC = CreateCompatibleDC(hDC);
		RECT rect;
		GetClientRect(g_hWnd, &rect);

		HBITMAP hBitmap = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);
		SelectObject(hMemDC, hBitmap);

		// 반투명 빨강으로 채움
		HBRUSH hBrush = CreateSolidBrush(RGB(21,244,238));
		FillRect(hMemDC, &rect, hBrush);

		// Alpha blending
		BLENDFUNCTION bf = { AC_SRC_OVER, 0, 32, 0 }; // 128 = 50% 투명도
		AlphaBlend(hDC, 0, 0, rect.right, rect.bottom, hMemDC, 0, 0, rect.right, rect.bottom, bf);

		// 정리
		DeleteObject(hBrush);
		DeleteObject(hBitmap);
		DeleteDC(hMemDC);
		ReleaseDC(g_hWnd, hDC);
	}
}

void CSceneMgr::End_Filter(HDC hDC)
{
	if (g_bGameClear)
	{
		HDC hMemDC = CreateCompatibleDC(hDC);
		RECT rect;
		GetClientRect(g_hWnd, &rect);

		HBITMAP hBitmap = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);
		SelectObject(hMemDC, hBitmap);

		HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
		FillRect(hMemDC, &rect, hBrush);

		BLENDFUNCTION bf = { AC_SRC_OVER, 0, m_iFilterCounter, 0 }; // 128 = 50% 투명도
		AlphaBlend(hDC, 0, 0, rect.right, rect.bottom, hMemDC, 0, 0, rect.right, rect.bottom, bf);

		// 정리
		DeleteObject(hBrush);
		DeleteObject(hBitmap);
		DeleteDC(hMemDC);
		ReleaseDC(g_hWnd, hDC);

		if (m_iFilterCounter <= 255 && (GetTickCount64() - m_fFilterTimer >= 100))
		{
			m_iFilterCounter += 3;
			m_fFilterTimer = GetTickCount64();
		}

		if (m_iFilterCounter > 255)
		{
			g_bStageClear = true;
		}
	}
}