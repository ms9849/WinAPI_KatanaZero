#include "pch.h"
#include "CHeadHunter_2.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"
#include "CObjMgr.h"
#include "CLineMgr.h"
#include "CSoundMgr.h"
#include "CEffectMgr.h"

CHeadHunter_2::CHeadHunter_2() : m_eCurMotion(IDLE), m_ePreMotion(MOTION_END), m_ePattern(NONE), m_ePrePattern(PATTERN_END), m_pTarget(nullptr), m_bIsHitCeil(false), m_bJump(false),
m_bWallGrap(false), m_bBulletSweep(false), m_bHeadCut(false)
{
}

CHeadHunter_2::~CHeadHunter_2()
{
	Release();
}

void CHeadHunter_2::Initialize()
{
    m_iAerialCount = 0;
    m_iDeathCount = 3; // 3번 죽으면 사망
    m_fSpeed = 8.f;
    m_tInfo.fCX = 44;
    m_tInfo.fCY = 88;
    m_tSpriteInfo = m_tInfo;
    m_fTime = 0.f;
    m_fVelocity = 14.f;
    m_fBulletAngle = 0.f;

    m_tGroundCheckPoint[0] = { 1000, 543 };
    m_tGroundCheckPoint[1] = { 300, 543 };
    
    //좌표는 임시로 둔거. 추후 수정해야함.
    m_tAerialCheckPoint[0] = { 372, 250 };
    m_tAerialCheckPoint[1] = { 672, 250 };
    m_tAerialCheckPoint[2] = { 972, 250 };

    m_tAerialSweepCheckPoint[0] = {400, 250};
    m_tAerialSweepCheckPoint[1] = {944, 250};

    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/HeadHunter/HeadHunter_Idle_12x2.bmp", L"HeadHunter_Idle");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/HeadHunter/HeadHunter_Roll_7x2.bmp", L"HeadHunter_Roll");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/HeadHunter/HeadHunter_Draw_Gun_8x2.bmp", L"HeadHunter_Draw_Gun");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/HeadHunter/HeadHunter_Aim_19x2.bmp", L"HeadHunter_Aim");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/HeadHunter/HeadHunter_Aim_Laser.bmp", L"HeadHunter_Aim_Laser");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/HeadHunter/HeadHunter_Pre_Rush_8x2.bmp", L"HeadHunter_Pre_Rush");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/HeadHunter/HeadHunter_Rush_1x2.bmp", L"HeadHunter_Rush");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/HeadHunter/HeadHunter_Sweep_18x2.bmp", L"HeadHunter_Sweep");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/HeadHunter/HeadHunter_WallGrap_1x2.bmp", L"HeadHunter_WallGrap");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/HeadHunter/HeadHunter_Teleport_4x2.bmp", L"HeadHunter_Teleport");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/HeadHunter/HeadHunter_Death_Jump_4x2.bmp", L"HeadHunter_Death_Jump");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/HeadHunter/HeadHunter_Dead_19x2.bmp", L"HeadHunter_Death_Ground");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/HeadHunter/HeadHunter_Nohead_1x2.bmp", L"HeadHunter_No_Head");
    //총알 난사, 108 x 124
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/HeadHunter/HeadHunter_Bullet_Sweep_7x2.bmp", L"HeadHunter_Bullet_Sweep");
    //공중 공격 시작, 56, 90
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/HeadHunter/HeadHunter_Aerial_Attack_In_4x2.bmp", L"HeadHunter_Aerial_Attack_In");
    //공중 공격 종료, 56, 90
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/HeadHunter/HeadHunter_Aerial_Attack_Out_4x2.bmp", L"HeadHunter_Aerial_Attack_Out");

    m_pFrameKey = L"HeadHunter_Idle";
    m_tFrame.iStart = 0;
    m_tFrame.iEnd = 11;
    m_tFrame.iMotion = 0;
    m_tFrame.dwTime = GetTickCount64();
    m_tFrame.dwFrameSpeed = 75.f; //추후 time 변수 추가

    m_pTarget = dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());
    m_fAttackDelayTime = GetTickCount64();
}

int CHeadHunter_2::Update()
{
    __super::Update_Rect();
    return NOEVENT;
}

void CHeadHunter_2::Late_Update()
{
    if (m_bDead)
    {
        Death_Bound();
        Move();
        __super::Update_Frame();
        __super::Update_Sprite();
        return;
    }

    Calc_Target();
    Update_Pattern();
    Dodge();
    Ground_Laser();
    Rush();
    Bullet_Sweep();
    Aerial_Sweep();
    Aerial_Bullet();
    Aerial_Laser();
    Teleport();

    if(!(m_ePattern == BULLET_SWEEP && isAnimationEnd()))
        __super::Update_Frame();

    __super::Update_Sprite();

    m_tInfo.fY += 1.0f * g_fSlowFactor;
    Motion_Change();
}

void CHeadHunter_2::Render(HDC hDC)
{
    int		iScrollX = (INT)CScrollMgr::Get_Instance()->Get_ScrollX();
    int		iScrollY = (INT)CScrollMgr::Get_Instance()->Get_ScrollY();

    if (!g_bMonoChrome)
    {
        HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

        GdiTransparentBlt(hDC,/// 복사 받을 dc
            m_tSpriteInfo.fX - m_tSpriteInfo.fCX / 2.f + iScrollX,		// 복사 받을 위치 좌표 left
            m_tSpriteInfo.fY - m_tSpriteInfo.fCY / 2.f + iScrollY,					// 복사 받을 위치 좌표 top
            (int)m_tSpriteInfo.fCX,				// 복사 받을 가로 사이즈
            (int)m_tSpriteInfo.fCY,				// 복사 받을 세로 사이즈
            hMemDC,							// 복사할 이미지 dc
            m_tFrame.iStart * (int)m_tSpriteInfo.fCX,
            m_tFrame.iMotion * (int)m_tSpriteInfo.fCY,							// 복사할 이미지의 left, top
            (int)m_tSpriteInfo.fCX,				// 복사할 이미지의 가로
            (int)m_tSpriteInfo.fCY,				// 복사할 이미지의 세로
            RGB(255, 0, 255));

        if (m_pFrameKey == L"HeadHunter_Aim" && !isAnimationEnd() && GetTickCount64() - m_fLaserDelayTime <= 500 / g_fSlowFactor)
        {
            HPEN hPen = CreatePen(PS_SOLID, (500 - (GetTickCount64() - m_fLaserDelayTime)) / 200 + 1, RGB(255, 255, 0)); // 빨간색 예시
            HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);

            MoveToEx(hDC, m_tInfo.fX + 30 * cosf(m_fTargetAngle * PI / 180.f) + iScrollX, m_tInfo.fY - 30 * sinf(m_fTargetAngle * PI / 180.f) + iScrollY, NULL);
            LineTo(hDC, m_tInfo.fX + 1500 * cosf(m_fTargetAngle * PI / 180.f) + iScrollX, m_tInfo.fY - 1500 * sinf(m_fTargetAngle * PI / 180.f) + iScrollY);

            SelectObject(hDC, hOldPen);
            DeleteObject(hPen);
        }
    }
    else
    {
        const int width = (int)m_tSpriteInfo.fCX;
        const int height = (int)m_tSpriteInfo.fCY;

        BITMAPINFO bmi = {};
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = width;
        bmi.bmiHeader.biHeight = -height; // top-down
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        void* pPixels = nullptr;
        HDC hMemDC = CreateCompatibleDC(hDC);
        HBITMAP hDibBmp = CreateDIBSection(hDC, &bmi, DIB_RGB_COLORS, &pPixels, nullptr, 0);
        HBITMAP hOldBmp = (HBITMAP)SelectObject(hMemDC, hDibBmp);

        BitBlt(
            hMemDC,
            0, 0,
            width, height,
            CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey),
            m_tFrame.iStart * width,
            m_tFrame.iMotion * height,
            SRCCOPY
        );

        // 픽셀 직접 수정
        DWORD* pixels = (DWORD*)pPixels;
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                DWORD& px = pixels[y * width + x];
                BYTE r = GetRValue(px);
                BYTE g = GetGValue(px);
                BYTE b = GetBValue(px);

                if (!(r == 255 && g == 0 && b == 255))
                {
                    r = 0;
                    g = 0;
                    b = 0;
                    px = (r << 16) | (g << 8) | b;
                }
            }
        }
        GdiTransparentBlt(hDC,/// 복사 받을 dc
            m_tSpriteInfo.fX - m_tSpriteInfo.fCX / 2.f + iScrollX,		// 복사 받을 위치 좌표 left
            m_tSpriteInfo.fY - m_tSpriteInfo.fCY / 2.f + iScrollY,					// 복사 받을 위치 좌표 top
            (int)m_tSpriteInfo.fCX,				// 복사 받을 가로 사이즈
            (int)m_tSpriteInfo.fCY,				// 복사 받을 세로 사이즈
            hMemDC,							// 복사할 이미지 dc
            0,
            0,							// 복사할 이미지의 left, top
            (int)m_tSpriteInfo.fCX,				// 복사할 이미지의 가로
            (int)m_tSpriteInfo.fCY,				// 복사할 이미지의 세로
            RGB(255, 0, 255));
        // 정리
        SelectObject(hMemDC, hOldBmp);
        DeleteObject(hDibBmp);
        DeleteDC(hMemDC);
    }
}

void CHeadHunter_2::Release()
{
}

void CHeadHunter_2::Calc_Target()
{
    if (nullptr != m_pTarget)
    {
        float fDistX = m_pTarget->Get_Info()->fX - m_tInfo.fX;
        float fDistY = m_pTarget->Get_Info()->fY - m_tInfo.fY;

        m_fTargetDist = sqrtf(fDistX * fDistX + fDistY * fDistY);

        if (m_pTarget->Get_Info()->fY < m_tInfo.fY)
            m_fTargetAngle = acosf(fDistX / m_fTargetDist) * 180 / PI;

        else
            m_fTargetAngle = 360.f - acosf(fDistX / m_fTargetDist) * 180 / PI;
    }
}

void CHeadHunter_2::Update_Pattern()
{
    //sweep은 벽에 닿고 일정 좌표 이하일때 발동함.
    if (GetTickCount64() - m_fAttackDelayTime >= 350 / g_fSlowFactor && m_ePattern == NONE)
    {
        // 회피를 제외하고 아무일 없이 0.5초 지났다면 패턴 진행
        if (GetTickCount64() - m_fAttackDelayTime >= 500 / g_fSlowFactor)
        {
            srand((unsigned)time(NULL));
            int iRand = rand() % 30;

            if (iRand >= 20)
            {
                m_eAerialDir = rand() % 2 == 0 ? LEFT : RIGHT;
                m_ePattern = AERIAL_LASER;
            }
            else if (iRand >= 10)
            {
                m_eAerialDir = rand() % 2 == 0 ? LEFT : RIGHT;
                m_ePattern = AERIAL_BULLET;
            }

            else
            {
                if (iRand >= 5)
                {
                    CSoundMgr::Get_Instance()->PlaySound(L"boss_teleport.wav", SOUND_EFFECT, g_fVolume);
                    if (m_tFrame.iMotion == 0)
                    {
                        m_ePattern = TELEPORT;
                        m_eCurMotion = RIGHT_TELEPORT;
                    }
                    else
                    {
                        m_ePattern = TELEPORT;
                        m_eCurMotion = LEFT_TELEPORT;
                    }
                }
                else
                {
                    m_ePattern = AERIAL_SWEEP;
                }
            }
        }
        //  플레이어가 공격했을때 가깝다면, 회피
        if (m_pTarget->Get_Motion() == CPlayer::LEFT_ATTACK || m_pTarget->Get_Motion() == CPlayer::RIGHT_ATTACK)
        {
            CSoundMgr::Get_Instance()->PlaySound(L"player_roll.wav", SOUND_EFFECT, g_fVolume);
            if (m_fTargetDist <= 400 && m_pTarget->Get_Info()->fX < m_tInfo.fX)
            {
                m_ePattern = DODGE;
                m_eCurMotion = LEFT_ROLL;
                m_tFrame.iStart = 6;
            }
            else if (m_fTargetDist <= 400 && m_pTarget->Get_Info()->fX > m_tInfo.fX)
            {
                m_ePattern = DODGE;
                m_eCurMotion = RIGHT_ROLL;
                m_tFrame.iStart = 0;
            }
        }
        // 땅 레이저 발사 패턴, 멀어야 발동
        else if (m_fTargetDist > 550 && m_ePrePattern != GROUND_LASER)
        {
            m_ePattern = GROUND_LASER;

            if (m_pTarget->Get_Info()->fX < m_tInfo.fX)
                m_eCurMotion = LEFT_DRAW_GUN;
            else
                m_eCurMotion = RIGHT_DRAW_GUN;
        }
        // 벽 점프
        else  if (m_fTargetDist < 200 && m_ePrePattern != RUSH && GetTickCount64() - m_fAttackDelayTime >= 1500 / g_fSlowFactor)
        {
            m_ePattern = RUSH;
            m_eCurMotion = m_tInfo.fX <= 672 ? RIGHT_PRE_RUSH : LEFT_PRE_RUSH;
            CSoundMgr::Get_Instance()->PlaySound(L"boss_pre_dash.wav", SOUND_EFFECT, g_fVolume);
        }

        m_ePrePattern = m_ePattern;
        Motion_Change();
    }
}

void CHeadHunter_2::Motion_Change()
{
    if (m_ePreMotion != m_eCurMotion)
    {
        switch (m_eCurMotion)
        {
        case IDLE:
            m_pFrameKey = L"HeadHunter_Idle";

            if (m_pTarget->Get_Info()->fX <= m_tInfo.fX) // 왼쪽 보게
            {
                m_tFrame.iStart = 11;
                m_tFrame.iEnd = 11;
                m_tFrame.iMotion = 1;
            }
            else
            {
                m_tFrame.iStart = 0;
                m_tFrame.iEnd = 11;
                m_tFrame.iMotion = 0;
            }
            m_tSpriteInfo.fCX = 44;
            m_tSpriteInfo.fCY = 88;

            m_tFrame.dwTime = GetTickCount64();
            m_tFrame.dwFrameSpeed = 75;
            break;
            //여기서 다른 케이스 추가.
        case LEFT_ROLL:
            m_pFrameKey = L"HeadHunter_Roll";

            m_tSpriteInfo.fCX = 118;
            m_tSpriteInfo.fCY = 96;

            m_tFrame.iStart = 6;
            m_tFrame.iEnd = 6;
            m_tFrame.iMotion = 1;

            m_tFrame.dwTime = GetTickCount64();
            m_tFrame.dwFrameSpeed = 50;

            break;

        case RIGHT_ROLL:
            m_pFrameKey = L"HeadHunter_Roll";

            m_tSpriteInfo.fCX = 118;
            m_tSpriteInfo.fCY = 96;

            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 6;
            m_tFrame.iMotion = 0;

            m_tFrame.dwTime = GetTickCount64();
            m_tFrame.dwFrameSpeed = 50;
            break;

        case LEFT_DRAW_GUN:
            m_pFrameKey = L"HeadHunter_Draw_Gun";

            m_tSpriteInfo.fCX = 76;
            m_tSpriteInfo.fCY = 86;

            m_tFrame.iStart = 7;
            m_tFrame.iEnd = 7;
            m_tFrame.iMotion = 1;

            m_tFrame.dwTime = GetTickCount64();
            m_tFrame.dwFrameSpeed = 75;
            break;
        case RIGHT_DRAW_GUN:
            m_pFrameKey = L"HeadHunter_Draw_Gun";

            m_tSpriteInfo.fCX = 76;
            m_tSpriteInfo.fCY = 86;

            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 7;
            m_tFrame.iMotion = 0;

            m_tFrame.dwTime = GetTickCount64();
            m_tFrame.dwFrameSpeed = 75;
            break;

        case LEFT_AIM:
            m_pFrameKey = L"HeadHunter_Aim";

            m_tSpriteInfo.fCX = 78;
            m_tSpriteInfo.fCY = 104;

            m_tFrame.iStart = 9;
            m_tFrame.iEnd = -100;
            m_tFrame.iMotion = 1;

            m_tFrame.dwTime = GetTickCount64();
            m_tFrame.dwFrameSpeed = 10000000;
            break;
        case RIGHT_AIM:
            m_pFrameKey = L"HeadHunter_Aim";

            m_tSpriteInfo.fCX = 78;
            m_tSpriteInfo.fCY = 104;

            m_tFrame.iStart = 9;
            m_tFrame.iEnd = 200;
            m_tFrame.iMotion = 0;

            m_tFrame.dwTime = GetTickCount64();
            m_tFrame.dwFrameSpeed = 10000000;
            break;

        case LEFT_PRE_RUSH:
            m_pFrameKey = L"HeadHunter_Pre_Rush";

            m_tSpriteInfo.fCX = 114;
            m_tSpriteInfo.fCY = 86;

            m_tFrame.iStart = 7;
            m_tFrame.iEnd = 7;
            m_tFrame.iMotion = 1;

            m_tFrame.dwTime = GetTickCount64();
            m_tFrame.dwFrameSpeed = 30;
            break;

        case RIGHT_PRE_RUSH:
            m_pFrameKey = L"HeadHunter_Pre_Rush";

            m_tSpriteInfo.fCX = 114;
            m_tSpriteInfo.fCY = 86;

            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 7;
            m_tFrame.iMotion = 0;

            m_tFrame.dwTime = GetTickCount64();
            m_tFrame.dwFrameSpeed = 30;
            break;

        case LEFT_RUSH:
            m_pFrameKey = L"HeadHunter_Rush";

            m_tSpriteInfo.fCX = 56;
            m_tSpriteInfo.fCY = 50;

            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 0;
            m_tFrame.iMotion = 1;

            m_tFrame.dwTime = GetTickCount64();
            m_tFrame.dwFrameSpeed = 30;
            break;
        case RIGHT_RUSH:
            m_pFrameKey = L"HeadHunter_Rush";

            m_tSpriteInfo.fCX = 56;
            m_tSpriteInfo.fCY = 50;

            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 0;
            m_tFrame.iMotion = 0;

            m_tFrame.dwTime = GetTickCount64();
            m_tFrame.dwFrameSpeed = 30;
            break;

        case LEFT_WALLGRAP:
            m_pFrameKey = L"HeadHunter_WallGrap";

            m_tSpriteInfo.fCX = 60;
            m_tSpriteInfo.fCY = 86;

            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 0;
            m_tFrame.iMotion = 1;

            m_tFrame.dwTime = GetTickCount64();
            m_tFrame.dwFrameSpeed = 30;
            break;

        case RIGHT_WALLGRAP:
            m_pFrameKey = L"HeadHunter_WallGrap";

            m_tSpriteInfo.fCX = 60;
            m_tSpriteInfo.fCY = 86;

            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 0;
            m_tFrame.iMotion = 0;

            m_tFrame.dwTime = GetTickCount64();
            m_tFrame.dwFrameSpeed = 30;
            break;

        case LEFT_TELEPORT:
            m_pFrameKey = L"HeadHunter_Teleport";

            m_tSpriteInfo.fCX = 72;
            m_tSpriteInfo.fCY = 62;

            m_tFrame.iStart = 3;
            m_tFrame.iEnd = 3;
            m_tFrame.iMotion = 1;

            m_tFrame.dwTime = GetTickCount64();
            m_tFrame.dwFrameSpeed = 50;
            break;

        case RIGHT_TELEPORT:
            m_pFrameKey = L"HeadHunter_Teleport";

            m_tSpriteInfo.fCX = 72;
            m_tSpriteInfo.fCY = 62;

            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 3;
            m_tFrame.iMotion = 0;

            m_tFrame.dwTime = GetTickCount64();
            m_tFrame.dwFrameSpeed = 50;
            break;

        case LEFT_DEATH_JUMP:
            m_pFrameKey = L"HeadHunter_Death_Jump";

            m_tSpriteInfo.fCX = 82;
            m_tSpriteInfo.fCY = 52;

            m_tFrame.iStart = 3;
            m_tFrame.iEnd = 3;
            m_tFrame.iMotion = 1;

            m_tFrame.dwTime = GetTickCount64();
            m_tFrame.dwFrameSpeed = 50;
            break;
        case RIGHT_DEATH_JUMP:
            m_pFrameKey = L"HeadHunter_Death_Jump";

            m_tSpriteInfo.fCX = 82;
            m_tSpriteInfo.fCY = 52;

            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 3;
            m_tFrame.iMotion = 0;

            m_tFrame.dwTime = GetTickCount64();
            m_tFrame.dwFrameSpeed = 50;
            break;

        case LEFT_DEATH_GROUND:
            m_pFrameKey = L"HeadHunter_Death_Ground";

            m_tSpriteInfo.fCX = 112;
            m_tSpriteInfo.fCY = 82;

            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 18;
            m_tFrame.iMotion = 0;

            m_tFrame.dwTime = GetTickCount64();
            m_tFrame.dwFrameSpeed = 200;
            break;

        case RIGHT_DEATH_GROUND:
            m_pFrameKey = L"HeadHunter_Death_Ground";

            m_tSpriteInfo.fCX = 112;
            m_tSpriteInfo.fCY = 82;

            m_tFrame.iStart = 18;
            m_tFrame.iEnd = 18;
            m_tFrame.iMotion = 1;

            m_tFrame.dwTime = GetTickCount64();
            m_tFrame.dwFrameSpeed = 200;
            break;

        case LEFT_NO_HEAD:
            m_pFrameKey = L"HeadHunter_No_Head";

            m_tSpriteInfo.fCX = 120;
            m_tSpriteInfo.fCY = 82;

            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 0;
            m_tFrame.iMotion = 0;

            m_tFrame.dwTime = GetTickCount64();
            m_tFrame.dwFrameSpeed = 200;
            break;

        case RIGHT_NO_HEAD:
            m_pFrameKey = L"HeadHunter_No_Head";

            m_tSpriteInfo.fCX = 120;
            m_tSpriteInfo.fCY = 82;

            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 0;
            m_tFrame.iMotion = 1;

            m_tFrame.dwTime = GetTickCount64();
            m_tFrame.dwFrameSpeed = 200;
            break;

        case LEFT_BULLET_SWEEP:
            m_pFrameKey = L"HeadHunter_Bullet_Sweep";

            m_tSpriteInfo.fCX = 108;
            m_tSpriteInfo.fCY = 124;

            m_tFrame.iStart = 6;
            m_tFrame.iEnd = 6;
            m_tFrame.iMotion = 1;

            m_tFrame.dwTime = GetTickCount64();
            m_tFrame.dwFrameSpeed = 75;
            break;

        case RIGHT_BULLET_SWEEP:
            m_pFrameKey = L"HeadHunter_Bullet_Sweep";

            m_tSpriteInfo.fCX = 108;
            m_tSpriteInfo.fCY = 124;

            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 6;
            m_tFrame.iMotion = 0;

            m_tFrame.dwTime = GetTickCount64();
            m_tFrame.dwFrameSpeed = 75;
            break;

        case LEFT_LASER_SWEEP:
            m_pFrameKey = L"HeadHunter_Sweep";

            m_tSpriteInfo.fCX = 118;
            m_tSpriteInfo.fCY = 92;

            m_tFrame.iStart = 17;
            m_tFrame.iEnd = 17;
            m_tFrame.iMotion = 1;

            m_tFrame.dwTime = GetTickCount64();
            m_tFrame.dwFrameSpeed = 75;
            break;

        case RIGHT_LASER_SWEEP:
            m_pFrameKey = L"HeadHunter_Sweep";

            m_tSpriteInfo.fCX = 118;
            m_tSpriteInfo.fCY = 92;

            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 17;
            m_tFrame.iMotion = 0;

            m_tFrame.dwTime = GetTickCount64();
            m_tFrame.dwFrameSpeed = 75;
            break;

        case AERIAL_ATTACK_IN:
            m_pFrameKey = L"HeadHunter_Aerial_Attack_In";

            m_tSpriteInfo.fCX = 56;
            m_tSpriteInfo.fCY = 90;

            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 3;
            m_tFrame.iMotion = 0;

            m_tFrame.dwTime = GetTickCount64();
            m_tFrame.dwFrameSpeed = 75;
            break;

        case AERIAL_ATTACK_OUT:
            m_pFrameKey = L"HeadHunter_Aerial_Attack_Out";

            m_tSpriteInfo.fCX = 56;
            m_tSpriteInfo.fCY = 90;

            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 3;
            m_tFrame.iMotion = 0;

            m_tFrame.dwTime = GetTickCount64();
            m_tFrame.dwFrameSpeed = 75;
            break;
        }


        m_ePreMotion = m_eCurMotion;
    }
}

void CHeadHunter_2::Move()
{
    if (m_bDead && m_bHeadCut == false && m_eCurMotion == LEFT_DEATH_GROUND || m_eCurMotion == RIGHT_DEATH_GROUND)
    {
        if (m_tInfo.fX <= 649)
        {
            m_tInfo.fX += 0.5f;
            m_eCurMotion = LEFT_DEATH_GROUND;
            Motion_Change();
        }
 

        else if (m_tInfo.fX > 650)
        {
            m_tInfo.fX -= 0.5f;
            m_eCurMotion = RIGHT_DEATH_GROUND;
            Motion_Change();
        }
    }
}

void CHeadHunter_2::HitActor(CObj* HitActor, HITPOINT _HitInfo)
{
}

void CHeadHunter_2::HitActor(CMap* HitActor, HITPOINT _HitInfo)
{
    if (HitActor->GetObjType() == TYPE_GROUND) // 선 충돌.
    {
        m_bIsHitCeil = false;
        if (m_eCurMotion == LEFT_DEATH_JUMP || m_eCurMotion == RIGHT_DEATH_JUMP)
        {
            m_fTime = 0.f;
            m_bJump = false;
            //진짜 죽음. 여기서 이벤트 발생시켜도 될거같은데? 일단 이건 나중에
            if (m_iDeathCount <= 0)
            {
                g_bMonoChrome = false;
                g_bSlowMod = false;
                g_fSlowFactor = 1.0f;
                CSoundMgr::Get_Instance()->Set_PlaySpeed(1.0f);

                m_tFrame.iMotion == 1 ? m_eCurMotion = LEFT_DEATH_GROUND : m_eCurMotion = RIGHT_DEATH_GROUND;
                CSoundMgr::Get_Instance()->PlaySound(L"boss_death_ground.wav", SOUND_EFFECT, g_fVolume);
                Motion_Change();
            }
            else
            {
                m_bDead = false;
                m_ePattern = TELEPORT;
                m_tFrame.iMotion == 1 ? m_eCurMotion = LEFT_TELEPORT : m_eCurMotion = RIGHT_TELEPORT;
                CSoundMgr::Get_Instance()->PlaySound(L"boss_teleport.wav", SOUND_EFFECT, g_fVolume);
                Update_Pattern();
                Motion_Change();
            }
        }

        else if (m_ePattern == BULLET_SWEEP)
        {
            m_ePattern = NONE;
            m_eCurMotion = IDLE;
            m_fAttackDelayTime = GetTickCount64();
            m_fBulletAngle = 0.f;
            m_bBulletSweep = false;
        }
    }

    if (HitActor->GetObjType() == TYPE_WALL || HitActor->GetObjType() == TYPE_MONSTER_WALL) // 렉트 벽 충돌
    {
        if (_HitInfo.eDirection == DIRECTION::DIR_DOWN)
        {
            m_fTime = 3.9f;
        }

        if (_HitInfo.eDirection == DIRECTION::DIR_UP)
        {
            m_tInfo.fY += fabsf(_HitInfo.fY);
        }

        if (_HitInfo.eDirection == DIRECTION::DIR_LEFT)
        {
            m_tInfo.fX += (fabsf(_HitInfo.fX) + 3.f);
            //m_eCurMotion = LEFT_WALLGRAP;
            if (m_iDeathCount >= 0 && m_eCurMotion != LEFT_DEATH_JUMP && m_eCurMotion != RIGHT_DEATH_JUMP)
            {
                m_ePattern = BULLET_SWEEP;
                m_eCurMotion = LEFT_WALLGRAP;
            }
        }

        if (_HitInfo.eDirection == DIRECTION::DIR_RIGHT)
        {
            m_tInfo.fX -= (fabsf(_HitInfo.fX) + 3.f);

            if (m_iDeathCount >= 0 && m_eCurMotion != LEFT_DEATH_JUMP && m_eCurMotion != RIGHT_DEATH_JUMP)
            {
                m_ePattern = BULLET_SWEEP;
                m_eCurMotion = RIGHT_WALLGRAP;
            }
        }
    }
}

void CHeadHunter_2::HitActor(CObj* HitActor)
{
    if (HitActor->Get_Type() == OBJ && dynamic_cast<CHitBox*>(HitActor)->Get_Hit() == true) return;

    if(m_bDead == true && m_bHeadCut == false && (m_eCurMotion == LEFT_DEATH_GROUND ||
        m_eCurMotion == RIGHT_DEATH_GROUND) && HitActor->Get_Type() == OBJ && 
        dynamic_cast<CHitBox*>(HitActor)->Get_Hit() == false)
    {

        m_bHeadCut = true;

        //여기서 게임 클리어
        g_bGameClear = true;
        //모션도 머리 잘린 상태로 바꿔줘야 한다.
        m_eCurMotion = m_eCurMotion == LEFT_DEATH_GROUND ? LEFT_NO_HEAD : RIGHT_NO_HEAD;
        Motion_Change();
        //머리 추가
        CObj* pHead = CAbstractFactory<CHead>::Create_Obj(m_tInfo.fX, m_tInfo.fY);
        pHead->Set_Angle(m_fAttackAngle);
        CObjMgr::Get_Instance()->Add_Object(OBJ_HEAD, pHead);
        
        //뎅겅~
        CSoundMgr::Get_Instance()->PlaySound(L"monster_death_sword.wav", SOUND_EFFECT, g_fVolume);

        CEffect* pEffect;
        //핏자국 이펙트
        pEffect = new CStainEffect(m_tInfo.fX, m_tInfo.fY, L"Blood_Stain", 180 + m_fAttackAngle);
        pEffect->Initialize();
        CEffectMgr::Get_Instance()->Add_Effect(pEffect);

        pEffect = new CSlashEffect(m_tInfo.fX, m_tInfo.fY, L"Player_Enemy_Slash", dynamic_cast<CHitBox*>(HitActor)->Get_TargetAngle());
        pEffect->Initialize();
        CEffectMgr::Get_Instance()->Add_Front_Effect(pEffect);
        CScrollMgr::Get_Instance()->Shake(10, 200);

    }

    if ((HitActor->Get_Type() == BULLET && dynamic_cast<CBullet*>(HitActor)->Get_Parried() == false) 
        || m_bDead == true || HitActor->Get_Dead() == true)
        return;

    if (HitActor->Get_Type() == OBJ && dynamic_cast<CHitBox*>(HitActor)->Get_Hit() == true)
        return;
    else if(HitActor->Get_Type() == OBJ && dynamic_cast<CHitBox*>(HitActor)->Get_Hit() == false)
        dynamic_cast<CHitBox*>(HitActor)->Set_Hit(true);

    if (m_eCurMotion != LEFT_ROLL && m_eCurMotion != RIGHT_ROLL &&
        m_bDead == false)
    {
        if (m_tFrame.iMotion == 1)
            m_eCurMotion = LEFT_DEATH_JUMP;

        else
            m_eCurMotion = RIGHT_DEATH_JUMP;

        if (m_pFrameKey != L"HeadHunter_Death_Jump" && m_pFrameKey != L"HeadHunter_Death_Ground")
            m_bJump = true;

        if (HitActor->Get_Type() == OBJ)
        {
            m_fAttackAngle = dynamic_cast<CHitBox*>(HitActor)->Get_TargetAngle();
            m_iDeathCount--;
            m_ePattern = DEATH;
            m_bDead = true;
            if (m_iDeathCount <= 0)
            {
                CSoundMgr::Get_Instance()->StopAll();
                g_bSlowMod = true;
                g_fSlowFactor = 0.3f;
                CSoundMgr::Get_Instance()->Set_PlaySpeed(0.9f);
                CSoundMgr::Get_Instance()->PlaySound(L"boss_death_ground.wav", SOUND_EFFECT, g_fVolume);
                g_bMonoChrome = true;
                
            }
            CSoundMgr::Get_Instance()->PlaySound(L"boss_hurt.wav", SOUND_EFFECT, g_fVolume);
            Motion_Change();
        }

        else if (HitActor->Get_Type() == BULLET && dynamic_cast<CBullet*>(HitActor)->Get_Parried() == true)
        {
            m_fAttackAngle = 135.f;
            m_iDeathCount--;
            m_ePattern = DEATH;
            m_bDead = true;
            if (m_iDeathCount <= 0)
            {
                g_bSlowMod = true;
                g_fSlowFactor = 0.3f;
                g_bMonoChrome = true;
            }
            CSoundMgr::Get_Instance()->PlaySound(L"boss_hurt.wav", SOUND_EFFECT, g_fVolume);
            Motion_Change();
        }
        /*
            m_ePattern = DEATH;
            m_bDead = true;
            Motion_Change();

            -> 실제 사망 처리를 위한 코드
        */
        Motion_Change();
    }
}

void CHeadHunter_2::Ground_Laser()
{
    if (m_ePattern == GROUND_LASER)
    {
        //총 뽑고
        if (m_pFrameKey == L"HeadHunter_Draw_Gun" && isAnimationEnd())
        {
            if (m_pTarget->Get_Info()->fX < m_tInfo.fX)
                m_eCurMotion = LEFT_AIM;
            else
                m_eCurMotion = RIGHT_AIM;

            m_fLaserDelayTime = GetTickCount64();
            CSoundMgr::Get_Instance()->PlaySound(L"boss_aim_ground.wav", SOUND_EFFECT, g_fVolume);
        }
        else if (m_pFrameKey == L"HeadHunter_Draw_Gun" && !isAnimationEnd())
            return;
        // 조준하는 모션
        else if (m_pFrameKey == L"HeadHunter_Aim" && !isAnimationEnd() && (GetTickCount64() - m_fLaserDelayTime <= 800 / g_fSlowFactor))
        {
            if (m_pTarget->Get_Info()->fX < m_tInfo.fX)
            {
                m_eCurMotion = LEFT_AIM;
                m_tFrame.iMotion = 1;
            }
            else
            {
                m_eCurMotion = RIGHT_AIM;
                m_tFrame.iMotion = 0;
            }

            if (m_fTargetAngle >= 90.0f && m_fTargetAngle <= 270.0f)
            {
                // 90 ~ 270 → 1 ~ 17 선형 증가
                m_tFrame.iStart = static_cast<int>(1 + ((m_fTargetAngle - 90.0f) / 180.0f) * 16.0f);
            }
            else
            {
                if (m_fTargetAngle < 90.0f)
                    m_tFrame.iStart = static_cast<int>((m_fTargetAngle / 10.0f) + 9.0f);

                else
                    m_tFrame.iStart = static_cast<int>((m_fTargetAngle - 270.0f) / 10.0f);
            }

        }
        //레이저 히트박스 생성 후 잠깐의 대기 모션이 끝나면 발사. 레이저 히트박스는 line으로 처리하는게 나아보이긴 한다.
        else if (m_pFrameKey == L"HeadHunter_Aim" && (GetTickCount64() - m_fLaserDelayTime > 500 / g_fSlowFactor) && !m_bLaserShoot)
        {
            CSoundMgr::Get_Instance()->PlaySound(L"boss_laser_ground.wav",SOUND_EFFECT,g_fVolume);
            CLineMgr::Get_Instance()->Add_Line(new CLaser(
                { m_tInfo.fX + 30 * cosf(m_fTargetAngle * PI / 180.f), m_tInfo.fY - 30 * sinf(m_fTargetAngle * PI / 180.f) },
                { m_tInfo.fX + 1500 * cosf(m_fTargetAngle * PI / 180.f), m_tInfo.fY - 1500 * sinf(m_fTargetAngle * PI / 180.f) },
                TYPE_LASER));

            m_bLaserShoot = true;
        }
        else if (GetTickCount64() - m_fLaserDelayTime > 100 / g_fSlowFactor)
        {
            m_ePattern = NONE;
            m_eCurMotion = IDLE;
            m_fAttackDelayTime = GetTickCount64();
            m_bLaserShoot = false;
        }
    }
}

void CHeadHunter_2::Dodge()
{
    if (m_ePattern == DODGE)
    {
        if (m_eCurMotion == LEFT_ROLL)
        {
            if (m_tFrame.iStart == 0) // 프레임 재생 끝났으면,
            {
                m_eCurMotion = IDLE;
                m_ePattern = NONE; // IDLE, NONE
                //m_fAttackDelayTime = GetTickCount64() ;
            }

            m_tInfo.fX -= m_fSpeed * g_fSlowFactor;
        }
        else if (m_eCurMotion == RIGHT_ROLL)
        {
            if (m_tFrame.iStart == m_tFrame.iEnd) // 프레임 재생 끝났으면,
            {
                m_eCurMotion = IDLE;
                m_ePattern = NONE; // IDLE, NONE
                //m_fAttackDelayTime = GetTickCount64();
            }

            m_tInfo.fX += m_fSpeed * g_fSlowFactor;
        }

    }
}

void CHeadHunter_2::Rush()
{
    if (m_ePattern == RUSH)
    {
        //칼 뽑아드는 준비모션
        if (m_pFrameKey == L"HeadHunter_Pre_Rush" && isAnimationEnd())
        {
            if (m_eCurMotion == LEFT_PRE_RUSH)
                m_eCurMotion = LEFT_RUSH;
            else
                m_eCurMotion = RIGHT_RUSH;

            CSoundMgr::Get_Instance()->PlaySound(L"boss_dash.wav", SOUND_EFFECT, g_fVolume);
            m_fRushAngle = m_fTargetAngle;
        }
        else if (m_pFrameKey == L"HeadHunter_Pre_Rush" && !isAnimationEnd())
            return;

        else if (m_pFrameKey == L"HeadHunter_Rush")
        {
            m_fRushAngle = (m_eCurMotion == LEFT_RUSH) ? 155.f : 25.f;
            float radian = m_fRushAngle * PI / 180.f;

            float speed = m_fSpeed * g_fSlowFactor;

            m_tInfo.fX += (m_eCurMotion == LEFT_RUSH ? -1 : 1) * speed * 2.5;
            m_tInfo.fY -= speed * 1.5f * sinf(radian);
        }
    }

}

void CHeadHunter_2::Bullet_Sweep()
{
    if (m_ePattern == BULLET_SWEEP)
    {

        if (m_tInfo.fY >= 380)
        {
            if (m_eCurMotion == LEFT_WALLGRAP)
            {
                m_eCurMotion = LEFT_BULLET_SWEEP;
                m_fBulletAngle = 360.f;
                m_fBulletTimer = GetTickCount64();

                CSoundMgr::Get_Instance()->PlaySound(L"boss_bullet_sweep_1.wav",SOUND_EFFECT, g_fVolume);
                CSoundMgr::Get_Instance()->PlaySound(L"boss_bullet_sweep_2.wav", SOUND_EFFECT, g_fVolume);
            }

            else if (m_eCurMotion == RIGHT_WALLGRAP)
            {
                m_eCurMotion = RIGHT_BULLET_SWEEP;
                m_fBulletAngle = 180.f;
                m_fBulletTimer = GetTickCount64();

                CSoundMgr::Get_Instance()->PlaySound(L"boss_bullet_sweep_1.wav", SOUND_EFFECT, g_fVolume);
                CSoundMgr::Get_Instance()->PlaySound(L"boss_bullet_sweep_2.wav", SOUND_EFFECT, g_fVolume);
            }
        }

        if (m_eCurMotion == LEFT_BULLET_SWEEP)
        {
            if(GetTickCount64() - m_fBulletTimer >= 25 / g_fSlowFactor && m_fBulletAngle >= 225)
            {
                CObj* pBullet = new CBullet(m_tInfo.fX, m_tInfo.fY, m_fBulletAngle);
                pBullet->Initialize();
                CObjMgr::Get_Instance()->Add_Object(OBJ_BULLET, pBullet);
                m_fBulletAngle -= 8.f;
                m_fBulletTimer = GetTickCount64();
            }

            m_tInfo.fX += m_fSpeed * g_fSlowFactor;

            if (m_tInfo.fY <= 200)
                m_bIsHitCeil = true;

            if (!m_bIsHitCeil)
                m_tInfo.fY -= m_fSpeed * g_fSlowFactor;
            else
                m_tInfo.fY += m_fSpeed * g_fSlowFactor;

        }
        else if (m_eCurMotion == RIGHT_BULLET_SWEEP)
        {
            if (GetTickCount64() - m_fBulletTimer >= 25 / g_fSlowFactor && m_fBulletAngle <= 315)
            {
                CObj* pBullet = new CBullet(m_tInfo.fX, m_tInfo.fY, m_fBulletAngle);
                pBullet->Initialize();
                CObjMgr::Get_Instance()->Add_Object(OBJ_BULLET, pBullet);
                m_fBulletAngle += 8.f;
                m_fBulletTimer = GetTickCount64();
            }

            m_tInfo.fX -= m_fSpeed * g_fSlowFactor;

            if (m_tInfo.fY <= 200)
                m_bIsHitCeil = true;

            if (!m_bIsHitCeil)
                m_tInfo.fY -= m_fSpeed * g_fSlowFactor;
            else
                m_tInfo.fY += m_fSpeed * g_fSlowFactor;
        }
    }
}

void CHeadHunter_2::Aerial_Bullet()
{
    if (m_ePattern == AERIAL_BULLET)
    {
        //공중 텔포
        if ((m_eCurMotion != AERIAL_ATTACK_IN && m_eCurMotion != AERIAL_ATTACK_OUT) && (m_eCurMotion != LEFT_TELEPORT && m_eCurMotion != RIGHT_TELEPORT))
        {
            if (m_eAerialDir == RIGHT)
                m_iAerialCount = 0;
            else
                m_iAerialCount = 2;

            m_eCurMotion = m_tFrame.iMotion == 1 ? LEFT_TELEPORT : RIGHT_TELEPORT;
            CSoundMgr::Get_Instance()->PlaySound(L"boss_teleport.wav", SOUND_EFFECT, g_fVolume);
            Update_Frame();
        }
        else if ((m_eCurMotion == LEFT_TELEPORT || m_eCurMotion == RIGHT_TELEPORT) && isAnimationEnd())
        {
            m_tInfo.fX = m_tAerialCheckPoint[m_iAerialCount].x;
            m_tInfo.fY = m_tAerialCheckPoint[m_iAerialCount].y;
            CSoundMgr::Get_Instance()->PlaySound(L"boss_teleport.wav", SOUND_EFFECT, g_fVolume);
            m_eCurMotion = AERIAL_ATTACK_IN;
            Update_Frame();
            m_fFloatTimer = GetTickCount64();
        }
        else if (m_eCurMotion == AERIAL_ATTACK_IN && isAnimationEnd() && (GetTickCount64()  - m_fFloatTimer >= 300 / g_fSlowFactor))
        {
            CObj* pBullet = new CBullet(m_tInfo.fX, m_tInfo.fY, m_fTargetAngle);
            pBullet->Initialize();
            CObjMgr::Get_Instance()->Add_Object(OBJ_BULLET, pBullet);
            CSoundMgr::Get_Instance()->PlaySound(L"gun_fire_1.wav", SOUND_EFFECT, g_fVolume);
            m_eCurMotion = AERIAL_ATTACK_OUT;
        }
        else if (m_eCurMotion == AERIAL_ATTACK_OUT && isAnimationEnd())
        {
            if ((m_eAerialDir == RIGHT && m_iAerialCount < 2) || (m_eAerialDir == LEFT && m_iAerialCount > 0))
            {
                if (m_eAerialDir == RIGHT)
                    m_iAerialCount++;
                else
                    m_iAerialCount--;

                m_tInfo.fX = m_tAerialCheckPoint[m_iAerialCount].x;
                m_tInfo.fY = m_tAerialCheckPoint[m_iAerialCount].y;
                CSoundMgr::Get_Instance()->PlaySound(L"boss_teleport.wav", SOUND_EFFECT, g_fVolume);
                m_eCurMotion = AERIAL_ATTACK_IN;
                Update_Frame();
            }
            else
            {
                if (m_eAerialDir == RIGHT)
                {
                    m_tInfo.fX = m_tGroundCheckPoint[1].x;
                    m_tInfo.fY = m_tGroundCheckPoint[1].y;
                    CSoundMgr::Get_Instance()->PlaySound(L"boss_teleport.wav", SOUND_EFFECT, g_fVolume);
                }
                else
                {
                    m_tInfo.fX = m_tGroundCheckPoint[0].x;
                    m_tInfo.fY = m_tGroundCheckPoint[0].y;
                    CSoundMgr::Get_Instance()->PlaySound(L"boss_teleport.wav", SOUND_EFFECT, g_fVolume);
                }
                m_iAerialCount = 0;
                m_ePattern = NONE;
                m_eCurMotion = IDLE;
                Update_Frame();
                m_fAttackDelayTime = GetTickCount64();
            }
        }
    }
}

void CHeadHunter_2::Aerial_Laser()
{
    if (m_ePattern == AERIAL_LASER)
    {
        //공중 텔포
        if ((m_eCurMotion != AERIAL_ATTACK_IN && m_eCurMotion != AERIAL_ATTACK_OUT) && (m_eCurMotion != LEFT_TELEPORT && m_eCurMotion != RIGHT_TELEPORT))
        {
            if (m_eAerialDir == RIGHT)
                m_iAerialCount = 0;
            else
                m_iAerialCount = 2;

            m_eCurMotion = m_tFrame.iMotion == 1 ? LEFT_TELEPORT : RIGHT_TELEPORT;
            CSoundMgr::Get_Instance()->PlaySound(L"boss_teleport.wav", SOUND_EFFECT, g_fVolume);
            Update_Frame();
        }
        else if ((m_eCurMotion == LEFT_TELEPORT || m_eCurMotion == RIGHT_TELEPORT) && isAnimationEnd()) 
        {
            m_tInfo.fX = m_tAerialCheckPoint[m_iAerialCount].x;
            m_tInfo.fY = m_tAerialCheckPoint[m_iAerialCount].y;
            CSoundMgr::Get_Instance()->PlaySound(L"boss_teleport.wav", SOUND_EFFECT, g_fVolume);
            m_eCurMotion = AERIAL_ATTACK_IN;
            Update_Frame();
            m_fFloatTimer = GetTickCount64();
        }
        else if (m_eCurMotion == AERIAL_ATTACK_IN && isAnimationEnd() && (GetTickCount64() - m_fFloatTimer >= 300 / g_fSlowFactor))
        {
            m_eCurMotion = AERIAL_ATTACK_OUT;
            CSoundMgr::Get_Instance()->PlaySound(L"boss_laser_ground.wav", SOUND_EFFECT, g_fVolume);
            CLineMgr::Get_Instance()->Add_Line(new CLaser(
                { m_tInfo.fX , m_tInfo.fY },
                { m_tInfo.fX , m_tInfo.fY + 900 },
                TYPE_LASER));
        }
        else if (m_eCurMotion == AERIAL_ATTACK_OUT && isAnimationEnd())
        {
            if ((m_eAerialDir == RIGHT && m_iAerialCount < 2) || (m_eAerialDir == LEFT && m_iAerialCount > 0))
            {
                if (m_eAerialDir == RIGHT)
                    m_iAerialCount++;
                else
                    m_iAerialCount--;

                m_tInfo.fX = m_tAerialCheckPoint[m_iAerialCount].x;
                m_tInfo.fY = m_tAerialCheckPoint[m_iAerialCount].y;
                CSoundMgr::Get_Instance()->PlaySound(L"boss_teleport.wav", SOUND_EFFECT, g_fVolume);
                m_eCurMotion = AERIAL_ATTACK_IN;
                Update_Frame();
            }
            else
            {
                if (m_eAerialDir == RIGHT)
                {
                    m_tInfo.fX = m_tGroundCheckPoint[1].x;
                    m_tInfo.fY = m_tGroundCheckPoint[1].y;
                }
                else
                {
                    m_tInfo.fX = m_tGroundCheckPoint[0].x;
                    m_tInfo.fY = m_tGroundCheckPoint[0].y;
                }
                m_iAerialCount = 0;
                m_ePattern = NONE;
                m_eCurMotion = IDLE;
                Update_Frame();
                m_fAttackDelayTime = GetTickCount64();
            }
        }
    }
}

void CHeadHunter_2::Aerial_Sweep()
{
    if (m_ePattern == AERIAL_SWEEP)
    {
        //공중 텔포
        if ((m_eCurMotion != LEFT_LASER_SWEEP && m_eCurMotion != RIGHT_LASER_SWEEP) && (m_eCurMotion != LEFT_TELEPORT && m_eCurMotion != RIGHT_TELEPORT))
        {
            m_eCurMotion = m_tFrame.iMotion == 1 ? LEFT_TELEPORT : RIGHT_TELEPORT;
            CSoundMgr::Get_Instance()->PlaySound(L"boss_teleport.wav", SOUND_EFFECT, g_fVolume);
            Update_Frame();
        }
        else if ((m_eCurMotion == LEFT_TELEPORT || m_eCurMotion == RIGHT_TELEPORT) && isAnimationEnd())
        {
            int iRand = rand() % 2;
            m_tInfo.fX = m_tAerialSweepCheckPoint[iRand].x;
            m_tInfo.fY = m_tAerialSweepCheckPoint[iRand].y;
            CSoundMgr::Get_Instance()->PlaySound(L"boss_teleport.wav", SOUND_EFFECT, g_fVolume);
            CSoundMgr::Get_Instance()->PlaySound(L"boss_aerial_sweep.wav", SOUND_EFFECT, g_fVolume);
            m_eCurMotion = iRand == 1 ? RIGHT_LASER_SWEEP : LEFT_LASER_SWEEP;
            CLineMgr::Get_Instance()->Add_Line(new CMovingLaser(this, m_eCurMotion == LEFT_LASER_SWEEP ? CMovingLaser::LEFT : CMovingLaser::RIGHT, TYPE_LASER));
        }
        else if ((m_eCurMotion == LEFT_LASER_SWEEP || m_eCurMotion == RIGHT_LASER_SWEEP) && isAnimationEnd())
        {
            m_iAerialCount = 0;
            m_ePattern = NONE;
            m_eCurMotion = IDLE;
            Update_Frame();
            int iRand = rand() % 2;
            m_tInfo.fX = m_tGroundCheckPoint[iRand].x;
            m_tInfo.fY = m_tGroundCheckPoint[iRand].y;
            m_fAttackDelayTime = GetTickCount64();
        }
    }
}

void CHeadHunter_2::Teleport()
{
    if (m_ePattern == TELEPORT)
    {
        if (isAnimationEnd())
        {
            int iNum = rand() % 2;
            m_tInfo.fX = m_tGroundCheckPoint[iNum].x;
            m_tInfo.fY = m_tGroundCheckPoint[iNum].y;

            Calc_Target();
            if (m_fTargetDist >= 450)
            {
                m_ePattern = GROUND_LASER;
                if (m_pTarget->Get_Info()->fX < m_tInfo.fX)
                    m_eCurMotion = LEFT_DRAW_GUN;
                else
                    m_eCurMotion = RIGHT_DRAW_GUN;
            }
            else
            {
                m_ePattern = RUSH;
                m_eCurMotion = m_tInfo.fX <= 672 ? RIGHT_PRE_RUSH : LEFT_PRE_RUSH;

            }
            m_fAttackDelayTime = GetTickCount64();
            Update_Pattern();
        }
    }
}

void CHeadHunter_2::Death_Bound()
{
    if (m_bDead && m_bJump)
    {
        m_fTime += 0.15f * g_fSlowFactor;
        m_tInfo.fX += m_fSpeed * cosf(m_fAttackAngle * PI / 180.f) * g_fSlowFactor;

        if (m_fSpeed * g_fSlowFactor * m_fTime - 0.5 * 3.9 * m_fTime * m_fTime * g_fSlowFactor >= -6.5f * g_fSlowFactor)
            m_tInfo.fY -= m_fSpeed * g_fSlowFactor * m_fTime - 0.5 * 3.9 * m_fTime * m_fTime * g_fSlowFactor;
        else
            m_tInfo.fY -= -6.5f * g_fSlowFactor;
    }
}

