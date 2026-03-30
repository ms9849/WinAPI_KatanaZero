#include "pch.h"
#include "CRectMgr.h"
#include "CRect.h"
#include "CKeyMgr.h"

CRectMgr* CRectMgr::m_pInstance = nullptr;

CRectMgr::CRectMgr() : m_bShowRect(false)
{
}

CRectMgr::~CRectMgr()
{
	Release();
}

void CRectMgr::Initialize()
{

}

void CRectMgr::Update()
{
	
}

void CRectMgr::Late_Update()
{
//	for (auto& pRect : m_Rectlist)
//		pRect->Late_Update();
}

void CRectMgr::Render(HDC hDC)
{
	for (auto& pRect : m_Rectlist)
	{
		pRect->Render(hDC);
	}

	/*
	if (m_bShowRect)
	{
		for (auto& pRect : m_Rectlist)
		{
			pRect->Render(hDC);
		}
	}
	*/
}

void CRectMgr::Release()
{
	for_each(m_Rectlist.begin(), m_Rectlist.end(), Safe_Delete<CRect*>);
	m_Rectlist.clear();
}
