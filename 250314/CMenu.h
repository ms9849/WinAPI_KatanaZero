#pragma once
#include "CScene.h"
#include "Define.h"
#include "CButton.h"

class CMenu : public CScene
{
public:
	CMenu();
	virtual ~CMenu();

public:
	void Initialize() override;
	void Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

protected:

};

