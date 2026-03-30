#include "pch.h"
#include "CMenu.h"
#include "CObjMgr.h"
#include "CAbstractFactory.h"
#include "CBmpMgr.h"
#include "CSoundMgr.h"

CMenu::CMenu()
{
}

CMenu::~CMenu()
{
	Release();
}

void CMenu::Initialize()
{
	CSoundMgr::Get_Instance()->PlayBGM(L"stage_menu.ogg", g_fVolume);
	//....?..
	CObj* pButton = CAbstractFactory<CButton>::Create_Obj(400, 425);
	pButton->Set_FrameKey(L"New_Game");
	CObjMgr::Get_Instance()->Add_Object(OBJ_BUTTON, pButton);

	pButton = CAbstractFactory<CButton>::Create_Obj(400, 470);
	pButton->Set_FrameKey(L"Exit");
	CObjMgr::Get_Instance()->Add_Object(OBJ_BUTTON, pButton);

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Menu/Menu.bmp", L"Menu");
}

void CMenu::Update()
{
	CObjMgr::Get_Instance()->Update();
}

void CMenu::Late_Update()
{
	CObjMgr::Get_Instance()->Late_Update();
}

void CMenu::Render(HDC hDC)
{
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Menu");

	BitBlt(hDC,
		0,
		0,
		WINCX, WINCY,
		hMemDC,
		0, 0,
		SRCCOPY);

	CObjMgr::Get_Instance()->Render(hDC);
}

void CMenu::Release()
{
	CObjMgr::Get_Instance()->Delete_Object(OBJ_BUTTON);
}
