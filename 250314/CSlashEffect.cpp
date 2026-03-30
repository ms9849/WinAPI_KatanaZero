#include "pch.h"
#include "CSlashEffect.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"
#include "CEffectMgr.h"

CSlashEffect::CSlashEffect()
{
}

CSlashEffect::~CSlashEffect()
{
}

void CSlashEffect::Initialize()
{
	m_fSpeed = 6.f;
	m_tInfo.fCX = 94.f;
	m_tInfo.fCY = 5.f;
	m_tFrame.iStart = 0;
	m_tFrame.iEnd = 0;
	m_tFrame.iMotion = 0;
	m_pImage = new Image(L"../Image/Player/0.png");
}

int CSlashEffect::Update()
{
    return NOEVENT;
}

void CSlashEffect::Late_Update()
{
}

void CSlashEffect::Render(HDC hDC)
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

	// 회전
	g.RotateTransform(180 - m_fAngle);  // 회전할 각도 (예: 90, 180 등)

	// 이미지를 회전 후 그리기 (이미지의 중심을 기준으로 회전되므로, 그리기 위치를 조정해야 함)
	g.DrawImage(m_pImage,
		-(INT)m_tInfo.fCX * 9 / 2,   // X 좌표 (이미지의 중심을 맞추기 위한 보정)
		-(INT)m_tInfo.fCY / 2,         // Y 좌표 (이미지의 중심을 맞추기 위한 보정)
		(INT)m_tInfo.fCX * 9,         // 이미지의 너비
		(INT)m_tInfo.fCY);             // 이미지의 높이

	g.ResetTransform();  // 변환 초기화

	m_tInfo.fX += 5 * m_fSpeed * g_fSlowFactor * cosf(m_fAngle * PI / 180);
	m_tInfo.fY -= 5 * m_fSpeed * g_fSlowFactor * sinf(m_fAngle * PI / 180);

	if (fabsf(m_tInfo.fX) >= 3000 || fabsf(m_tInfo.fY) >= 3000 || (GetTickCount64() - m_fCreatedTime >= 10000))
		m_bDead = true;
}

void CSlashEffect::Release()
{
}
