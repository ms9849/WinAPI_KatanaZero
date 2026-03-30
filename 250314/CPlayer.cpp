#include "pch.h"
#include "CPlayer.h"
#include "CBmpMgr.h"
#include "CKeyMgr.h"
#include "CLineMgr.h"
#include "CRectMgr.h"
#include "CScrollMgr.h"
#include "CObjMgr.h"
#include "CCollisionMgr.h"
#include "CStage3.h"
#include "CSceneMgr.h"
#include "CUiMgr.h"
#include "CSoundMgr.h"
#include "CEffectMgr.h"

CPlayer::CPlayer() :  m_ePreMotion(MOTION_END), m_eCurMotion(IDLE), m_bJump(false), m_fTime(0.f), m_bFallFlag(false), m_bAttackFloat(false), m_bCollided(false), m_iAttackCount(0), m_fAttackTime(0.f), m_fAttackVelocity(15.f), m_bWallGrap(false), 
m_iSlowGauge(500), m_iMaxSlowGauge(500), m_bInvincible(false), m_fDeathTime(0.f), m_bDeadFlag(false)
{
}

CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Initialize()
{
	m_tInfo.fCX = 70.f;
	m_tInfo.fCY = 70.f;
	m_tSpriteInfo = m_tInfo; // sprite Info의 첫 크기는 복사받음.
	m_fSpeed = 5.5f;
	m_fRollSpeed = 9.f;
	m_fVelocity = 11.f;
	m_fMaxJumpSpeed = -6.5f;
	m_fMaxWallJumpSpeed = -15.f;
	m_fAngle = 0.f;
	m_fAttackDelayTime = GetTickCount64();
	m_fRollDelayTime = GetTickCount64();
	m_fRunSoundDelayTime = GetTickCount64();
	m_pHand = nullptr;

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/player_idle_11x2.bmp", L"Player_Idle");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/player_run_10x2.bmp", L"Player_Run");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/player_dance_12x1.bmp", L"Player_Dance");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/player_jump_4x2.bmp", L"Player_Jump");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/player_fall_4x2.bmp", L"Player_Fall");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/player_roll_7x2.bmp", L"Player_Roll");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/player_attack_7x2.bmp", L"Player_Attack");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/player_run_to_idle_5x2.bmp", L"Player_Run_to_Idle");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/player_idle_to_run_4x2.bmp", L"Player_Idle_to_Run");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/player_flip_11x2.bmp", L"Player_WallJump");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/player_wallgrap_1x2.bmp", L"Player_WallGrap");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/player_death_jump_4x2.bmp", L"Player_Death_Jump");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/player_death_ground_6x2.bmp", L"Player_Death_Ground");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/player_draw_sword_19x1.bmp", L"Player_Draw_Sword");

	m_pFrameKey = L"Player_Idle";
	m_tFrame.iStart = 0;
	m_tFrame.iEnd = 10;
	m_tFrame.iMotion = 0;
	m_tFrame.dwTime = GetTickCount64();
	m_tFrame.dwFrameSpeed = 75.f; //추후 time 변수 추가
}

int CPlayer::Update()
{
	__super::Update_Rect();

	if (m_bDead == true && !m_bInvincible)
		return NOEVENT;

	if (!m_bCollided && (m_eCurMotion == LEFT_RUN || m_eCurMotion == RIGHT_RUN ||
		m_eCurMotion == LEFT_ROLL || m_eCurMotion == RIGHT_ROLL))
	{
		m_pFrameKey = L"Player_Fall";
		if (m_tFrame.iMotion == 0)
			m_eCurMotion = RIGHT_FALL;
		else if (m_tFrame.iMotion == 1)
			m_eCurMotion = LEFT_FALL;
	}
	/*
		if (!m_bOnGround && (m_eCurMotion == LEFT_RUN || m_eCurMotion == RIGHT_RUN ||
		m_eCurMotion == LEFT_ATTACK || m_eCurMotion == RIGHT_ATTACK ||
		m_eCurMotion == LEFT_RUN_TO_IDLE || m_eCurMotion == RIGHT_RUN_TO_IDLE ||
		m_eCurMotion == LEFT_ROLL || m_eCurMotion == RIGHT_ROLL || 
		m_eCurMotion == LEFT_IDLE_TO_RUN || m_eCurMotion == RIGHT_IDLE_TO_RUN))
		m_tInfo.fY += 1.0f;
	*/
	return NOEVENT;
}

void CPlayer::Late_Update()
{
	if (m_bDead == true && !m_bInvincible)
	{
		Death();
		__super::Update_Frame();
		__super::Update_Sprite();
		
		return;
	}

	if (m_eCurMotion == DRAW_SWORD)
	{
		ShiShiSonSon();
		__super::Update_Frame();
		__super::Update_Sprite();
		Motion_Change();
		return;
	}

	Key_Input();
	Attack();
	Run_to_Idle();
	Idle_to_Run();
	Jump();
	Roll();
	__super::Update_Frame();
	__super::Update_Sprite();
	Motion_Change();
	Create_Trail();
	Fall();
	Offset();
	m_bOnGround = false;
	m_bCollided = false;
}

void CPlayer::Render(HDC hDC)
{
	int		iScrollX = (INT)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (INT)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
	//Rectangle(hDC, tInfo.fX - halfWidth, tInfo.fY - halfHeight, tInfo.fX  + halfWidth, tInfo.fY + halfHeight);

	if (!g_bMonoChrome)
	{
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

void CPlayer::Release()
{
	if (m_pHand != nullptr)
		Safe_Delete(m_pHand);
}

void CPlayer::Create_Trail()
{
	if ((m_pFrameKey == L"Player_Jump" || m_pFrameKey == L"Player_Run" || m_pFrameKey == L"Player_Attack" ||
		m_pFrameKey == L"Player_Roll" || m_pFrameKey == L"Player_WallJump" || m_pFrameKey == L"Player_Fall"))
	{
		CEffect* pEffect = new CRemainEffect(m_tInfo.fX, m_tInfo.fY, m_pFrameKey, m_tFrame);
		pEffect->Initialize();
		CEffectMgr::Get_Instance()->Add_Effect(pEffect);
	}
}

void CPlayer::Death()
{
	if (m_eCurMotion != LEFT_DEATH_GROUND && m_eCurMotion != RIGHT_DEATH_GROUND &&
		m_eCurMotion != LEFT_DEATH_JUMP && m_eCurMotion != RIGHT_DEATH_JUMP)
	{
		if (m_tFrame.iMotion == 1)
			m_eCurMotion = LEFT_DEATH_JUMP;
		else
			m_eCurMotion = RIGHT_DEATH_JUMP;

		m_pFrameKey = L"Player_Death_Jump";
		Motion_Change();
	}


	m_fDeathTime += 0.13f * g_fSlowFactor;

	if(m_pFrameKey == L"Player_Death_Jump") m_tInfo.fX += m_fSpeed * cosf((m_tFrame.iMotion == 1 ? 45 : 135) * PI / 180);

	if (1.2 * m_fSpeed * g_fSlowFactor * m_fDeathTime - 0.5 * 3.9 * m_fDeathTime * m_fDeathTime * g_fSlowFactor > -8.0f)
		m_tInfo.fY -= 1.2 * m_fSpeed * g_fSlowFactor * m_fDeathTime - 0.5 * 3.9 * m_fDeathTime * m_fDeathTime * g_fSlowFactor;
	else
		m_tInfo.fY -= -8.0f;

	if (isAnimationEnd() && m_pFrameKey == L"Player_Death_Ground")
	{
		m_fAttackTime = 0.f;
		CUIMgr::Get_Instance()->Add_UI(new CRestartUI);
		g_bGameOver = true;
	}
}

void CPlayer::Key_Input()
{
	if (CKeyMgr::Get_Instance()->Key_Down('H'))
	{
		int iScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
		int iScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();
		//여기서 필살기 로직 작성. H일 필욘 없지만.. 이게 제일 무난할듯?
		CSoundMgr::Get_Instance()->StopAll();
		CSoundMgr::Get_Instance()->PlaySound(L"pilsalgi.mp3", SOUND_EFFECT, g_fVolume);

		CEffect* pEffect = new CRemainEffect(600 - iScrollX, 300 - iScrollY, L"Zoro");
		pEffect->Initialize();
		CEffectMgr::Get_Instance()->Add_Front_Effect(pEffect);
	
		//pEffect = new CRemainEffect(m_tInfo.fX, m_tInfo.fY - 20, L"Wind");
		//pEffect->Initialize();
		//CEffectMgr::Get_Instance()->Add_Front_Effect(pEffect);

		m_eCurMotion = DRAW_SWORD;
		m_pFrameKey = L"Player_Draw_Sword";
		g_bDark = true;
		m_bInvincible = true;
		Motion_Change();
	}
	if (m_eCurMotion != LEFT_ROLL && m_eCurMotion != RIGHT_ROLL && m_eCurMotion != LEFT_ATTACK &&
		m_eCurMotion != RIGHT_ATTACK
		)
	{
		//공격은 따로 처리해준다.
		if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
		{
			if (GetTickCount64() > m_fAttackDelayTime + 280)
			{
				float fMouseX = CObjMgr::Get_Instance()->Get_Mouse()->Get_Info()->fX - CScrollMgr::Get_Instance()->Get_ScrollX();
				float fMouseY = CObjMgr::Get_Instance()->Get_Mouse()->Get_Info()->fY - CScrollMgr::Get_Instance()->Get_ScrollY();

				float fDistX = fMouseX - m_tInfo.fX;
				float fDistY = fMouseY - m_tInfo.fY;
				float fDiagonal = sqrtf(fDistX * fDistX + fDistY * fDistY);

				if (fMouseY <= m_tInfo.fY)
					m_fAttackAngle = acosf(fDistX / fDiagonal) * (180 / PI);

				else
					m_fAttackAngle = 360 - acosf(fDistX / fDiagonal) * (180 / PI);

				if (m_bJump == true)
					m_bAttackFloat = true;

				else if (m_bJump == false && fMouseY <= m_tInfo.fY)
				{
					m_bAttackFloat = true;
					m_bJump = true;
					m_fTime = 2.5f;
				}

				m_iAttackCount++;

				if (m_tInfo.fX < CObjMgr::Get_Instance()->Get_Mouse()->Get_Info()->fX - (INT)CScrollMgr::Get_Instance()->Get_ScrollX())
				{
					if (m_bWallGrap) 
					{
						m_bWallGrap = false;
						CSoundMgr::Get_Instance()->StopSound(SOUND_WALLGRAP);
					}
					m_pFrameKey = L"Player_Attack";
					m_eCurMotion = RIGHT_ATTACK;
					Motion_Change();
				}
				else
				{
					if (m_bWallGrap) 
					{
						m_bWallGrap = false;
						CSoundMgr::Get_Instance()->StopSound(SOUND_WALLGRAP);
					}
					m_pFrameKey = L"Player_Attack";
					m_eCurMotion = LEFT_ATTACK;
					Motion_Change();
				}

				int randomIndex = rand() % 3 + 1;

				// 파일명을 무작위로 선택
				TCHAR soundKey[32];
				swprintf_s(soundKey, 32, L"slash_%d.wav", randomIndex);
				CSoundMgr::Get_Instance()->PlaySound(soundKey, SOUND_EFFECT, g_fVolume);

				m_fAttackDelayTime = GetTickCount64();

				CObj* pHitBox = new CHitBox(CHitBox::PLAYER, this, m_fAttackAngle);
				pHitBox->Initialize();
				CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER_HITBOX, pHitBox);
			}
		}
		else if (CKeyMgr::Get_Instance()->Key_Down(VK_RBUTTON))
		{
			if (m_pHand != nullptr)
			{
				float fMouseX = CObjMgr::Get_Instance()->Get_Mouse()->Get_Info()->fX - CScrollMgr::Get_Instance()->Get_ScrollX();
				float fMouseY = CObjMgr::Get_Instance()->Get_Mouse()->Get_Info()->fY - CScrollMgr::Get_Instance()->Get_ScrollY();

				float fDistX = fMouseX - m_tInfo.fX;
				float fDistY = fMouseY - m_tInfo.fY;
				float fDiagonal = sqrtf(fDistX * fDistX + fDistY * fDistY);

				if (fMouseY <= m_tInfo.fY)
					m_fAttackAngle = acosf(fDistX / fDiagonal) * (180 / PI);

				else
					m_fAttackAngle = 360 - acosf(fDistX / fDiagonal) * (180 / PI);

				dynamic_cast<CItem*>(m_pHand)->Set_Throw(true);
				dynamic_cast<CItem*>(m_pHand)->Set_ThrowAngle(m_fAttackAngle);
				m_pHand->Set_Pos(m_tInfo.fX, m_tInfo.fY);
				CObjMgr::Get_Instance()->Add_Object(OBJ_ITEM, m_pHand);
				CSoundMgr::Get_Instance()->PlaySound(L"item_throw.wav", SOUND_EFFECT, g_fVolume);
				m_pHand = nullptr;
			}
		}
		//점프와 구르기도 따로 처리해준다.
		if (CKeyMgr::Get_Instance()->Key_Down('W'))
		{
			if (m_bJump == false)
			{
				m_bJump = true;
				m_fTime = 0.5f;
				m_pFrameKey = L"Player_Jump";

				if (m_tFrame.iMotion == 0)
					m_eCurMotion = RIGHT_JUMP;

				else if (m_tFrame.iMotion == 1)
					m_eCurMotion = LEFT_JUMP;

				CSoundMgr::Get_Instance()->PlaySound(L"player_jump.wav", SOUND_EFFECT, g_fVolume);

				CEffect* pEffect = new CRemainEffect(m_tInfo.fX, m_tInfo.fY, L"Player_Jumpdust");
				pEffect->Initialize();
				CEffectMgr::Get_Instance()->Add_Effect(pEffect);
			}
			else if (m_eCurMotion == LEFT_WALLGRAP || m_eCurMotion == RIGHT_WALLGRAP)
			{
				m_bWallGrap = false;
				CSoundMgr::Get_Instance()->StopSound(SOUND_WALLGRAP);
				m_pFrameKey = L"Player_WallJump";

				if (m_tFrame.iMotion == 0)
				{
					m_eCurMotion = LEFT_WALLJUMP;
					m_fAngle = 130.f;
					m_fTime = 0.f;
				}

				else if (m_tFrame.iMotion == 1)
				{
					m_eCurMotion = RIGHT_WALLJUMP;
					m_fAngle = 50.f;
					m_fTime = 0.f;
				}
				CSoundMgr::Get_Instance()->PlaySound(L"player_roll.wav", SOUND_EFFECT, g_fVolume);
			}
			//오른쪽이면 motion 0, 오른쪽이 기본이다.
		}
		if (!m_bWallGrap)
		{
			if (CKeyMgr::Get_Instance()->Key_Down('S') && GetTickCount64() - m_fRollDelayTime >= 400 / g_fSlowFactor)
			{
				if (m_bJump == false)
				{
					m_pFrameKey = L"Player_Roll";
					if (m_tFrame.iMotion == 0)
						m_eCurMotion = RIGHT_ROLL;

					else if (m_tFrame.iMotion == 1)
						m_eCurMotion = LEFT_ROLL;

					CSoundMgr::Get_Instance()->PlaySound(L"player_roll.wav", SOUND_EFFECT, g_fVolume);
					m_fRollDelayTime = GetTickCount64();
				}
			}
			if (CKeyMgr::Get_Instance()->Key_Pressing('D')) //오른쪽 로직 다 집어넣어야 하나?
			{
				if (m_eCurMotion != LEFT_WALLGRAP && m_eCurMotion != RIGHT_WALLGRAP &&
					m_eCurMotion != LEFT_WALLJUMP && m_eCurMotion != RIGHT_WALLJUMP &&
					m_eCurMotion != LEFT_ROLL && m_eCurMotion != RIGHT_ROLL)
				{
					m_tInfo.fX += m_fSpeed * g_fSlowFactor;

					if (m_eCurMotion == LEFT_JUMP || m_eCurMotion == RIGHT_JUMP)
					{
						m_pFrameKey = L"Player_Jump";
						m_eCurMotion = RIGHT_JUMP;
					}
					else if (m_eCurMotion == LEFT_FALL || m_eCurMotion == RIGHT_FALL)
					{
						m_pFrameKey = L"Player_Fall";
						m_eCurMotion = RIGHT_FALL;
					}
					else if (m_eCurMotion == IDLE)
					{
						CEffect* pEffect = new CRemainEffect(m_tInfo.fX, m_tInfo.fY, L"RIGHT_RUSH");
						pEffect->Initialize();
						CEffectMgr::Get_Instance()->Add_Effect(pEffect);

						m_pFrameKey = L"Player_Idle_to_Run";
						m_eCurMotion = RIGHT_IDLE_TO_RUN;
					}
					else if (m_eCurMotion == LEFT_RUN || m_eCurMotion == RIGHT_RUN ||
						m_eCurMotion == RIGHT_RUN_TO_IDLE || m_eCurMotion == LEFT_RUN_TO_IDLE
						|| m_eCurMotion == LEFT_IDLE_TO_RUN)
					{
						m_pFrameKey = L"Player_Run";
						m_eCurMotion = RIGHT_RUN;

						if (GetTickCount64() - m_fRunSoundDelayTime >= 300)
						{
							int randomIndex = rand() % 4 + 1;
							// 파일명을 무작위로 선택
							TCHAR soundKey[32];
							swprintf_s(soundKey, 32, L"player_running_%d.wav", randomIndex);
							CSoundMgr::Get_Instance()->PlaySound(soundKey, SOUND_EFFECT, g_fVolume);
							m_fRunSoundDelayTime = GetTickCount64();
						}
					}
				}
			}

			else if (CKeyMgr::Get_Instance()->Key_Pressing('A')) // 왼쪽 관련 로직들..
			{
				if (m_eCurMotion != LEFT_WALLGRAP && m_eCurMotion != RIGHT_WALLGRAP &&
					m_eCurMotion != LEFT_WALLJUMP && m_eCurMotion != RIGHT_WALLJUMP && 
					m_eCurMotion != LEFT_ROLL && m_eCurMotion != RIGHT_ROLL)
				{
					
				m_tInfo.fX -= m_fSpeed * g_fSlowFactor;

					if (m_eCurMotion == RIGHT_JUMP || m_eCurMotion == LEFT_JUMP)
					{
						m_pFrameKey = L"Player_Jump";
						m_eCurMotion = LEFT_JUMP;
					}
					else if (m_eCurMotion == RIGHT_FALL || m_eCurMotion == LEFT_FALL)
					{
						m_pFrameKey = L"Player_Fall";
						m_eCurMotion = LEFT_FALL;
					}
					else if (m_eCurMotion == IDLE)
					{
						CEffect* pEffect = new CRemainEffect(m_tInfo.fX, m_tInfo.fY, L"LEFT_RUSH");
						pEffect->Initialize();
						CEffectMgr::Get_Instance()->Add_Effect(pEffect);

						m_pFrameKey = L"Player_Idle_to_Run";
						m_eCurMotion = LEFT_IDLE_TO_RUN;
					}
					else if (m_eCurMotion == LEFT_RUN || m_eCurMotion == RIGHT_RUN || m_eCurMotion == RIGHT_RUN_TO_IDLE || m_eCurMotion == LEFT_RUN_TO_IDLE || m_eCurMotion == RIGHT_IDLE_TO_RUN)
					{
						m_pFrameKey = L"Player_Run";
						m_eCurMotion = LEFT_RUN;

						if (GetTickCount64() - m_fRunSoundDelayTime >= 300)
						{
							int randomIndex = rand() % 4 + 1; 
							// 파일명을 무작위로 선택
							TCHAR soundKey[32];
							swprintf_s(soundKey, 32, L"player_running_%d.wav", randomIndex);
							CSoundMgr::Get_Instance()->PlaySound(soundKey, SOUND_EFFECT, g_fVolume);
							m_fRunSoundDelayTime = GetTickCount64();
						}
					}
				}
			}
			else if (CKeyMgr::Get_Instance()->Key_Pressing('L'))
			{
				m_pFrameKey = L"Player_Dance";
				m_eCurMotion = DANCE;
			}
			else
			{
				if (m_eCurMotion == LEFT_RUN || m_eCurMotion == RIGHT_RUN
					)
				{
					m_pFrameKey = L"Player_Run_to_Idle";

					if (m_tFrame.iMotion == 0)
						m_eCurMotion = RIGHT_RUN_TO_IDLE;

					else if (m_tFrame.iMotion == 1)
						m_eCurMotion = LEFT_RUN_TO_IDLE;
				}
			}
		}
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_F2))
	{
		CLineMgr::Get_Instance()->Set_Show();
		CRectMgr::Get_Instance()->Set_Show();
	}

	if (CKeyMgr::Get_Instance()->Key_Down(VK_F3))
	{
		m_bInvincible = !m_bInvincible;
	}

	if (!g_bMonoChrome)
	{
		if (CKeyMgr::Get_Instance()->Key_Pressing(VK_SHIFT) && m_iSlowGauge >= 50)
		{
			if (!g_bSlowMod)
			{
				CSoundMgr::Get_Instance()->StopSound(SOUND_SLOW_MOTION);
				CSoundMgr::Get_Instance()->PlaySoundOne(L"slow_enter.wav", SOUND_SLOW_MOTION, g_fVolume);
			}
			CSoundMgr::Get_Instance()->Set_PlaySpeed(0.85f);
			g_bSlowMod = true;
			g_fSlowFactor = 0.3f;
			m_iSlowGauge -= 1;
			if (m_iSlowGauge <= 50) m_iSlowGauge = 0;
		}
		else
		{
			if (g_bSlowMod)
			{
				CSoundMgr::Get_Instance()->StopSound(SOUND_SLOW_MOTION);
				CSoundMgr::Get_Instance()->PlaySoundOne(L"slow_exit.wav", SOUND_SLOW_MOTION, g_fVolume);
			}

			g_bSlowMod = false;
			g_fSlowFactor = 1.0f;
			CSoundMgr::Get_Instance()->Set_PlaySpeed(1.0f);

			if (m_iSlowGauge < m_iMaxSlowGauge)
				m_iSlowGauge += 1;
		}
	}
}

void CPlayer::Run_to_Idle()
{
	if (m_eCurMotion == LEFT_RUN_TO_IDLE)
	{
		if (m_tFrame.iStart == 0)
		{
			m_eCurMotion = IDLE;
			m_pFrameKey = L"Player_Idle";
		}
	}
	else if (m_eCurMotion == RIGHT_RUN_TO_IDLE)
	{
		if (m_tFrame.iStart == m_tFrame.iEnd)
		{
			m_eCurMotion = IDLE;
			m_pFrameKey = L"Player_Idle";
		}
	}
}

void CPlayer::Idle_to_Run()
{
	if (m_eCurMotion == LEFT_IDLE_TO_RUN)
	{
		if (m_tFrame.iStart == 0)
		{
			m_eCurMotion = LEFT_RUN;
			m_pFrameKey = L"Player_Run";
		}
	}
	else if (m_eCurMotion == RIGHT_IDLE_TO_RUN)
	{
		if (m_tFrame.iStart == m_tFrame.iEnd)
		{
			m_eCurMotion = RIGHT_RUN;
			m_pFrameKey = L"Player_Run";
		}
	}
}

void CPlayer::Attack()
{
	if ((m_eCurMotion == RIGHT_ATTACK && m_tFrame.iStart == m_tFrame.iEnd) ||
		m_eCurMotion == LEFT_ATTACK && m_tFrame.iStart == 0)
	{
		if (m_bAttackFloat == false)
		{
			m_pFrameKey = L"Player_Idle";
			m_eCurMotion = IDLE;
			m_bAttackFloat = false;

		}
		else if (m_bAttackFloat == true)
		{
			m_pFrameKey = L"Player_Fall";
			if (m_tFrame.iMotion == 0)
				m_eCurMotion = RIGHT_FALL;
			else if (m_tFrame.iMotion == 1)
				m_eCurMotion = LEFT_FALL;

			m_bAttackFloat = false;
		}
	}

	if (m_eCurMotion == LEFT_ATTACK || m_eCurMotion == RIGHT_ATTACK)
	{
		m_tInfo.fX += 4.0f * cosf(m_fAttackAngle * PI / 180) * g_fSlowFactor;
	}
}

void CPlayer::ShiShiSonSon()
{
	if (m_eCurMotion == DRAW_SWORD && isAnimationEnd())
	{
		m_eCurMotion = IDLE;
		m_pFrameKey = L"Player_Idle";
		m_bInvincible = false;
		g_bDark = false;
	}
}

void CPlayer::Roll()
{
	if ((m_eCurMotion == RIGHT_ROLL && m_tFrame.iStart == m_tFrame.iEnd) ||
		m_eCurMotion == LEFT_ROLL && m_tFrame.iStart == 0)
	{
		m_pFrameKey = L"Player_Idle";
		m_eCurMotion = IDLE;
	}

	if (m_eCurMotion == LEFT_ROLL)
	{
		m_tInfo.fX -= 9.f * g_fSlowFactor;
	}
	else if (m_eCurMotion == RIGHT_ROLL)
	{
		m_tInfo.fX += 9.f * g_fSlowFactor;
	}
}

void CPlayer::Jump()
{
	if (m_bJump)
	{
		m_fJumpSpeed = m_fVelocity * m_fTime - 0.5f * 4.9f * m_fTime * m_fTime;

		if (m_eCurMotion == LEFT_WALLJUMP || m_eCurMotion == RIGHT_WALLJUMP)
		{
			if (m_fJumpSpeed >= 0)
				m_tInfo.fY -= 8.f * g_fSlowFactor;

			else
				m_tInfo.fY -= -5.f * g_fSlowFactor;

			m_tInfo.fX += 12.f * cosf(m_fAngle * PI / 180) * g_fSlowFactor;
			m_fTime -= 0.08f * g_fSlowFactor;
		}
		else if (!m_bAttackFloat)
		{
			if (!m_bWallGrap)
			{
				if (m_fJumpSpeed > m_fMaxJumpSpeed && m_fJumpSpeed < m_fSpeed)
					m_tInfo.fY -= m_fJumpSpeed * g_fSlowFactor;

				else if (m_fJumpSpeed > m_fSpeed)
					m_tInfo.fY -= m_fSpeed * g_fSlowFactor;

				else
					m_tInfo.fY -= m_fMaxJumpSpeed * g_fSlowFactor;
			}
			else if (m_bWallGrap)
			{
				m_fWallFallSpeed = 9.5f * m_fTime - 0.5f * 4.9f * m_fTime * m_fTime;

				if (m_fWallFallSpeed >= m_fMaxJumpSpeed * 0.5 && m_fWallFallSpeed <= m_fSpeed * 0.8f)
					m_tInfo.fY -= m_fWallFallSpeed * g_fSlowFactor;

				else if (m_fWallFallSpeed > m_fSpeed * 0.8f)
					m_tInfo.fY -= m_fWallFallSpeed * 0.8f * g_fSlowFactor;

				else
				{
					m_tInfo.fY -= m_fMaxJumpSpeed * 0.5 * g_fSlowFactor;
					CSoundMgr::Get_Instance()->PlaySoundOne(L"player_wallslide.wav", SOUND_WALLGRAP, g_fVolume);

					CEffect* pEffect = new CRemainEffect(m_tInfo.fX, m_tInfo.fY, L"Player_Dust");
					pEffect->Initialize();
					CEffectMgr::Get_Instance()->Add_Effect(pEffect);
				}


				m_fTime -= 0.08f * g_fSlowFactor;
			}
		}
		/*
		else if (!m_bAttackFloat)
		{
			if (!m_bWallGrap)
			{
				if (m_fJumpSpeed > m_fMaxJumpSpeed && m_fJumpSpeed < m_fSpeed)
					m_tInfo.fY -= m_fJumpSpeed * g_fSlowFactor;

				else if (m_fJumpSpeed > m_fSpeed)
					m_tInfo.fY -= m_fSpeed * g_fSlowFactor;

				else
					m_tInfo.fY -= m_fMaxJumpSpeed * g_fSlowFactor;
			}
			else if (m_bWallGrap)
			{
				m_fWallFallSpeed = 11.5f * m_fTime - 0.5f * 4.9f * m_fTime * m_fTime;

				if (m_fWallFallSpeed >= m_fMaxJumpSpeed * 0.5 && m_fWallFallSpeed <= m_fSpeed * 0.8f)
					m_tInfo.fY -= m_fWallFallSpeed * g_fSlowFactor;

				else if (m_fWallFallSpeed > m_fSpeed * 0.8f)
					m_tInfo.fY -= m_fSpeed * 0.8f * g_fSlowFactor;

				else
					m_tInfo.fY -= m_fMaxJumpSpeed * 0.5 * g_fSlowFactor;


				m_fTime -= 0.08f * g_fSlowFactor;
			}
		}
		*/
		else
		{
			if (m_iAttackCount == 1)
			{
				m_tInfo.fY -= 4.0f * sinf(m_fAttackAngle * PI / 180) * g_fSlowFactor;
				m_tInfo.fX += 2.0f * cosf(m_fAttackAngle * PI / 180) * g_fSlowFactor;
			}
			else
			{
				m_tInfo.fY -= 0.2f * sinf(m_fAttackAngle * PI / 180) * g_fSlowFactor;
				m_tInfo.fX += 2.0f * cosf(m_fAttackAngle * PI / 180) * g_fSlowFactor;
			}
		}

		m_fTime += 0.2f * g_fSlowFactor;
	}
}

void CPlayer::Fall()
{
	if (m_bJump == true)
	{
		// FALL 처리 부분.
		if ((m_fJumpSpeed <= 0 && m_bFallFlag == false &&
			m_eCurMotion != LEFT_ATTACK && m_eCurMotion != RIGHT_ATTACK &&
			m_eCurMotion != LEFT_WALLJUMP && m_eCurMotion != RIGHT_WALLJUMP &&
			m_bWallGrap != true)
			)
		{
			m_pFrameKey = L"Player_Fall";
			if (m_tFrame.iMotion == 0)
				m_eCurMotion = RIGHT_FALL;
			else if (m_tFrame.iMotion == 1)
				m_eCurMotion = LEFT_FALL;
		}

		//벽잡 떨어지기
		else if ((m_eCurMotion == LEFT_WALLGRAP || m_eCurMotion == RIGHT_WALLGRAP) && m_bCollided == false)
		{
			m_bWallGrap = false;
			CSoundMgr::Get_Instance()->StopSound(SOUND_WALLGRAP);
			m_pFrameKey = L"Player_Fall";

			if (m_tFrame.iMotion == 0)
				m_eCurMotion = LEFT_FALL;
			else if (m_tFrame.iMotion == 1)
				m_eCurMotion = RIGHT_FALL;
		}
		m_bFallFlag = true;
	}
	else if (false == CCollisionMgr::Check_PlayerUnderLine(CObjMgr::Get_Instance()->Get_Playerlist(), CLineMgr::Get_Instance()->Get_LineList()))
	{
		m_bJump = true;
		m_fTime = 4.2f;

		m_bFall = true;
		m_tInfo.fY += 8.0f;
		
		//m_pFrameKey = L"Player_Fall";
		//if (m_tFrame.iMotion == 0)
			//m_eCurMotion = RIGHT_FALL;
		//else if (m_tFrame.iMotion == 1)
			//m_eCurMotion = LEFT_FALL;

	}
	else if (m_bWallGrap)
		m_tInfo.fY += 0.6f;
}

void CPlayer::Offset()
{
	int		iOffsetMinX = 300;
	int		iOffsetMaxX = 450;

	int		iOffsetMinY = 200;
	int		iOffsetMaxY = 550;

	int		iScrollX = (INT)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (INT)CScrollMgr::Get_Instance()->Get_ScrollY();


	if (iOffsetMinX > m_tInfo.fX + iScrollX)
		CScrollMgr::Get_Instance()->Set_ScrollX(m_fSpeed);

	if (iOffsetMaxX < m_tInfo.fX + iScrollX)
		CScrollMgr::Get_Instance()->Set_ScrollX(-1.f * m_fSpeed);

	if (iOffsetMinY > m_tInfo.fY + iScrollY)
		CScrollMgr::Get_Instance()->Set_ScrollY(m_fSpeed);

	if (iOffsetMaxY < m_tInfo.fY + iScrollY)
		CScrollMgr::Get_Instance()->Set_ScrollY(-1.f * m_fSpeed);
}

void CPlayer::Motion_Change()
{
	if (m_ePreMotion != m_eCurMotion)
	{
		switch (m_eCurMotion)
		{
		case IDLE:
			if (m_ePreMotion == LEFT_RUN || m_ePreMotion == LEFT_JUMP ||
				m_ePreMotion == LEFT_ATTACK || m_ePreMotion == LEFT_ROLL || m_ePreMotion == LEFT_FALL ||
				m_ePreMotion == LEFT_RUN_TO_IDLE || m_ePreMotion == LEFT_IDLE_TO_RUN ||
				m_ePreMotion == RIGHT_WALLGRAP || m_ePreMotion == LEFT_WALLJUMP)
			{
				m_tFrame.iStart = 10;
				m_tFrame.iEnd = 10;
				m_tFrame.iMotion = 1;
			}
			else
			{
				m_tFrame.iStart = 0;
				m_tFrame.iEnd = 10;
				m_tFrame.iMotion = 0;
			}
			m_tSpriteInfo.fCX = 70;
			m_tSpriteInfo.fCY = 70;

			m_tFrame.dwTime = GetTickCount64();
			m_tFrame.dwFrameSpeed = 75;
			break;

		case LEFT_IDLE_TO_RUN:
			//왼쪽 갈떈 end와 start 같이 줘야함
			m_tFrame.iEnd = 3;
			m_tFrame.iStart = 3;
			// motion 값이 1이면 업데이트 거꾸로 돌아감.
			// 왼쪽일떄 motion 값 1
			m_tFrame.iMotion = 1;

			m_tSpriteInfo.fCX = 84;
			m_tSpriteInfo.fCY = 64;
			//sprite rect 크기가 바뀌었다면 그에 따른 위치 조정도 필요하다.

			m_tFrame.dwTime = GetTickCount64();
			m_tFrame.dwFrameSpeed = 75;
			break;

		case RIGHT_IDLE_TO_RUN:
			m_tFrame.iStart = 0;
			m_tFrame.iEnd = 3;
			m_tFrame.iMotion = 0;

			m_tSpriteInfo.fCX = 84;
			m_tSpriteInfo.fCY = 64;

			m_tFrame.dwTime = GetTickCount64();
			m_tFrame.dwFrameSpeed = 75;
			break;

		case LEFT_RUN:
			//왼쪽 갈떈 end와 start 같이 줘야함
			m_tFrame.iEnd = 9;
			m_tFrame.iStart = 9;
			// motion 값이 1이면 업데이트 거꾸로 돌아감.
			// 왼쪽일떄 motion 값 1
			m_tFrame.iMotion = 1;

			m_tSpriteInfo.fCX = 88;
			m_tSpriteInfo.fCY = 64;
			//sprite rect 크기가 바뀌었다면 그에 따른 위치 조정도 필요하다.

			m_tFrame.dwTime = GetTickCount64();
			m_tFrame.dwFrameSpeed = 75;
			break;

		case RIGHT_RUN:
			m_tFrame.iStart = 0;
			m_tFrame.iEnd = 9;
			m_tFrame.iMotion = 0;

			m_tSpriteInfo.fCX = 88;
			m_tSpriteInfo.fCY = 64;

			m_tFrame.dwTime = GetTickCount64();
			m_tFrame.dwFrameSpeed = 75;
			break;

		case LEFT_RUN_TO_IDLE:
			//왼쪽 갈떈 end와 start 같이 줘야함
			m_tFrame.iEnd = 4;
			m_tFrame.iStart = 4;
			// motion 값이 1이면 업데이트 거꾸로 돌아감.
			// 왼쪽일떄 motion 값 1
			m_tFrame.iMotion = 1;

			m_tSpriteInfo.fCX = 80;
			m_tSpriteInfo.fCY = 70;
			//sprite rect 크기가 바뀌었다면 그에 따른 위치 조정도 필요하다.

			m_tFrame.dwTime = GetTickCount64();
			m_tFrame.dwFrameSpeed = 50;
			break;

		case RIGHT_RUN_TO_IDLE:
			m_tFrame.iStart = 0;
			m_tFrame.iEnd = 4;
			m_tFrame.iMotion = 0;

			m_tSpriteInfo.fCX = 80;
			m_tSpriteInfo.fCY = 70;

			m_tFrame.dwTime = GetTickCount64();
			m_tFrame.dwFrameSpeed = 50;
			break;

		case LEFT_ROLL:
			m_tFrame.iStart = 6;
			m_tFrame.iEnd = 6;
			m_tFrame.iMotion = 1;

			m_tSpriteInfo.fCX = 94;
			m_tSpriteInfo.fCY = 64;

			m_tFrame.dwTime = GetTickCount64();
			m_tFrame.dwFrameSpeed = 60;
			break;

		case RIGHT_ROLL:
			m_tFrame.iStart = 0;
			m_tFrame.iEnd = 6;
			m_tFrame.iMotion = 0;

			m_tSpriteInfo.fCX = 94;
			m_tSpriteInfo.fCY = 64;

			m_tFrame.dwTime = GetTickCount64();
			m_tFrame.dwFrameSpeed = 60;
			break;

		case LEFT_JUMP:
			//왼쪽 갈떈 end와 start 같이 줘야함
			m_tFrame.iEnd = 3;
			m_tFrame.iStart = 3;
			// motion 값이 1이면 업데이트 거꾸로 돌아감.
			// 왼쪽일떄 motion 값 1
			m_tFrame.iMotion = 1;

			m_tSpriteInfo.fCX = 60;
			m_tSpriteInfo.fCY = 84;

			m_tFrame.dwTime = GetTickCount64();
			m_tFrame.dwFrameSpeed = 75;
			break;

		case RIGHT_JUMP:
			m_tFrame.iStart = 0;
			m_tFrame.iEnd = 3;
			m_tFrame.iMotion = 0;

			m_tSpriteInfo.fCX = 60;
			m_tSpriteInfo.fCY = 84;

			m_tFrame.dwTime = GetTickCount64();
			m_tFrame.dwFrameSpeed = 75;
			break;

		case LEFT_FALL:
			//왼쪽 갈떈 end와 start 같이 줘야함
			m_tFrame.iEnd = 3;
			m_tFrame.iStart = 3;
			// motion 값이 1이면 업데이트 거꾸로 돌아감.
			// 왼쪽일떄 motion 값 1
			m_tFrame.iMotion = 1;

			m_tSpriteInfo.fCX = 82;
			m_tSpriteInfo.fCY = 96;

			m_tFrame.dwTime = GetTickCount64();
			m_tFrame.dwFrameSpeed = 75;
			break;

		case RIGHT_FALL:
			m_tFrame.iStart = 0;
			m_tFrame.iEnd = 3;
			m_tFrame.iMotion = 0;

			m_tSpriteInfo.fCX = 82;
			m_tSpriteInfo.fCY = 96;

			m_tFrame.dwTime = GetTickCount64();
			m_tFrame.dwFrameSpeed = 75;
			break;

		case LEFT_ATTACK:
			//왼쪽 갈떈 end와 start 같이 줘야함
			m_tFrame.iEnd = 6;
			m_tFrame.iStart = 6;
			// motion 값이 1이면 업데이트 거꾸로 돌아감.
			// 왼쪽일떄 motion 값 1
			m_tFrame.iMotion = 1;

			m_tSpriteInfo.fCX = 120;
			m_tSpriteInfo.fCY = 82;

			m_tFrame.dwTime = GetTickCount64();
			m_tFrame.dwFrameSpeed = 40;
			break;

		case RIGHT_ATTACK:
			m_tFrame.iStart = 0;
			m_tFrame.iEnd = 6;
			m_tFrame.iMotion = 0;

			m_tSpriteInfo.fCX = 120;
			m_tSpriteInfo.fCY = 82;

			m_tFrame.dwTime = GetTickCount64();
			m_tFrame.dwFrameSpeed = 40;
			break;

		case LEFT_WALLGRAP:
			//왼쪽 갈떈 end와 start 같이 줘야함
			m_tFrame.iEnd = 0;
			m_tFrame.iStart = 0;
			// motion 값이 1이면 업데이트 거꾸로 돌아감.
			// 왼쪽일떄 motion 값 1
			m_tFrame.iMotion = 0;

			m_tSpriteInfo.fCX = 48;
			m_tSpriteInfo.fCY = 96;

			m_tFrame.dwTime = GetTickCount64();
			m_tFrame.dwFrameSpeed = 200;
			break;

		case RIGHT_WALLGRAP:
			m_tFrame.iStart = 0;
			m_tFrame.iEnd = 0;
			m_tFrame.iMotion = 1;

			m_tSpriteInfo.fCX = 48;
			m_tSpriteInfo.fCY = 96;

			m_tFrame.dwTime = GetTickCount64();
			m_tFrame.dwFrameSpeed = 200;
			break;

		case LEFT_WALLJUMP:
			//왼쪽 갈떈 end와 start 같이 줘야함
			m_tFrame.iEnd = 10;
			m_tFrame.iStart = 10;
			// motion 값이 1이면 업데이트 거꾸로 돌아감.
			// 왼쪽일떄 motion 값 1
			m_tFrame.iMotion = 1;

			m_tSpriteInfo.fCX = 96;
			m_tSpriteInfo.fCY = 88;

			m_tFrame.dwTime = GetTickCount64();
			m_tFrame.dwFrameSpeed = 50;
			break;

		case RIGHT_WALLJUMP:
			m_tFrame.iStart = 0;
			m_tFrame.iEnd = 10;
			m_tFrame.iMotion = 0;

			m_tSpriteInfo.fCX = 96;
			m_tSpriteInfo.fCY = 88;

			m_tFrame.dwTime = GetTickCount64();
			m_tFrame.dwFrameSpeed = 50;
			break;

		case LEFT_DEATH_JUMP:
			//왼쪽 갈떈 end와 start 같이 줘야함
			m_tFrame.iEnd = 3;
			m_tFrame.iStart = 3;
			// motion 값이 1이면 업데이트 거꾸로 돌아감.
			// 왼쪽일떄 motion 값 1
			m_tFrame.iMotion = 1;

			m_tSpriteInfo.fCX = 100;
			m_tSpriteInfo.fCY = 66;

			m_tFrame.dwTime = GetTickCount64();
			m_tFrame.dwFrameSpeed = 50;
			break;

		case RIGHT_DEATH_JUMP:
			m_tFrame.iStart = 0;
			m_tFrame.iEnd = 3;
			m_tFrame.iMotion = 0;

			m_tSpriteInfo.fCX = 100;
			m_tSpriteInfo.fCY = 66;

			m_tFrame.dwTime = GetTickCount64();
			m_tFrame.dwFrameSpeed = 50;
			break;

		case LEFT_DEATH_GROUND:
			//왼쪽 갈떈 end와 start 같이 줘야함
			m_tFrame.iEnd = 5;
			m_tFrame.iStart = 5;
			// motion 값이 1이면 업데이트 거꾸로 돌아감.
			// 왼쪽일떄 motion 값 1
			m_tFrame.iMotion = 1;

			m_tSpriteInfo.fCX = 114;
			m_tSpriteInfo.fCY = 46;

			m_tFrame.dwTime = GetTickCount64();
			m_tFrame.dwFrameSpeed = 50;
			break;

		case RIGHT_DEATH_GROUND:
			m_tFrame.iStart = 0;
			m_tFrame.iEnd = 5;
			m_tFrame.iMotion = 0;

			m_tSpriteInfo.fCX = 114;
			m_tSpriteInfo.fCY = 46;

			m_tFrame.dwTime = GetTickCount64();
			m_tFrame.dwFrameSpeed = 50;
			break;

		case DANCE:
			m_tFrame.iStart = 0;
			m_tFrame.iEnd = 11;
			m_tFrame.iMotion = 0;

			m_tSpriteInfo.fCX = 68;
			m_tSpriteInfo.fCY = 86;

			m_tFrame.dwTime = GetTickCount64();
			m_tFrame.dwFrameSpeed = 75;
			break;

		case DRAW_SWORD:
			m_tFrame.iStart = 0;
			m_tFrame.iEnd = 18;
			m_tFrame.iMotion = 0;

			m_tSpriteInfo.fCX = 100;
			m_tSpriteInfo.fCY = 82;

			m_tFrame.dwTime = GetTickCount64();
			m_tFrame.dwFrameSpeed = 400;
			break;
		}

		m_ePreMotion = m_eCurMotion;
	}

}

void CPlayer::HitActor(CObj* HitActor, HITPOINT _HitInfo)
{
}

void CPlayer::HitActor(CMap* HitActor, HITPOINT _HitInfo)
{
	// 다음 스테이지로 넘어가는 로직을 작성해야 하는데..
	// 이건 뭐 씬 매니저에서 ㅇㅇ..
	if (HitActor->GetObjType() == TYPE_PORTAL)
		g_bStageClear = true;
	// 레이저 (선 충돌)
	if (HitActor->GetObjType() == TYPE_LASER && HitActor->Get_Dead() == false) 
	{
		if (m_bInvincible) return;

		if (m_eCurMotion != LEFT_ROLL && m_eCurMotion != RIGHT_ROLL && !m_bDead)
		{
			CEffect* pEffect = new CRemainEffect(m_tInfo.fX, m_tInfo.fY, L"Explosion");
			pEffect->Initialize();
			CEffectMgr::Get_Instance()->Add_Front_Effect(pEffect);

			CSoundMgr::Get_Instance()->PlaySound(L"player_die.wav", SOUND_EFFECT, g_fVolume);
			if(!g_bMonoChrome)
				m_bDead = true;
			g_bSlowMod = false;
			g_fSlowFactor = 1.0f;
		}
	}
	// 바닥 ((선 충돌)).
	if (HitActor->GetObjType() == TYPE_GROUND) 
	{
		m_fTime = 0.f;

		m_fJumpSpeed = 0.f;
		m_fAngle = 0.f;
		m_iAttackCount = 0;

		m_bJump = false;
		m_bFallFlag = false;
		m_bWallGrap = false;
		CSoundMgr::Get_Instance()->StopSound(SOUND_WALLGRAP);
		m_bAttackFloat = false;
		m_bOnGround = true;

		if (m_bDead)
		{
			if ((m_eCurMotion == LEFT_DEATH_JUMP || m_eCurMotion == RIGHT_DEATH_JUMP) && !m_bDeadFlag)
			{
				m_bDeadFlag = true;
				m_eCurMotion = m_tFrame.iMotion == 0 ? RIGHT_DEATH_GROUND : LEFT_DEATH_GROUND;
				m_pFrameKey = L"Player_Death_Ground";
				Motion_Change();
			}
			else if (m_eCurMotion == LEFT_DEATH_GROUND || m_eCurMotion == RIGHT_DEATH_GROUND)
			{
				m_fDeathTime = 4.2f;
			}
		}

		else
		{
			if (m_eCurMotion == LEFT_JUMP || m_eCurMotion == RIGHT_JUMP)
			{
				CSoundMgr::Get_Instance()->PlaySound(L"player_land.wav", SOUND_EFFECT, g_fVolume);

				CEffect* pEffect = new CRemainEffect(m_tInfo.fX, m_tInfo.fY, L"Player_Landdust");
				pEffect->Initialize();
				CEffectMgr::Get_Instance()->Add_Effect(pEffect);
			}

			if (m_eCurMotion != LEFT_RUN && m_eCurMotion != RIGHT_RUN &&
				m_eCurMotion != LEFT_RUN_TO_IDLE && m_eCurMotion != RIGHT_RUN_TO_IDLE &&
				m_eCurMotion != LEFT_IDLE_TO_RUN && m_eCurMotion != RIGHT_IDLE_TO_RUN &&
				m_eCurMotion != LEFT_ROLL && m_eCurMotion != RIGHT_ROLL &&
				m_eCurMotion != LEFT_ATTACK && m_eCurMotion != RIGHT_ATTACK &&
				m_eCurMotion != DRAW_SWORD)
			{
				m_pFrameKey = L"Player_Idle";
				m_eCurMotion = IDLE;
				Motion_Change();
			}

			m_bCollided = true;
		}
	}
	
	if (HitActor->GetObjType() == TYPE_WALL) // 렉트 벽 충돌
	{
		if (_HitInfo.eDirection == DIRECTION::DIR_UP)
		{
			m_tInfo.fY += fabsf(_HitInfo.fY);
			m_bFallFlag = true;
			m_bJump = true;
			m_fTime = 10.0f;
		}

		if (_HitInfo.eDirection == DIRECTION::DIR_LEFT)
		{
			m_tInfo.fX += fabsf(_HitInfo.fX);

			if (m_eCurMotion != IDLE && !m_bDead)
			{
				if (m_eCurMotion == LEFT_WALLJUMP || m_eCurMotion == RIGHT_WALLJUMP)
					m_fTime = 2.9f;

				m_pFrameKey = L"Player_WallGrap";
				m_eCurMotion = RIGHT_WALLGRAP;

				if (!m_bWallGrap)
				{
					m_tInfo.fY -= 3.f;
					m_bWallGrap = true;
				}

				m_bAttackFloat = false;
				Motion_Change();
			}
		}

		if (_HitInfo.eDirection == DIRECTION::DIR_RIGHT)
		{
			m_tInfo.fX -= fabsf(_HitInfo.fX);

			if (m_eCurMotion != IDLE)
			{
				if (m_eCurMotion == LEFT_WALLJUMP || m_eCurMotion == RIGHT_WALLJUMP)
					m_fTime = 2.9f;

				m_pFrameKey = L"Player_WallGrap";
				m_eCurMotion = LEFT_WALLGRAP;

				if (!m_bWallGrap)
				{
					m_tInfo.fY -= 3.f;
					m_bWallGrap = true;
				}

				m_bAttackFloat = false;
				Motion_Change();
			}
		}
		m_bCollided = true;
	}
}

//히트박스, 총알과의 충돌. 여기서 플레이어는 사망해야 한다.
void CPlayer::HitActor(CObj* HitActor)
{
	if (m_bInvincible) return;

	if (HitActor->Get_Type() == BULLET && dynamic_cast<CBullet*>(HitActor)->Get_Parried() == true)
		return;

	if ((m_eCurMotion != LEFT_ROLL && m_eCurMotion != RIGHT_ROLL) && !m_bDead && ((HitActor->Get_Type() == BULLET && dynamic_cast<CBullet*>(HitActor)->Get_Dead() == false) || HitActor->Get_Type() == OBJ))
	{
		CSoundMgr::Get_Instance()->PlaySound(L"player_die.wav", SOUND_EFFECT, g_fVolume);
		if(!g_bMonoChrome)
			m_bDead = true;
		g_bSlowMod = false;
		g_fSlowFactor = 1.0f;
	}

	if (HitActor->Get_Type() == FANBLADE && (HitActor->Get_Frame().iStart < 12 || HitActor->Get_Frame().iStart > 24) && !m_bDead)
	{
		CSoundMgr::Get_Instance()->PlaySound(L"player_die.wav", SOUND_EFFECT, g_fVolume);
		m_bDead = true;
		g_bSlowMod = false;
		g_fSlowFactor = 1.0f;
	}

	if ((HitActor->Get_Type() == ITEM))
	{
		if (dynamic_cast<CItem*>(HitActor)->Get_Throw() == false && (m_pHand == nullptr))
		{
			HitActor->Set_Dead(false);
			CEffect* pEffect = nullptr;
			pEffect = new CFollowEffect(m_tInfo.fX, m_tInfo.fY - 20, L"Butcher_Knife_Effect", this);
			pEffect->Initialize();
			CEffectMgr::Get_Instance()->Add_Front_Effect(pEffect);
			CSoundMgr::Get_Instance()->PlaySound(L"item_pick.wav", SOUND_EFFECT, g_fVolume);
			CObjMgr::Get_Instance()->Delete_Object(OBJ_ITEM, HitActor);
			m_pHand = HitActor;
		}
	}
}
