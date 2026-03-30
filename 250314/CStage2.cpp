#include "pch.h"
#include "CStage2.h"
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

CStage2::CStage2() : m_bCleared(false), m_bPortalCreated(false), m_iCount(0)
{
}

CStage2::~CStage2()
{
    Release();
}

void CStage2::Initialize()
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
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/stage2.bmp", L"stage2");

    //이펙트 매니저 이니셜라이즈
    CEffectMgr::Get_Instance()->Initialize();

    //지형지물 추가
    CLineMgr::Get_Instance()->Add_Line(new CLine({ 0,1088 }, { 420,1088 }, TYPE_GROUND)); //시작 선
    CLineMgr::Get_Instance()->Add_Line(new CLine({ 420,1088 }, { 672,833 }, TYPE_GROUND)); // 계단
    CLineMgr::Get_Instance()->Add_Line(new CLine({ 0,832 }, { 380,832 }, TYPE_GROUND)); // 맨 왼쪽 플랫폼
    CLineMgr::Get_Instance()->Add_Line(new CLine({ 672, 833 }, { 1183,833 }, TYPE_GROUND)); // 계단 이후 선
    CLineMgr::Get_Instance()->Add_Line(new CLine({ 1152, 1088 }, { 2824,1088 }, TYPE_GROUND)); // 지하 선
    CLineMgr::Get_Instance()->Add_Line(new CLine({ 1312, 833 }, { 2303,833 }, TYPE_GROUND)); // 1층 선
    CLineMgr::Get_Instance()->Add_Line(new CLine({ 2432, 833 }, { 3071,833 }, TYPE_GROUND)); // 1층 선 2

    CLineMgr::Get_Instance()->Add_Line(new CLine({ 960,576 }, { 1698, 576 }, TYPE_GROUND)); // 2층 선 1
    CLineMgr::Get_Instance()->Add_Line(new CLine({ 1856,576 }, {2561,576 }, TYPE_GROUND)); // 2층 선 2

    CRectMgr::Get_Instance()->Add_Rect(new CRect{ {0,837 , 367,895}, TYPE_WALL}); 
    CRectMgr::Get_Instance()->Add_Rect(new CRect{ {0,576 , 95,837}, TYPE_WALL });
    CRectMgr::Get_Instance()->Add_Rect(new CRect{ {96,576 , 942,639}, TYPE_WALL }); // 첫 천장

    CRectMgr::Get_Instance()->Add_Rect(new CRect{ {942,640 , 985,703}, TYPE_WALL }); // 2층 왼쪽 벽 1


    CRectMgr::Get_Instance()->Add_Rect(new CRect{ {2700,576, 3071,640 },TYPE_WALL }); // 2층 천장 4

    CRectMgr::Get_Instance()->Add_Rect(new CRect{ {2700,384, 2749,640 },TYPE_WALL }); // 2층 왼쪽벽 1

    CRectMgr::Get_Instance()->Add_Rect(new CRect{ {1516,640, 1690,703},TYPE_WALL }); // 1층 천장 1
    CRectMgr::Get_Instance()->Add_Rect(new CRect{ {1868,640, 2041,703},TYPE_WALL }); // 1층 천장 2
    CRectMgr::Get_Instance()->Add_Rect(new CRect{ {2508,640, 2553,703},TYPE_WALL }); // 1층 천장 3
    CRectMgr::Get_Instance()->Add_Rect(new CRect{ {2700,640, 2745,703},TYPE_WALL }); // 1층 천장 4

    CRectMgr::Get_Instance()->Add_Rect(new CRect{ {1328,836, 2292, 895},TYPE_WALL }); // 1층 선 밑 플랫폼
    CRectMgr::Get_Instance()->Add_Rect(new CRect{ {2448,836, 3071, 895},TYPE_WALL }); // 1층 선 2 밑 플랫폼

    CRectMgr::Get_Instance()->Add_Rect(new CRect{ {983,579, 1690, 640}, TYPE_WALL }); // 2층 선 밑 플랫폼 1
    CRectMgr::Get_Instance()->Add_Rect(new CRect{ {1868,579, 2553, 640},TYPE_WALL }); // 2층 선 밑 플랫폼 2

    CRectMgr::Get_Instance()->Add_Rect(new CRect{ {928,320, 971,576}, TYPE_WALL }); // 2층 왼쪽 벽
    CRectMgr::Get_Instance()->Add_Rect(new CRect{ {928,320, 1503, 383}, TYPE_WALL }); // 2층 왼쪽 벽
    CRectMgr::Get_Instance()->Add_Rect(new CRect{ {1520, 192, 1559, 447}, TYPE_WALL }); // 2층 왼쪽 벽2
    CRectMgr::Get_Instance()->Add_Rect(new CRect{ {1472, 160, 2303, 223}, TYPE_WALL }); // 2층 맨 위 천장
    CRectMgr::Get_Instance()->Add_Rect(new CRect{ {2220, 194, 2265, 446}, TYPE_WALL }); // 2층 오른쪽 벽2
    CRectMgr::Get_Instance()->Add_Rect(new CRect{ {2231, 349, 2721, 383}, TYPE_WALL }); // 2층 오른쪽 천장

    CRectMgr::Get_Instance()->Add_Rect(new CRect{ {1140,840, 1165,1087},TYPE_WALL }); // 지하 벽 1
    CRectMgr::Get_Instance()->Add_Rect(new CRect{ {2836,896, 2847,1087},TYPE_WALL }); // 지하 벽 2
    CRectMgr::Get_Instance()->Add_Rect(new CRect{ {1836,886, 1879, 959},TYPE_WALL }); // 지하 천장 1
    CRectMgr::Get_Instance()->Add_Rect(new CRect{ {2255,886, 2292, 959},TYPE_WALL }); // 지하 천장 2
    CRectMgr::Get_Instance()->Add_Rect(new CRect{ {2448,840, 2489, 959},TYPE_WALL }); // 지하 천장 3
    //obj 추가
    CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CPlayer>::Create_Obj(217.f, 1068.f));
    CObjMgr::Get_Instance()->Add_Object(OBJ_MOUSE, CAbstractFactory<CMouse>::Create_Obj());
    
    //아이템 추가
    CObjMgr::Get_Instance()->Add_Object(OBJ_ITEM, CAbstractFactory<CItem>::Create_Obj(303.f, 1086.f));
    CObjMgr::Get_Instance()->Add_Object(OBJ_ITEM, CAbstractFactory<CItem>::Create_Obj(1508.f, 568.f));
    //Monster 추가

    //스타트 라인
    CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CBold>::Create_Obj(250, 796));
    CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CGrunt>::Create_Obj(531, 940));
    CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CBold>::Create_Obj(1100, 818));
    

    //2층 플랫폼 (왼쪽)
    CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CBold>::Create_Obj(1160, 538));
    CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CGrunt>::Create_Obj(1440, 538));
    CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CGrunt>::Create_Obj(1632, 538));
    //2층 플랫폼 (오른쪽)
    CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CGrunt>::Create_Obj(2208, 538));
    CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CBold>::Create_Obj(1944, 538));
    CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CGrunt>::Create_Obj(2506, 538));

    //1층 플랫폼
    CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CGrunt>::Create_Obj(1598, 796));
    CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CBold>::Create_Obj(1832, 796));
    CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CGrunt>::Create_Obj(2120, 796));

    //지하
    CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CBold>::Create_Obj(1560, 1054));
    CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CGrunt>::Create_Obj(1820, 1054));
    CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CGrunt>::Create_Obj(2295, 1054));
    CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CBold>::Create_Obj(2734, 1054));
    //CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CGrunt>::Create_Obj(836, 734));
    //CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CGrunt>::Create_Obj(1643, 392));

    //UI 추가
    CUI* pHUD = new CHUD();
    pHUD->Initialize();
    CUIMgr::Get_Instance()->Add_UI(pHUD);

    //scroll 최대최소 설정, 시작 좌표
    CScrollMgr::Get_Instance()->Set_ScrollPos(0, -700.f);
    CScrollMgr::Get_Instance()->Set_ScrollMinX(0.f);
    CScrollMgr::Get_Instance()->Set_ScrollMaxX(3000.f);

    CScrollMgr::Get_Instance()->Set_ScrollMinY(0.f);
    CScrollMgr::Get_Instance()->Set_ScrollMaxY(1280.f);

    //리플레이 매니저 추적 시작.
    CReplayMgr::Get_Instance()->Set_ObjList(CObjMgr::Get_Instance()->Get_Playerlist_Ref(), OBJ_PLAYER);
    CReplayMgr::Get_Instance()->Set_ObjList(CObjMgr::Get_Instance()->Get_Monsterlist_Ref(), OBJ_MONSTER);
}

void CStage2::Update()
{
    CObjMgr::Get_Instance()->Update();
}

void CStage2::Late_Update()
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

void CStage2::Render(HDC hDC)
{
    HDC		hGroundDC = CBmpMgr::Get_Instance()->Find_Image(L"stage2");
    int		iScrollX = (INT)CScrollMgr::Get_Instance()->Get_ScrollX();
    int		iScrollY = (INT)CScrollMgr::Get_Instance()->Get_ScrollY();

    BitBlt(hDC,
        iScrollX,
        iScrollY,
        3072, 1280,
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

void CStage2::Release()
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

void CStage2::Create_SnapShot()
{
    // 15프레임마다 녹화. 프레임이 아니라 시간 단위로 하면 안되나 싶긴한데...
    if ((!g_bSlowMod && m_iCount % 10 == 0) || (g_bSlowMod && m_iCount % 20 == 0))
    {
        CReplayMgr::Get_Instance()->Create_Snapshot();
        CReplayMgr::Get_Instance()->Add_Id();
    }
}

void CStage2::Check_Clear()
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
        CRectMgr::Get_Instance()->Add_Rect(new CRect({ 3016,641, 3070,831 }, TYPE_PORTAL));

        //GO UI 추가해서 플레이어 위치 추적하게 하고..
        m_bPortalCreated = true;
    }
}

void CStage2::Start_Replay()
{
    if (g_bGameOver == true && CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON) && !g_bReplaying)
    {
        g_bReplaying = true;
        CSoundMgr::Get_Instance()->PlaySound(L"player_replay.wav", SOUND_EFFECT, g_fVolume);
        CUIMgr::Get_Instance()->Delete_Focused_UI();
    }
}

void CStage2::Restart()
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
