#pragma once
#include "CScene.h"
#include "CPlayer.h"
#include "CGrunt.h"
#include "CBold.h"
#include "CHeadHunter.h"
#include "CHUD.h"
#include "CMouse.h"

class CStage2 : public CScene
{
public:
	CStage2();
	virtual ~CStage2();

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

