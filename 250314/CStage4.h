#pragma once
#include "CScene.h"
#include "CPlayer.h"
#include "CGrunt.h"
#include "CHUD.h"
#include "CHeadHunter_2.h"
#include "CMouse.h"
#include "CBold.h"

class CStage4 : public CScene
{
public:
	CStage4();
	virtual ~CStage4();

public:
	void Initialize() override;
	void Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

public:
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

