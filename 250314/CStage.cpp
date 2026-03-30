#include "pch.h"
#include "CStage.h"
#include "CBmpMgr.h"
#include "CObjMgr.h"
#include "CAbstractFactory.h"
#include "CScrollMgr.h"
#include "CLineMgr.h"
#include "CRectMgr.h"
#include "CCollisionMgr.h"
#include "CUIMgr.h"
#include "CKeyMgr.h"
#include "CReplayMgr.h"
#include "CSoundMgr.h"
#include "CEffectMgr.h"
#include "CItem.h"

CStage::CStage() : m_bCleared(false), m_bPortalCreated(false), m_iCount(0)
{
}

CStage::~CStage()
{
	Release();
}

void CStage::Initialize()
{
    m_bPortalCreated = false;
    m_iFadeCount = 0;
    m_fFadeOutTimer = GetTickCount64();
    g_iStageReplayCount++;
    m_iCount = 0;
    m_fStageStartTime = GetTickCount64();
    //bgm 추가
    CSoundMgr::Get_Instance()->PlayBGM(L"stage_bg.ogg", g_fVolume);
    //bg 추가
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/stage1.bmp", L"stage1");

    //이펙트 매니저 이니셜라이즈
    CEffectMgr::Get_Instance()->Initialize();

    //지형지물 추가
    CLineMgr::Get_Instance()->Add_Line(new CLine({ 0,1184 }, { 559,1184 }, TYPE_GROUND));
    CLineMgr::Get_Instance()->Add_Line(new CLine({ 535, 768 }, { 1039, 768 }, TYPE_GROUND));
    CLineMgr::Get_Instance()->Add_Line(new CLine({ 1005, 480 }, { 1949, 480 }, TYPE_GROUND));

    CRectMgr::Get_Instance()->Add_Rect(new CRect({ 816,198, 1949,255 }, TYPE_WALL));
    CRectMgr::Get_Instance()->Add_Rect(new CRect({ 1041,488, 1057,768 }, TYPE_WALL)); //맨위 Rect
    CRectMgr::Get_Instance()->Add_Rect(new CRect({ 816,198, 847,607 }, TYPE_WALL));
    CRectMgr::Get_Instance()->Add_Rect(new CRect({ 0,975, 400,991 }, TYPE_WALL));  // 맨아래 rect
    CRectMgr::Get_Instance()->Add_Rect(new CRect({ 384,526, 847,542 }, TYPE_WALL));
    CRectMgr::Get_Instance()->Add_Rect(new CRect({ 384,526, 400,985 }, TYPE_WALL));  // 991이였으
    CRectMgr::Get_Instance()->Add_Rect(new CRect({ 559,780, 575,1184 }, TYPE_WALL));
    CRectMgr::Get_Instance()->Add_Rect(new CMonsterRect({ 1560,300, 1747, 445 }, TYPE_MONSTER_WALL));
    //obj 추가
    //플레이어, 마우스 추가
    CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CPlayer>::Create_Obj(217.f, 1152.f));
    CObjMgr::Get_Instance()->Add_Object(OBJ_MOUSE, CAbstractFactory<CMouse>::Create_Obj());
    //
    //환풍구
    CObjMgr::Get_Instance()->Add_Object(OBJ_FANBLADE, CAbstractFactory<CFanBlade>::Create_Obj(1620.f, 380.f));
    CObjMgr::Get_Instance()->Add_Object(OBJ_FANBLADE_FG, CAbstractFactory<CFanblade_Fg>::Create_Obj(1662.f, 368));

    //Monster 추가
    //CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CGrunt>::Create_Obj(250, 1150));
    CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CBold>::Create_Obj(836, 734));
    CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CGrunt>::Create_Obj(1443, 392));

    //UI 추가
    CUI* pHUD = new CHUD();
    pHUD->Initialize();
    CUIMgr::Get_Instance()->Add_UI(pHUD);

    //scroll 최대최소 설정, 시작 좌표
    CScrollMgr::Get_Instance()->Set_ScrollPos(0, -800.f);
    CScrollMgr::Get_Instance()->Set_ScrollMinX(0.f);
    CScrollMgr::Get_Instance()->Set_ScrollMaxX(1950.f);

    CScrollMgr::Get_Instance()->Set_ScrollMinY(0.f);
    CScrollMgr::Get_Instance()->Set_ScrollMaxY(1400.f);

    //리플레이 매니저 추적 시작.
    CReplayMgr::Get_Instance()->Set_ObjList(CObjMgr::Get_Instance()->Get_Playerlist_Ref(), OBJ_PLAYER);
    CReplayMgr::Get_Instance()->Set_ObjList(CObjMgr::Get_Instance()->Get_Monsterlist_Ref(), OBJ_MONSTER);
}

void CStage::Update()
{
    CObjMgr::Get_Instance()->Update();
}

void CStage::Late_Update()
{
    //리플레이 관련 로직.. 함수화 굳이 안햇음.
    if (g_bReplaying)
    {
        if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON) || CReplayMgr::Get_Instance()->Get_Id() < 0)
        {
            Restart();
            g_bReplaying = false;
        }

        if (m_iCount % 2 == 0)
        {
            CReplayMgr::Get_Instance()->Rollback();
            CReplayMgr::Get_Instance()->Sub_Id();
        }
        m_iCount--;
        return;
    }

    CObjMgr::Get_Instance()->Late_Update();
    CScrollMgr::Get_Instance()->Scroll_Lock();

    IsStageLimitOver();
    Check_Clear();
    Create_SnapShot();
    Start_Replay();
    m_iCount++;
}

void CStage::Render(HDC hDC)
{
    HDC		hGroundDC = CBmpMgr::Get_Instance()->Find_Image(L"stage1");
    int		iScrollX = (INT)CScrollMgr::Get_Instance()->Get_ScrollX();
    int		iScrollY = (INT)CScrollMgr::Get_Instance()->Get_ScrollY();

    BitBlt(hDC,
        iScrollX,
        iScrollY,
        1950, 1400,
        hGroundDC,
        0, 0,
        SRCCOPY);

    //이펙트가 먼저.
    CEffectMgr::Get_Instance()->Render(hDC);

    CObjMgr::Get_Instance()->Render(hDC);

    //필터 적용 안되는 이펙트
    CEffectMgr::Get_Instance()->Render_Front(hDC);
    CLineMgr::Get_Instance()->Render(hDC);
    CRectMgr::Get_Instance()->Render(hDC);

    /*
        TCHAR szName[64] = L"";

    swprintf_s(szName, L"Player Pos: %f %f", CObjMgr::Get_Instance()->Get_Player()->Get_Info()->fX, CObjMgr::Get_Instance()->Get_Player()->Get_Info()->fY);
    TextOut(hDC, 25, 80, szName, lstrlen(szName));

    swprintf_s(szName, L"Resolution %d %d", WINCX, WINCY);
    TextOut(hDC, 25, 100, szName, lstrlen(szName));

    swprintf_s(szName, L"Player Sprite Size: %f %f", CObjMgr::Get_Instance()->Get_Player()->Get_SpriteInfo()->fCX, CObjMgr::Get_Instance()->Get_Player()->Get_SpriteInfo()->fCY);
    TextOut(hDC, 25, 140, szName, lstrlen(szName));

    swprintf_s(szName, L"Mouse Pos: %f %f", CObjMgr::Get_Instance()->Get_Mouse()->Get_Info()->fX, CObjMgr::Get_Instance()->Get_Mouse()->Get_Info()->fY);
    TextOut(hDC, 25, 160, szName, lstrlen(szName));

    swprintf_s(szName, L"Scroll Pos: %f %f", CScrollMgr::Get_Instance()->Get_ScrollX(), CScrollMgr::Get_Instance()->Get_ScrollY());
    TextOut(hDC, 25, 180, szName, lstrlen(szName));

    swprintf_s(szName, L"Player to Mouse Angle: % f", dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_AttackAngle());
    TextOut(hDC, 25, 200, szName, lstrlen(szName));

    swprintf_s(szName, L"Collided: %d", dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_Collided());
    TextOut(hDC, 25, 220, szName, lstrlen(szName));

    swprintf_s(szName, L"Slow Mod: %d", g_bSlowMod);
    TextOut(hDC, 25, 240, szName, lstrlen(szName));

    swprintf_s(szName, L"Slow Gauge: %d", dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_SlowGauge());
    TextOut(hDC, 25, 260, szName, lstrlen(szName));

    swprintf_s(szName, L"Player Dead: %d", dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_Dead());
    TextOut(hDC, 25, 280, szName, lstrlen(szName));
    */

    if (g_iStageReplayCount == 1)
    {
        HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Fade_Out");

        GdiTransparentBlt(hDC,/// 복사 받을 dc
            m_iFadeCount * 32,		// 복사 받을 위치 좌표 left
            0,					// 복사 받을 위치 좌표 top
            896,				// 복사 받을 가로 사이즈
            600,				// 복사 받을 세로 사이즈
            hMemDC,							// 복사할 이미지 dc
            0, 0,
            896, 576,			// 복사할 이미지의 세로
            RGB(255, 255, 255));

        if (GetTickCount64() - m_fFadeOutTimer >= 18)
        {
            m_iFadeCount++;
            m_fFadeOutTimer = GetTickCount64();
        }

        if (m_iFadeCount >= 32)
            g_iStageReplayCount++;
    }
}

void CStage::Release()
{
    // 마우스는.. 다시 써도 될듯?
    // UI는.. 몰루..
    /*
        CObjMgr::Get_Instance()->Delete_Object(OBJ_PLAYER);
    CObjMgr::Get_Instance()->Delete_Object(OBJ_MONSTER);
    CObjMgr::Get_Instance()->Delete_Object(OBJ_BULLET);
    */

    CEffectMgr::Destroy_Instance();
    CObjMgr::Destroy_Instance();
    CReplayMgr::Destroy_Instance();
    CRectMgr::Destroy_Instance();
    CLineMgr::Destroy_Instance();
}

void CStage::Create_SnapShot()
{
    // 15프레임마다 녹화. 프레임이 아니라 시간 단위로 하면 안되나 싶긴한데...
    if ( (!g_bSlowMod && m_iCount % 10 == 0) || (g_bSlowMod && m_iCount % 20 == 0))
    {
        CReplayMgr::Get_Instance()->Create_Snapshot();
        CReplayMgr::Get_Instance()->Add_Id();
    }
}

void CStage::Check_Clear()
{
    list<CObj*> Monsterlist = CObjMgr::Get_Instance()->Get_Monsterlist();

    for (auto& Src : Monsterlist)
    {
        if (Src->Get_Dead() == false)
            return;
    }

    m_bCleared = true; 

    //포탈 생성 로직. 일단 보스 스테이지엔 필요 없음.
    if (m_bCleared && !m_bPortalCreated)
    {
        CRectMgr::Get_Instance()->Add_Rect(new CRect({ 1904,0, 1949,600 }, TYPE_PORTAL));

        //GO UI 추가해서 플레이어 위치 추적하게 하고..
        m_bPortalCreated = true;
    }
}

void CStage::Start_Replay()
{
    if (g_bGameOver == true && CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON) && !g_bReplaying)
    {
        g_bReplaying = true;
        CSoundMgr::Get_Instance()->PlaySound(L"player_replay.wav", SOUND_EFFECT, g_fVolume);
        CUIMgr::Get_Instance()->Delete_Focused_UI();
    }
}

void CStage::Restart()
{
    CEffectMgr::Destroy_Instance();
    CObjMgr::Destroy_Instance();
    CScrollMgr::Destroy_Instance();
    CUIMgr::Destroy_Instance();
    CLineMgr::Destroy_Instance();
    CRectMgr::Destroy_Instance();
    CReplayMgr::Destroy_Instance();
    g_bGameOver = false;
    Initialize();
}
