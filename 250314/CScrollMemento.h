#pragma once

#include "Define.h"

class CScrollMgr;

class CScrollMemento
{
public:
	friend class CScrollMgr;

public:
	CScrollMemento(float _fScrollX, float _fScrollY) { m_fScrollX = _fScrollX; m_fScrollY = _fScrollY; }

private:
	int m_fScrollX;
	int m_fScrollY;
};

