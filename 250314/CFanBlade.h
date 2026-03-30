#pragma once
#include "CObj.h"

class CFanBlade : public CObj
{
public:
	CFanBlade();
	~CFanBlade();

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

public:
	virtual OBJTYPE Get_Type() const { return FANBLADE; };

private:
};

