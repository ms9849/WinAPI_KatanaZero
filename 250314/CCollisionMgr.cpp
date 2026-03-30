#include "pch.h"
#include "Define.h"
#include "CCollisionMgr.h"

void CCollisionMgr::Collision_Rect(list<CObj*> DstList, list<CObj*> SrcList)
{
	RECT rc{};

	if (DstList.empty() == true || SrcList.empty() == true) return;

	for (auto& Dst : DstList)
	{
		for (auto& Src : SrcList)
		{
			if (IntersectRect(&rc, Dst->Get_Rect(), Src->Get_Rect()))
			{
				Dst->HitActor(Src);
				Src->HitActor(Dst);
			}
		}
	}

}

void CCollisionMgr::Collision_RectEx(list<CObj*> DstList, list<CObj*> SrcList)
{
	float fWidth(0.f), fHeight(0.f);
	
	if (DstList.empty() || SrcList.empty()) return;
	for (auto& Dst : DstList)
	{
		for (auto& Src : SrcList)
		{
			if (Check_Rect(Dst, Src, &fWidth, &fHeight))
			{
				if (fWidth > fHeight)	// 상하 충돌
				{
					// 상 충돌
					if (Dst->Get_Info()->fY < Src->Get_Info()->fY)
					{
						Dst->HitActor(Src, HITPOINT(fWidth, fHeight, DIRECTION::DIR_DOWN));
						Src->HitActor(Dst, HITPOINT(fWidth, fHeight, DIRECTION::DIR_LEFT));
					}

					// 하 충돌
					else
					{
						Dst->HitActor(Src, HITPOINT(fWidth, fHeight, DIRECTION::DIR_UP));
						Src->HitActor(Dst, HITPOINT(fWidth, fHeight, DIRECTION::DIR_DOWN));
					}
				}

				else		// 좌우 충돌
				{
					// 좌 충돌
					if (Dst->Get_Info()->fX < Src->Get_Info()->fX)
					{
						Dst->HitActor(Src, HITPOINT(fWidth, fHeight, DIRECTION::DIR_RIGHT));
						Src->HitActor(Dst, HITPOINT(fWidth, fHeight, DIRECTION::DIR_LEFT));
					}

					// 우 충돌
					else
					{
						Dst->HitActor(Src, HITPOINT(fWidth, fHeight, DIRECTION::DIR_LEFT));
						Src->HitActor(Dst, HITPOINT(fWidth, fHeight, DIRECTION::DIR_RIGHT));
					}
				}
			}
		}
	}	
}

bool CCollisionMgr::Check_Rect(CObj* pDst, CObj* pSrc, float* pWidth, float* pHeight)
{
	float		fRWidth = fabsf(pDst->Get_Info()->fX - pSrc->Get_Info()->fX);
	float		fRHeight = fabsf(pDst->Get_Info()->fY - pSrc->Get_Info()->fY);

	float		fRadiusX = (pDst->Get_Info()->fCX + pSrc->Get_Info()->fCX) * 0.5f;
	float		fRadiusY = (pDst->Get_Info()->fCY + pSrc->Get_Info()->fCY) * 0.5f;

	if ((fRadiusX >= fRWidth) && (fRadiusY >= fRHeight))
	{
		*pWidth = fRadiusX - fRWidth;
		*pHeight = fRadiusY - fRHeight;

		return true;
	}

	return false;
}

void CCollisionMgr::Collision_StaticRect(list<CObj*> DstList, list<CRect*> SrcList)
{
	INFO srcInfo{};
	float fWidth(0.f), fHeight(0.f);

	if (DstList.empty() || SrcList.empty()) return;

	for (auto& Dst : DstList)
	{
		for (auto& Src : SrcList)
		{
			if (Check_RectToObj(Dst, Src, &srcInfo, &fWidth, &fHeight))
			{
				if (fWidth > fHeight)	// 상하 충돌
				{
					// 상 충돌
					if (Dst->Get_Info()->fY < srcInfo.fY)
					{
						Dst->HitActor(Src, HITPOINT(fWidth, fHeight, DIRECTION::DIR_DOWN ));
					}
					// 하 충돌
					else
					{
						Dst->HitActor(Src, HITPOINT(fWidth, fHeight, DIRECTION::DIR_UP));
					}
				}
				else
				{
					// 상 충돌
					if (Dst->Get_Info()->fX < srcInfo.fX)
					{
						Dst->HitActor(Src, HITPOINT(fWidth, fHeight, DIRECTION::DIR_RIGHT));
					}
					// 하 충돌
					else
					{
						Dst->HitActor(Src, HITPOINT(fWidth, fHeight, DIRECTION::DIR_LEFT));
					}
				}
			}
		}
	}
}

bool CCollisionMgr::Check_RectToObj(CObj* pDst, CRect* pSrc, INFO* psrcInfo, float* pWidth, float* pHeight)
{
	*psrcInfo = { pSrc->Get_Rect()->left + ((pSrc->Get_Rect()->right - pSrc->Get_Rect()->left) * 0.5f),
				  pSrc->Get_Rect()->top + ((pSrc->Get_Rect()->bottom - pSrc->Get_Rect()->top) * 0.5f),
				  float(pSrc->Get_Rect()->right - pSrc->Get_Rect()->left),
				  float(pSrc->Get_Rect()->bottom - pSrc->Get_Rect()->top) };

	float fRWidth = fabsf(pDst->Get_Info()->fX - psrcInfo->fX);
	float fRHeight = fabsf(pDst->Get_Info()->fY - psrcInfo->fY);

	float fRadiusX = (pDst->Get_Info()->fCX + psrcInfo->fCX) * 0.5f;
	float fRadiusY = (pDst->Get_Info()->fCY + psrcInfo->fCY) * 0.5f;

	if ((fRadiusX >= fRWidth) && (fRadiusY >= fRHeight))//(fRadiusX >= fRWidth) && )
	{
		*pWidth = fRadiusX - fRWidth;
		*pHeight = fRadiusY - fRHeight;
		return true;
	}

	return false;
}

void  CCollisionMgr::Collision_StaticLine(list<CObj*> DstList, list<CLine*> SrcList)
{
	float fWidth, fHeight;

	if (DstList.empty() || SrcList.empty()) return;

	for (auto& Dst : DstList)
	{
		for (auto& Src : SrcList)
		{
			if (Check_LineToObj(Dst, Src, &fWidth, &fHeight))
			{
				if (Src->GetObjType() == TYPE_LASER && fabsf(Src->Get_Left().fX - Src->Get_Right().fX) <= 7)
				{
					if (Dst->Get_Info()->fX >= (Src->Get_Left().fX - 7) && Dst->Get_Info()->fX <= (Src->Get_Right().fX + 7))
						Dst->HitActor(Src, HITPOINT(fWidth, fHeight, DIRECTION::DIR_UP));
				}
				if (((Dst->Get_Info()->fY + Dst->Get_Info()->fCY /2.f) > fHeight) && ((Dst->Get_Info()->fY + Dst->Get_Info()->fCY / 2.f) < fHeight + 50))
				{
					if(Src->GetObjType() != TYPE_LASER)
						Dst->Set_PosY(fHeight - Dst->Get_Info()->fCY / 2.f );

					Dst->HitActor(Src, HITPOINT(fWidth, fHeight, DIRECTION::DIR_UP));
				}

				// 플레이어가 아래에서 위로 박치기 했을때의 케이스긴 한데.. 당장은 쓸 데 없음.
				//else if ((Dst->Get_Info()->fY < (fHeight - 15)) && (Dst->Get_Info()->fY > (fHeight - 30)))
				//{
				//	Dst->HitActor(Src, HITPOINT(fWidth, fHeight, DIRECTION::DIR_DOWN));
				//}
			}
		}
	}
}

bool  CCollisionMgr::Check_LineToObj(CObj* pDst, CLine* pSrc, float* pWidth, float* pHeight)
{
	LINEPOINT lpLeft = pSrc->Get_Left();
	LINEPOINT lpRight = pSrc->Get_Right();

	//좌표만 체크해서, x좌표, y좌표 설정 후 넘겨줌.
	if (pDst->Get_Info()->fX >= lpLeft.fX && pDst->Get_Info()->fX <= lpRight.fX)
	{
		*pWidth = pDst->Get_Info()->fX;
		*pHeight = (lpLeft.fY - lpRight.fY) / (lpLeft.fX - lpRight.fX) * (pDst->Get_Info()->fX - lpLeft.fX) + lpLeft.fY;
		return true;
	}
	//x값의 차이가 5보다 작다면, 즉 기울기가 존나 크다면
	else if(pSrc->GetObjType() == TYPE_LASER && fabsf(lpLeft.fX - lpRight.fX) <= 7)
	{
		if (pDst->Get_Info()->fX >= (lpLeft.fX - 7) && pDst->Get_Info()->fX <= (lpRight.fX + 7))
		{
			*pWidth = pDst->Get_Info()->fX;
			*pHeight = (lpRight.fY - lpLeft.fY) / 2.f;
			return true;
		}
	}

	return false;
}

bool CCollisionMgr::Check_PlayerUnderLine(list<CObj*> DstList, list<CLine*> SrcList)
{
	float fWidth, fHeight;

	for (auto& Dst : DstList)
	{
		for (auto& Src : SrcList)
		{
			if (Src->GetObjType() != TYPE_LASER)
			{
				if (Check_LineToObj(Dst, Src, &fWidth, &fHeight))
				{
					if (((Dst->Get_Info()->fY + Dst->Get_Info()->fCY / 2.f) >= fHeight + 2) && (Dst->Get_Info()->fY + Dst->Get_Info()->fCY / 2.f) <= fHeight + 30)
						return true;
				}
			}
		}
	}

	return false;
}
