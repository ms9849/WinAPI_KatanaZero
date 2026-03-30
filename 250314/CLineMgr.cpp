#include "pch.h"
#include "CLineMgr.h"
#include "CKeyMgr.h"
#include "CLine.h"

CLineMgr*  CLineMgr::m_pInstance = nullptr;

CLineMgr::CLineMgr() : m_bShowLine(false)
{
	ZeroMemory(m_tPoint, sizeof(m_tPoint));
}

CLineMgr::~CLineMgr()
{
	Release();
}

void CLineMgr::Save_Line(const TCHAR* _filePath)
{
	//CFileMgr::SaveData<CLine>(_filePath, m_Linelist);
}

void CLineMgr::Load_Line(const TCHAR* _filePath)
{
	//CFileMgr::LoadData<CLine>(_filePath, m_Linelist);
}

void CLineMgr::Initialize()
{	
}

int CLineMgr::Update()
{
	

	return 0;
}

void CLineMgr::Late_Update()
{
	
	/*
	for (auto it = m_Linelist.begin(); it != m_Linelist.end(); )
	{
		if ((*it)->GetObjType() == TYPE_LASER && (*it)->Get_Dead())
		{
			Safe_Delete(*it);            // 포인터 delete
			it = m_Linelist.erase(it);   // 리스트에서 제거하고 다음 원소로 이동
		}
		else
		{
			++it; // 삭제 안했으면 다음 원소로
		}
	}
	*/
}

void CLineMgr::Render(HDC hDC)
{
	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	// 이전 펜/브러시 저장
	HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);

	for (auto& pLine : m_Linelist)
		pLine->Render(hDC);

	SelectObject(hDC, hOldPen);
	// 생성한 펜 제거
	DeleteObject(hPen);

	/*
	if (m_bShowLine)
	{
		HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
		// 이전 펜/브러시 저장
		HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);

		for (auto& pLine : m_Linelist)
			pLine->Render(hDC);

		SelectObject(hDC, hOldPen);
		// 생성한 펜 제거
		DeleteObject(hPen);
	}
	*/
}

void CLineMgr::Release()
{
	for_each(m_Linelist.begin(), m_Linelist.end(), Safe_Delete<CLine*>);
	m_Linelist.clear();
}
