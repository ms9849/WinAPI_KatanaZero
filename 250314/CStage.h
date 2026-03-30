#pragma once
#include "CScene.h"
#include "CPlayer.h"
#include "CGrunt.h"
#include "CHUD.h"
#include "CHeadHunter.h"
#include "CMouse.h"
#include "CBold.h"
#include "CFanblade.h"
#include "CFanblade_Fg.h"
#include "CMonsterRect.h"

class CStage : public CScene
{
public:
	CStage();
	virtual ~CStage();

public:
	void Initialize() override;
	void Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

private:
	void Create_SnapShot();
	void Check_Clear();
	void Start_Replay();
	void Restart();

private:
	int m_iFadeCount;
	ULONGLONG m_fFadeOutTimer;

	int m_iCount;
	bool m_bCleared;
	bool m_bPortalCreated;
};

//남은 시간을 어디서 끌어서 띄워주고 써주지?..
