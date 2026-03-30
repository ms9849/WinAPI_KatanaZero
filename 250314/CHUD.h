#pragma once
#include "CUI.h"
#include "CObj.h"
#include "CPlayer.h"

class CHUD : public CUI
{
public:
	CHUD();
	~CHUD();
	
public:
	void Set_Player(CObj* _pPlayer) { m_pPlayer = _pPlayer; }

public:
	void Initialize() override;
	void Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

private:
	CObj* m_pPlayer;
};

