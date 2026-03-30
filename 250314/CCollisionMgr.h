#pragma once

#include "CObj.h"
#include "CLine.h"
#include "CRect.h"

class CCollisionMgr
{
public:
	static void Collision_Rect(list<CObj*> DstList, list<CObj*> SrcList);
	static void	Collision_RectEx(list<CObj*> DstList, list<CObj*> SrcList);
	static bool Check_Rect(CObj* pDst, CObj* pSrc, float* pWidth, float* pHeight);

	static void Collision_StaticRect(list<CObj*> DstList, list<CRect*> SrcList);
	static bool Check_RectToObj(CObj* pDst, CRect* pSrc, INFO* psrcInfo, float* pWidth, float* pHeight);
	
	static void Collision_StaticLine(list<CObj*> DstList, list<CLine*> SrcList);
	static bool Check_LineToObj(CObj* pDst, CLine* pSrc, float* pWidth, float* pHeight);

	static bool Check_PlayerUnderLine(list<CObj*> DstList, list<CLine*> SrcList);
};

