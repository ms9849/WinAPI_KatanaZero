#pragma once
#include "Define.h"

//UI ∫£¿ÃΩ∫
class CUI
{
public:
	CUI();
	virtual ~CUI();

public:
	virtual void Initialize() PURE;
	virtual void Update() PURE;
	virtual void Late_Update() PURE;
	virtual void Render(HDC hDC) PURE;
	virtual void Release() PURE;
};

