#pragma once
#include "CObj.h"

template<typename T>
class CAbstractFactory
{
public:
	CAbstractFactory()  {}
	~CAbstractFactory() {}
public:
	static CObj* Create_Obj()
	{
		CObj* pTemp = new T;
		pTemp->Initialize();

		return pTemp;
	}

	static CObj* Create_Obj(float _fX, float _fY)
	{
		CObj* pTemp = new T;
		pTemp->Set_Pos(_fX, _fY);
		pTemp->Initialize();

		return pTemp;
	}
};

