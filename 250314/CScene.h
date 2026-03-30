#pragma once
#include "Define.h"
#include "CObjMgr.h"
#include "CSoundMgr.h"

class CScene abstract
{
public:
	CScene() : m_fStageStartTime(GetTickCount64()){};
	virtual ~CScene() {};

public:
	ULONGLONG Get_StageStartTime() { return m_fStageStartTime; }
	void IsStageLimitOver() 
	{ 
		if (GetTickCount64() - m_fStageStartTime >= g_fStageLimit)
		{
			if (CObjMgr::Get_Instance()->Get_Player()->Get_Dead() == false)
			{
				CObjMgr::Get_Instance()->Get_Player()->Set_Dead(true);
				CSoundMgr::Get_Instance()->PlaySound(L"player_die.wav", SOUND_EFFECT, g_fVolume);
			}
		}
	}

public:
	virtual void Initialize()PURE;
	virtual void Update()PURE;
	virtual void Late_Update()PURE;
	virtual void Render(HDC hDC)PURE;
	virtual void Release()PURE;

protected:
	ULONGLONG m_fStageStartTime;
};

