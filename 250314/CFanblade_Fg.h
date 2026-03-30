#pragma once
#include "CObj.h"

class CFanblade_Fg : public CObj
{
public:
	CFanblade_Fg();
	virtual ~CFanblade_Fg();

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
private:

};

