#pragma once
#include "CUI.h"

class CRestartUI : public CUI
{
public:
	CRestartUI();
	virtual ~CRestartUI();

public:
	void Initialize() override;
	void Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
};

