#include "pch.h"
#include "CStage3.h"
#include "CBmpMgr.h"
#include "CObjMgr.h"
#include "CAbstractFactory.h"
#include "CScrollMgr.h"
#include "CLineMgr.h"
#include "CRectMgr.h"
#include "CCollisionMgr.h"
#include "CUIMgr.h"
#include "CMonsterRect.h"
#include "CKeyMgr.h"
#include "CReplayMgr.h"
#include "CEffectMgr.h"

CStage3::CStage3() : m_bBossDead(false), m_bMonsterSpawn(false), m_iCount(0), m_bBombInstalled(false), m_iExplosionCount(0)
{
}

CStage3::~CStage3()
{
	Release();
}

void CStage3::Initialize()
{
    m_bExplode = false;
    m_iBombCount = 0;
    m_bMonsterDead = false;
    m_bBossDead = false;
    m_bMonsterSpawn = false;
    m_iFadeCount = 0;
    m_fFadeOutTimer = GetTickCount64();
    g_iStageReplayCount++;
    m_iCount = 0;
    m_fStageStartTime = GetTickCount64();
    //bgm 추가
    CSoundMgr::Get_Instance()->PlayBGM(L"stage3_bg.ogg", g_fVolume);
    //bg 추가
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/stage3.bmp", L"stage3");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/stage3_explode.bmp", L"stage3_explode");
    //stage_3_explode

    //이펙트 매니저 이니셜라이즈
    CEffectMgr::Get_Instance()->Initialize();

    //지형지물 추가
    //CLineMgr::Get_Instance()->Add_Line(new CLine({ 0,520 }, { 1344,520 }, TYPE_GROUND));
    CLineMgr::Get_Instance()->Add_Line(new CLine({ 0,578 }, { 1344,578 }, TYPE_GROUND));

    // Rect는 총 5개 추가해야함.
    CRectMgr::Get_Instance()->Add_Rect(new CRect({ 0,352, 130, 390}, TYPE_WALL)); // 왼쪽 천장
    CRectMgr::Get_Instance()->Add_Rect(new CRect({ 112, 123, 143, 420 }, TYPE_WALL)); // 수직 벽 (왼쪽)
    CRectMgr::Get_Instance()->Add_Rect(new CMonsterRect({ 90, 123, 143, 1300 }, TYPE_MONSTER_WALL)); // 몬스터 벽(왼쪽)
    CRectMgr::Get_Instance()->Add_Rect(new CRect({ 1201, 123, 1232, 420 }, TYPE_WALL)); // 수직 벽 (오른쪽)
    CRectMgr::Get_Instance()->Add_Rect(new CMonsterRect({ 1201, 123, 1252, 1300 }, TYPE_MONSTER_WALL)); // 몬스터 벽 (오른쪽)
    CRectMgr::Get_Instance()->Add_Rect(new CRect({ 128, 0, 1207, 154 }, TYPE_WALL)); // 맨 위쪽 천장
    CRectMgr::Get_Instance()->Add_Rect(new CRect({ 1214,352, 1344, 390 }, TYPE_WALL)); // 오른쪽 천장

    //CRectMgr::Get_Instance()->Add_Rect(new CRect({ 1214,578, 1344,578 }, TYPE_WALL));
    //CRectMgr::Get_Instance()->Add_Rect(new CRect({ 130,578, 1213,578 }, TYPE_WALL));
    //CRectMgr::Get_Instance()->Add_Rect(new CRect({ 1214,578, 1344,578 }, TYPE_WALL));

    //obj 추가
    CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CPlayer>::Create_Obj(100.f, 543.f));
    CObjMgr::Get_Instance()->Add_Object(OBJ_MOUSE, CAbstractFactory<CMouse>::Create_Obj());
    //Monster 추가
    CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CHeadHunter>::Create_Obj(1000, 558));

    //CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CGrunt>::Create_Obj(250, 568));
    //CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CGrunt>::Create_Obj(836, 568));
    //CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CGrunt>::Create_Obj(1176, 568));

    //UI 추가
    CUI* pHUD = new CHUD();
    pHUD->Initialize();
    CUIMgr::Get_Instance()->Add_UI(pHUD);

    //scroll 최대최소 설정
    CScrollMgr::Get_Instance()->Set_ScrollPos(0, -30.f);
    CScrollMgr::Get_Instance()->Set_ScrollMinX(0.f);
    CScrollMgr::Get_Instance()->Set_ScrollMaxX(1344.f);

    CScrollMgr::Get_Instance()->Set_ScrollMinY(0.f);
    CScrollMgr::Get_Instance()->Set_ScrollMaxY(784.f);

    //리플레이 매니저 추적 시작.
    CReplayMgr::Get_Instance()->Set_ObjList(CObjMgr::Get_Instance()->Get_Playerlist_Ref(), OBJ_PLAYER);
    CReplayMgr::Get_Instance()->Set_ObjList(CObjMgr::Get_Instance()->Get_Monsterlist_Ref(), OBJ_MONSTER);
}

void CStage3::Update()
{
    CObjMgr::Get_Instance()->Update();
}

void CStage3::Late_Update()
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
    Platform_Bomb();
    Create_SnapShot();
    Start_Replay();
    m_iCount++;
}

void CStage3::Render(HDC hDC)
{
    HDC		hGroundDC;

    if(m_bExplode == false)
        hGroundDC = CBmpMgr::Get_Instance()->Find_Image(L"stage3");
    else
        hGroundDC = CBmpMgr::Get_Instance()->Find_Image(L"stage3_explode");

    int		iScrollX = (INT)CScrollMgr::Get_Instance()->Get_ScrollX();
    int		iScrollY = (INT)CScrollMgr::Get_Instance()->Get_ScrollY();

    BitBlt(hDC,
        iScrollX,
        iScrollY,
        1344, 784,
        hGroundDC,
        0, 0,
        SRCCOPY);

    CEffectMgr::Get_Instance()->Render(hDC);
    CObjMgr::Get_Instance()->Render(hDC);
    CEffectMgr::Get_Instance()->Render_Front(hDC);
    CLineMgr::Get_Instance()->Render(hDC);
    CRectMgr::Get_Instance()->Render(hDC);

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

void CStage3::Release()
{
    // 마우스는.. 다시 써도 될듯?
    CEffectMgr::Destroy_Instance();
    CObjMgr::Destroy_Instance();
    CReplayMgr::Destroy_Instance();
    CRectMgr::Destroy_Instance();
    CLineMgr::Destroy_Instance();;
}

void CStage3::Create_SnapShot()
{
    // 15프레임마다 녹화. 프레임이 아니라 시간 단위로 하면 안되나 싶긴한데...
    if ((!g_bSlowMod && m_iCount % 10 == 0) || (g_bSlowMod && m_iCount % 20 == 0))
    {
        CReplayMgr::Get_Instance()->Create_Snapshot();
        CReplayMgr::Get_Instance()->Add_Id();
    }
}

void CStage3::Check_Clear()
{
    list<CObj*> Monsterlist = CObjMgr::Get_Instance()->Get_Monsterlist();

    for (auto& Src : Monsterlist)
    {
        if (Src->Get_Dead() == false)
            return;
    }

    if (m_bBossDead == false)
    {
        m_bBossDead = true;
        m_fSpawnTimer = GetTickCount64();
    }

    //포탈 생성 로직. 일단 보스 스테이지엔 필요 없음.
    if (m_bBossDead && GetTickCount64() - m_fSpawnTimer >= 1500 && m_bMonsterSpawn == false)
    {
        CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CGrunt>::Create_Obj(531, 543));
        CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CBold>::Create_Obj(1100, 543));
        CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CGrunt>::Create_Obj(531, 543));
        CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CBold>::Create_Obj(300, 543));
        CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CGrunt>::Create_Obj(500, 543));
        CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CBold>::Create_Obj(700, 543));
        //CRectMgr::Get_Instance()->Add_Rect(new CRect({ 1251, 0, 1302, 1300 }, TYPE_PORTAL));

        m_bMonsterSpawn = true;
    }

    if (m_bBossDead && m_bMonsterSpawn == true && m_bMonsterDead == false) // 몬스터가 모두 죽었으면 바닥 폭발시키기
    {
        list<CObj*> Monsterlist = CObjMgr::Get_Instance()->Get_Monsterlist();
        bool m_bFlag;

        for (auto& Src : Monsterlist)
        {
            if (Src->Get_Dead() == false)
                return;
        }

        m_bMonsterDead = true;
        m_fBombTimer = GetTickCount64();
    }
}

void CStage3::Start_Replay()
{
    if (g_bGameOver == true && CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON) && !g_bReplaying)
    {
        g_bReplaying = true;
        CSoundMgr::Get_Instance()->PlaySound(L"player_replay.wav", SOUND_EFFECT, g_fVolume);
        CUIMgr::Get_Instance()->Delete_Focused_UI();
    }
}

void CStage3::Restart()
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

void CStage3::Platform_Bomb()
{
    if (m_bMonsterDead)
    {
        if (GetTickCount64() - m_fBombTimer >= 80 && m_iBombCount < 20)
        {
            CEffect* pEffect = new CRemainEffect(m_iBombCount * 60, 575, L"Bomb");
            pEffect->Initialize();
            CEffectMgr::Get_Instance()->Add_Front_Effect(pEffect);
            CSoundMgr::Get_Instance()->PlaySound(L"bomb.wav", SOUND_EFFECT, g_fVolume);
            m_iBombCount++;
            m_fBombTimer = GetTickCount64();
        }

        if (m_iBombCount == 20)
            m_bBombInstalled = true;

        if (GetTickCount64() - m_fBombTimer >= 80 && m_bBombInstalled && m_iBombCount >= 20 && m_iBombCount <= 40)
        {
            CEffect* pEffect = new CRemainEffect((m_iBombCount - 20) * 60, 575, L"Explosion");
            pEffect->Initialize();
            CEffectMgr::Get_Instance()->Add_Front_Effect(pEffect);
            CSoundMgr::Get_Instance()->PlaySound(L"explosion.wav", SOUND_EFFECT, g_fVolume);
            m_iBombCount++;
            m_fBombTimer = GetTickCount64();
        }

        if (m_iBombCount == 35)
            m_bExplode = true;

        if (m_iBombCount == 40)
        {
            CLineMgr::Destroy_Instance();
            CRectMgr::Get_Instance()->Add_Rect(new CRect({ 0,900, 1344,1000 }, TYPE_PORTAL));
            m_iBombCount++;
        }
    }
}
