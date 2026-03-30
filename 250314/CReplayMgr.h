#pragma once

#include "Define.h"
#include "CObj.h"
#include "CObjMemento.h"
#include "CScrollMgr.h"
#include "CScrollMemento.h"

class CReplayMgr
{
public:
	CReplayMgr();
	~CReplayMgr();

public:
	void Release();
public:
	void Add_Id() { m_iId += 1; }
	void Sub_Id() { m_iId -= 1; }
	int Get_Id() { return m_iId; }

public:
	void Set_ObjList(list<CObj*>* _ObjList, OBJID _eObjType)
	{
		m_ObjList[_eObjType] = _ObjList;
		m_IdList.push_back(_eObjType);

		if (!m_ObjList[_eObjType]->empty())
		{
			for (auto iter : *m_ObjList[_eObjType])
			{
				if (iter != nullptr)
					m_ObjMementoList[_eObjType].push_back(stack<pair<CObjMemento*, int>>{});
			}
		}
	}

	void Create_Snapshot()
	{
		for (auto i = m_IdList.begin(); i != m_IdList.end(); ++i)
		{
			if (m_ObjList[*i] != nullptr && !m_ObjList[*i]->empty())
			{
				while (m_ObjMementoList[*i].size() < m_ObjList[*i]->size())
				{
					m_ObjMementoList[*i].push_back(stack<pair<CObjMemento*, int>>{});
				}
			}

			for (auto& iter : m_ObjMementoList[*i])
			{
				for (auto& iter2 : *m_ObjList[*i])
				{
					iter.push(pair<CObjMemento*, int>({(*iter2).Create_SnapShot(), m_iId}));
				}
			}
		}

		m_ScrollMementoList.push(pair<CScrollMemento*, int>({ CScrollMgr::Get_Instance()->Create_SnapShot(), m_iId }));
	}

	void Rollback()
	{
		for (auto i = m_IdList.begin(); i != m_IdList.end(); ++i)
		{
			if (!m_ObjMementoList[*i].empty())
			{
				for (auto& iter : m_ObjMementoList[*i])
				{
					for (auto& iter2 : *m_ObjList[*i])
					{
						//복원 타이밍이 잘못됐음 맨 위에 있다고 같은 프레임에서 재생하는게 아니다..
						if (!iter.empty())
						{
							iter2->Rollback(iter.top().first);
							Safe_Delete(iter.top().first);
							iter.pop();
						}
					}
				}
			}
		}

		if (!m_ScrollMementoList.empty())
		{
			CScrollMgr::Get_Instance()->Rollback(m_ScrollMementoList.top().first);
			Safe_Delete(m_ScrollMementoList.top().first);
			m_ScrollMementoList.pop();
		}
	}

public:
	static CReplayMgr* Get_Instance()
	{
		if (m_pInstance == nullptr) m_pInstance = new CReplayMgr;
		return m_pInstance;
	}

	static void Destroy_Instance()
	{
		if (m_pInstance == nullptr) return;
		delete m_pInstance;
		m_pInstance = nullptr;
	}

private:
	static CReplayMgr* m_pInstance;
	int m_iId = 0;
	//PLAYER, MONSTER만 실험적으로 진행.
	list<CObj*>* m_ObjList[OBJ_END];
	list<OBJID> m_IdList;

	list<stack<pair<CObjMemento*, int>>> m_ObjMementoList[OBJ_END];
	stack<pair<CScrollMemento*, int>> m_ScrollMementoList;
};

