#pragma once
#include "CObj.h"
class CButton : public CObj
{
public:
	CButton();
	~CButton();

public:	
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

private:
	int m_iDrawId;
};

