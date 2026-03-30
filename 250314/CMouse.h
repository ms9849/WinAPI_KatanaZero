#pragma once
#include "CObj.h"

class CMouse : public CObj
{
public:
	CMouse();
	virtual ~CMouse();

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

private:
};

