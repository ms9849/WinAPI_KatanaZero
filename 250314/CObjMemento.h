#pragma once
#include "Define.h"

class CObj;

class CObjMemento 
{
public:
	friend class CObj;

public:
	CObjMemento(INFO _tSpriteInfo, FRAME _tFrame, const TCHAR* _pFrameKey) : m_tSpriteInfo(_tSpriteInfo), m_tFrame(_tFrame), m_pFrameKey(_pFrameKey) {}

private:
	INFO m_tSpriteInfo;
	FRAME		m_tFrame;
	const TCHAR* m_pFrameKey;
};

