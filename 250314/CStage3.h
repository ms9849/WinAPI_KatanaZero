#pragma once
#include "CScene.h"
#include "CPlayer.h"
#include "CGrunt.h"
#include "CBold.h"
#include "CHeadHunter.h"
#include "CHUD.h"
#include "CMouse.h"

class CStage3 : public CScene
{
public:
	CStage3();
	virtual ~CStage3();

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
	void Platform_Bomb();
private:
	int m_iFadeCount;
	ULONGLONG m_fFadeOutTimer;

	int m_iCount;
	bool m_bBossDead;
	bool m_bMonsterSpawn;
	ULONGLONG m_fSpawnTimer;
	bool m_bMonsterDead;
	bool m_bExplode;

	ULONGLONG m_fBombTimer;
	int m_iBombCount;
	int m_iExplosionCount;
	bool m_bBombInstalled;
};

