#include "pch.h"
#include "CEndScene.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"
#include "CEffectMgr.h"

CEndScene::CEndScene()
{
}

CEndScene::~CEndScene()
{
}

void CEndScene::Initialize()
{
    CScrollMgr::Get_Instance()->Set_ScrollPos(0, 0);
    //여기서 bmpmgr로 엔딩씬 이미지들 다 가져와야 한다.
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/end.bmp", L"Ending");

    CEffectMgr::Get_Instance()->Initialize();

    CEffect* pEffect = new CRemainEffect(400, 500, L"Dancing");
    pEffect->Initialize();
    pEffect->Set_Infinity(true);

    CEffectMgr::Get_Instance()->Add_Front_Effect(pEffect);

    m_fFirstImageTimer = GetTickCount64();
    Release();
}

void CEndScene::Update()
{
}

void CEndScene::Late_Update()
{
}

void CEndScene::Render(HDC hDC)
{
    HDC hGroundDC = CBmpMgr::Get_Instance()->Find_Image(L"Ending");

    BitBlt(hDC,
        0,
        0,
        800,600,
        hGroundDC,
        0, 0,
        SRCCOPY);

    CEffectMgr::Get_Instance()->Render_Front(hDC);

}

void CEndScene::Release()
{
}
