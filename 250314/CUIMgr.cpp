#include "pch.h"
#include "CUIMgr.h"

CUIMgr* CUIMgr::m_pInstance = nullptr;

CUIMgr::CUIMgr()
{
}

CUIMgr::~CUIMgr()
{
	Release();
}

void CUIMgr::Initialize()
{
}

void CUIMgr::Update()
{
}

void CUIMgr::Late_Update()
{

}

void CUIMgr::Render(HDC hDC)
{
	for (auto& Dst : m_vecUI)
	{
		Dst->Render(hDC);
	}
}

void CUIMgr::Release()
{
	for_each(m_vecUI.begin(), m_vecUI.end(), Safe_Delete<CUI*>);
	m_vecUI.clear();
}
