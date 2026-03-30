#include "pch.h"
#include "CButton.h"
#include "CBmpMgr.h"
#include "CKeyMgr.h"
#include "CSceneMgr.h"

CButton::CButton()
{
}

CButton::~CButton()
{
}

void CButton::Initialize()
{
    m_tInfo.fCX = 150.f;
    m_tInfo.fCY = 25.f;

    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Ui/new_game_UI.bmp", L"New_Game"); 
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Ui/exit_UI.bmp", L"Exit");
}

int CButton::Update()
{
    __super::Update_Rect();
        
    return NOEVENT;
}

void CButton::Late_Update()
{
    POINT ptMouse{};
    GetCursorPos(&ptMouse);
    ScreenToClient(g_hWnd, &ptMouse);

    if (PtInRect(&m_tRect, ptMouse))
    {
        if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
        {
            if (!lstrcmp(L"New_Game", m_pFrameKey))
                g_bStageClear = true;
            else if (!lstrcmp(L"Exit", m_pFrameKey))
                DestroyWindow(g_hWnd);

            return;
        }
        m_iDrawId = 1;
    }
    else
        m_iDrawId = 0;
}

void CButton::Render(HDC hDC)
{
    /*
    반투명 검은색으로 버튼 표시.
    */
    HDC hMemDC = CreateCompatibleDC(hDC);
    RECT rect{ m_tInfo.fX - 300, m_tInfo.fY - 20, m_tInfo.fX + 300, m_tInfo.fY + 20  };
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    HBITMAP hBitmap = CreateCompatibleBitmap(hDC, width, height);
    SelectObject(hMemDC, hBitmap);

    // 반투명 검정으로 채움
    HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
    RECT localRect{ 0, 0, width, height }; // DC 기준 좌표
    FillRect(hMemDC, &localRect, hBrush);

    // Alpha blending
    BLENDFUNCTION bf = { AC_SRC_OVER, 0, 128, 0 }; // 50% 투명
    AlphaBlend(hDC, rect.left, rect.top, width, height,
        hMemDC, 0, 0, width, height, bf);

    // 정리
    DeleteObject(hBrush);
    DeleteObject(hBitmap);
    DeleteDC(hMemDC);
    ReleaseDC(g_hWnd, hDC);

    if (m_iDrawId == 1)
    {
        /*호버 표시*/
        HDC hMemDC = CreateCompatibleDC(hDC);
        RECT rect{ m_tInfo.fX - 275, m_tInfo.fY - 15, m_tInfo.fX + 275, m_tInfo.fY + 15 };
        int width = rect.right - rect.left;
        int height = rect.bottom - rect.top;

        HBITMAP hBitmap = CreateCompatibleBitmap(hDC, width, height);
        SelectObject(hMemDC, hBitmap);

        // 반투명 검정으로 채움
        HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
        RECT localRect{ 0, 0, width, height }; // DC 기준 좌표
        FillRect(hMemDC, &localRect, hBrush);

        // Alpha blending
        BLENDFUNCTION bf = { AC_SRC_OVER, 0, 64, 0 }; // 50% 투명
        AlphaBlend(hDC, rect.left, rect.top, width, height,
            hMemDC, 0, 0, width, height, bf);

        // 정리
        DeleteObject(hBrush);
        DeleteObject(hBitmap);
        DeleteDC(hMemDC);
        ReleaseDC(g_hWnd, hDC);
    }

    hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
    GdiTransparentBlt(hDC,/// 복사 받을 dc
        m_tRect.left,		// 복사 받을 위치 좌표 left
        m_tRect.top,					// 복사 받을 위치 좌표 top
        (int)m_tInfo.fCX,				// 복사 받을 가로 사이즈
        (int)m_tInfo.fCY,				// 복사 받을 세로 사이즈
        hMemDC,							// 복사할 이미지 dc
        0,0,// 복사할 이미지의 left, top
        (int)m_tInfo.fCX,				// 복사할 이미지의 가로
        (int)m_tInfo.fCY,				// 복사할 이미지의 세로
        RGB(255, 0, 255));			// 제거할 이미지 색상 값
}

void CButton::Release()
{
}
