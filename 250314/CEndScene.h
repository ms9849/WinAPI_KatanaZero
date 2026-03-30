#pragma once
#include "Cscene.h"

class CEndScene : public CScene
{
public:
	CEndScene();
	virtual ~CEndScene();

public:
	void Initialize() override;
	void Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

private:
	ULONGLONG m_fFirstImageTimer;
	ULONGLONG m_fSecondImageTimer;
};

