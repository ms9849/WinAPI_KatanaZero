#pragma once

#include "CObj.h"
#include "Define.h"

class CObjMgr
{
private:
	CObjMgr();
	CObjMgr(const CObjMgr& rhs) = delete;
	CObjMgr& operator=(CObj& rObj) = delete;
	~CObjMgr();
	
public:
	CObj* Get_Player() { return m_ObjList[OBJ_PLAYER].front(); }
	list<CObj*> Get_Playerlist() { return m_ObjList[OBJ_PLAYER]; }
	list<CObj*> Get_Monsterlist() { return m_ObjList[OBJ_MONSTER]; }
	list<CObj*>* Get_Playerlist_Ref() { return &m_ObjList[OBJ_PLAYER]; }
	list<CObj*>* Get_Monsterlist_Ref() { return &m_ObjList[OBJ_MONSTER]; }
	CObj* Get_Mouse() { return m_ObjList[OBJ_MOUSE].front(); }

public:
	void		Add_Object(OBJID eID, CObj* pObj);
	void		Update();
	void		Late_Update();
	void		Render(HDC hDC);
	void		Release();

	void		Delete_Object(OBJID eID);
	void		Delete_Object(OBJID eID, CObj* _pTarget);
	void		SlowMod_Filter(HDC hDC);
	void		MonoChrome_Filter(HDC hDC);
	void		Replay_Filter(HDC hDC);

public:
	static CObjMgr* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CObjMgr;

		return m_pInstance;
	}

	static void	Destroy_Instance()
	{
		if (nullptr != m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static CObjMgr* m_pInstance;
	list<CObj*>		m_ObjList[OBJ_END];

};

