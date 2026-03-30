#include "pch.h"
#include "CEffectMgr.h"
#include "CBmpMgr.h"

CEffectMgr* CEffectMgr::m_pInstance = nullptr;

CEffectMgr::CEffectMgr()
{
}

CEffectMgr::~CEffectMgr()
{
	Release();
}

void CEffectMgr::Start_Timer()
{
	m_tSlideEffectTimer = GetTickCount64();
	m_tTrailEffectTimer = GetTickCount64();
}

void CEffectMgr::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/zoro.bmp", L"Zoro");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/wind.bmp", L"Wind");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/player_dance_12x1.bmp", L"Dancing");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/player_jumpdust_5x1.bmp", L"Player_Jumpdust");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/player_dust_7x1.bmp", L"Player_Dust");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/player_landdust_8x1.bmp", L"Player_Landdust");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/player_enemy_slash.bmp", L"Player_Enemy_Slash");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/player_rush_dust_10x2.bmp", L"Player_Rushdust");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/player_bulletreflect_5x1.bmp", L"Bullet_Reflect");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/effect_blood_10x2.bmp", L"Effect_Blood");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Hud/hud_butcher_knife.bmp", L"Butcher_Knife_Effect");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Explosion_11x2.bmp", L"Explosion");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Bomb_1x1.bmp", L"Bomb");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/effect_gunspark_big_4x2.bmp", L"Gun_Spark");
}

void CEffectMgr::Update()
{
}

void CEffectMgr::Late_Update()
{
}

void CEffectMgr::Render(HDC hDC)
{
	for (auto& Src : m_EffectList)
	{
		if (Src->Get_Dead() == true)
		{
			continue;
		}

		Src->Render(hDC);
	}
}

void CEffectMgr::Render_Front(HDC hDC)
{
	for (auto& Src : m_FrontEffectList)
	{
		if (Src->Get_Dead() == true) 
		{
			continue;
		}

		Src->Render(hDC);
	}
}

void CEffectMgr::Release()
{
	for (auto& Src : m_EffectList)
	{
		Safe_Delete(Src);
	}

	for (auto& Src : m_FrontEffectList)
	{
		Safe_Delete(Src);
	}
}
