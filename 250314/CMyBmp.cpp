#include "pch.h"
#include "CMyBmp.h"

CMyBmp::CMyBmp()
{
}

CMyBmp::~CMyBmp()
{
	Release();
}

void CMyBmp::Load_Bmp(const TCHAR* pFilePath)
{
	HDC	hDC = GetDC(g_hWnd);

	// CreateCompatibleDC : 현재 dc와 호환되는 dc를 할당하는 함수

	m_hMemDC = CreateCompatibleDC(hDC);

	ReleaseDC(g_hWnd, hDC);

	m_hBitmap = (HBITMAP)LoadImage(NULL,		// 프로그램 인스턴스 핸들(파일로부터 읽어올 것이기 때문에 null)
									pFilePath,	// 파일의 경로
									IMAGE_BITMAP,	 // 어떤 타입의 파일인지 옵션 설정
									0, 0,			// 읽어들일 파일의 가로, 세로 사이즈(파일로 읽어들여온 크기 그대로를 출력할 것이기 때문에 0, 0)
									LR_LOADFROMFILE | LR_CREATEDIBSECTION);	// LR_LOADFROMFILE : 파일로부터 이미지 정보 불러오기, LR_CREATEDIBSECTION : DIB 형식으로 형태 변환

	// SelectObject : gdi 오브젝트를 선택하기 전에 기존에 가지고 있던 오브젝트를 반환
	m_hOldbmp = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);

}

void CMyBmp::Release()
{
	SelectObject(m_hMemDC, m_hOldbmp);
	DeleteObject(m_hBitmap);
	DeleteDC(m_hMemDC);

}
