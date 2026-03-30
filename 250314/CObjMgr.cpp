#include "pch.h"
#include "CObjMgr.h"
#include "CCollisionMgr.h"
#include "CLineMgr.h"
#include "CRectMgr.h"
#include "CScrollMgr.h"

CObjMgr* CObjMgr::m_pInstance = nullptr;

CObjMgr::CObjMgr()
{
}

CObjMgr::~CObjMgr()
{
	Release();
}

void CObjMgr::Add_Object(OBJID eID, CObj* pObj)
{
	if ((OBJ_END <= eID) || (nullptr == pObj))
		return;

	m_ObjList[eID].push_back(pObj);
}

void CObjMgr::Update()
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto iter = m_ObjList[i].begin();
			iter != m_ObjList[i].end(); )
		{
			if (g_bGameOver && i != OBJ_MOUSE)
			{
				++iter;
				continue;
			}

			int iResult = (*iter)->Update();

			if (DEAD == iResult)
			{
				Safe_Delete(*iter);
				iter = m_ObjList[i].erase(iter);
			}
			else
			{
				++iter;
			}
		}
	}
}

void CObjMgr::Late_Update()
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto iter = m_ObjList[i].begin();
			iter != m_ObjList[i].end(); ++iter)
		{
			if (g_bGameOver == false)
				(*iter)->Late_Update();

			else if(g_bGameOver == true && i == OBJ_MOUSE)
				(*iter)->Late_Update();

			if (m_ObjList[i].empty())
				break;
		}
	}

	CCollisionMgr::Collision_StaticRect(m_ObjList[OBJ_PLAYER], CRectMgr::Get_Instance()->Get_RectList());
	CCollisionMgr::Collision_StaticLine(m_ObjList[OBJ_PLAYER], CLineMgr::Get_Instance()->Get_LineList());
	CCollisionMgr::Collision_StaticRect(m_ObjList[OBJ_MONSTER], CRectMgr::Get_Instance()->Get_RectList());
	CCollisionMgr::Collision_StaticLine(m_ObjList[OBJ_MONSTER], CLineMgr::Get_Instance()->Get_LineList());
	CCollisionMgr::Collision_StaticRect(m_ObjList[OBJ_BULLET], CRectMgr::Get_Instance()->Get_RectList());
	CCollisionMgr::Collision_StaticLine(m_ObjList[OBJ_BULLET], CLineMgr::Get_Instance()->Get_LineList());
	CCollisionMgr::Collision_StaticRect(m_ObjList[OBJ_ITEM], CRectMgr::Get_Instance()->Get_RectList());
	CCollisionMgr::Collision_StaticLine(m_ObjList[OBJ_ITEM], CLineMgr::Get_Instance()->Get_LineList());
	CCollisionMgr::Collision_StaticRect(m_ObjList[OBJ_HEAD], CRectMgr::Get_Instance()->Get_RectList());
	CCollisionMgr::Collision_StaticLine(m_ObjList[OBJ_HEAD], CLineMgr::Get_Instance()->Get_LineList());
	//이거 그냥 진짜 Rect 충돌로 바꿔야 할듯?
	CCollisionMgr::Collision_Rect(m_ObjList[OBJ_MONSTER], m_ObjList[OBJ_PLAYER_HITBOX]);
	CCollisionMgr::Collision_Rect(m_ObjList[OBJ_PLAYER], m_ObjList[OBJ_MONSTER_HITBOX]);
	CCollisionMgr::Collision_Rect(m_ObjList[OBJ_BULLET], m_ObjList[OBJ_PLAYER_HITBOX]);

	CCollisionMgr::Collision_Rect(m_ObjList[OBJ_PLAYER], m_ObjList[OBJ_FANBLADE]);
	CCollisionMgr::Collision_Rect(m_ObjList[OBJ_PLAYER], m_ObjList[OBJ_ITEM]);
	CCollisionMgr::Collision_Rect(m_ObjList[OBJ_ITEM], m_ObjList[OBJ_BULLET]);
	CCollisionMgr::Collision_Rect(m_ObjList[OBJ_MONSTER], m_ObjList[OBJ_ITEM]);

	CCollisionMgr::Collision_Rect(m_ObjList[OBJ_PLAYER], m_ObjList[OBJ_BULLET]);
	//몬스터가 죽는 처리가 되면서 총알이 쭉 나아감..
	CCollisionMgr::Collision_Rect(m_ObjList[OBJ_MONSTER], m_ObjList[OBJ_BULLET]);
}

void CObjMgr::Render(HDC hDC)
{
	MonoChrome_Filter(hDC);
	SlowMod_Filter(hDC);

	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto iter = m_ObjList[i].begin();
			iter != m_ObjList[i].end(); ++iter)
		{
			(*iter)->Render(hDC);
		}
	}
}

void CObjMgr::Release()
{

	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for_each(m_ObjList[i].begin(), m_ObjList[i].end(), Safe_Delete<CObj*>);
		m_ObjList[i].clear();
	}
}

void CObjMgr::Delete_Object(OBJID eID)
{
	for (auto& pObj : m_ObjList[eID])
	{
		Safe_Delete<CObj*>(pObj);
	}
	m_ObjList[eID].clear();
}

void CObjMgr::Delete_Object(OBJID eID, CObj* _pTarget)
{
	for (auto it = m_ObjList[eID].begin(); it != m_ObjList[eID].end();)
	{
		if (*it == _pTarget)
			it = m_ObjList[eID].erase(it); // 삭제 없이 리스트에서만 제거
		else
			++it;
	}
}

void CObjMgr::SlowMod_Filter(HDC hDC)
{
	if ((g_bSlowMod && !g_bMonoChrome) || g_bDark)
	{
		HDC hMemDC = CreateCompatibleDC(hDC);
		RECT rect;
		GetClientRect(g_hWnd, &rect);

		HBITMAP hBitmap = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);
		SelectObject(hMemDC, hBitmap);

		// 반투명 빨강으로 채움
		HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
		FillRect(hMemDC, &rect, hBrush);

		// Alpha blending
		BLENDFUNCTION bf = { AC_SRC_OVER, 0, 128, 0 }; // 128 = 50% 투명도
		AlphaBlend(hDC, 0, 0, rect.right, rect.bottom, hMemDC, 0, 0, rect.right, rect.bottom, bf);

		// 정리
		DeleteObject(hBrush);
		DeleteObject(hBitmap);
		DeleteDC(hMemDC);
		ReleaseDC(g_hWnd, hDC);
	}
}

void CObjMgr::MonoChrome_Filter(HDC hDC)
{
	if (g_bMonoChrome)
	{
		int iScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
		int iScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

		HDC hMemDC = CreateCompatibleDC(hDC);
		RECT rect, whiteRect = {143 + iScrollX, 0 + iScrollY, 1201 + iScrollX, 578 + iScrollY};
		GetClientRect(g_hWnd, &rect);
		HBITMAP hBitmap = CreateCompatibleBitmap(hDC, WINCX, WINCY);
		HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

		HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
		FillRect(hMemDC, &rect, hBrush);
		HBRUSH hBrush2 = CreateSolidBrush(RGB(255, 255, 255 ));
		FillRect(hMemDC, &whiteRect, hBrush2);

		BitBlt(hDC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);

		// 정리
		SelectObject(hMemDC, hOldBitmap);
		DeleteObject(hBitmap);
		DeleteObject(hBrush);
		DeleteObject(hBrush2);
		DeleteDC(hMemDC);
	}
}
