#include "pch.h"
#include "CStainEffect.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"
#include "CEffectMgr.h"

CStainEffect::CStainEffect()
{
}

CStainEffect::~CStainEffect()
{
}

void CStainEffect::Initialize()
{
	m_tInfo.fCX = 151.f;
	m_tInfo.fCY = 141.f;
	m_tFrame.iStart = 0;
	m_tFrame.iEnd = 0;
	m_tFrame.iMotion = 0;
	m_pImage = new Image(L"../Image/Enemy/blood_stain.png");
}

int CStainEffect::Update()
{
    return NOEVENT;
}

void CStainEffect::Late_Update()
{
}

void CStainEffect::Render(HDC hDC)
{
	if (m_bDead)
	{
		if (m_pImage != nullptr) Safe_Delete(m_pImage);
		return;
	}

	int		iScrollX = (INT)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (INT)CScrollMgr::Get_Instance()->Get_ScrollY();

	Graphics g(hDC);
	g.TranslateTransform((INT)m_tInfo.fX + iScrollX, (INT)m_tInfo.fY + iScrollY);
	g.RotateTransform(180 - m_fAngle);  
	g.DrawImage(m_pImage, 0, -(INT)m_tInfo.fCY/2,         // Y 좌표 (이미지의 중심을 맞추기 위한 보정)
		(INT)m_tInfo.fCX,         // 이미지의 너비
		(INT)m_tInfo.fCY);             // 이미지의 높이

	g.ResetTransform();  // 변환 초기화

	if (fabsf(m_tInfo.fX) >= 3000 || fabsf(m_tInfo.fY) >= 3000)
		m_bDead = true;

}

void CStainEffect::Release()
{
}
