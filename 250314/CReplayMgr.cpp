#include "pch.h"
#include "CReplayMgr.h"

CReplayMgr* CReplayMgr::m_pInstance = nullptr;

CReplayMgr::CReplayMgr()
{
}

CReplayMgr::~CReplayMgr()
{
	Release();
}

void CReplayMgr::Release()
{
    for (int i = 0; i < OBJ_END; ++i)
    {
        auto& stackList = m_ObjMementoList[i];
        for (auto iter = stackList.begin(); iter != stackList.end(); )
        {
            stack<pair<CObjMemento*, int>>& mementoStack = *iter;
            while (!mementoStack.empty())
            {
                CObjMemento* pMemento = mementoStack.top().first;
                Safe_Delete(pMemento);
                mementoStack.pop();
            }

            iter = stackList.erase(iter); // 리스트 요소 자체도 제거
        }
    }

    while (m_ScrollMementoList.empty() == false)
    {
        CScrollMemento* pMemento = m_ScrollMementoList.top().first;
        Safe_Delete(pMemento); // 혹은 delete pMemento; 사용
        m_ScrollMementoList.pop();
    }
}
