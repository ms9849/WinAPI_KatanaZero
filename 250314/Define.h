#pragma once

#define WINCX 800
#define WINCY 600

#define	PI		3.141592f
#define PURE		= 0

#define	NOEVENT		0
#define DEAD		1

#define VK_MAX		0xff

extern HWND    g_hWnd;

extern bool g_bDark;
extern bool	g_bSlowMod; // 슬로우 모드 체크용
extern float g_fSlowFactor; // 슬로우 모드 배속
extern bool g_bGameOver; //게임 오버인지 아닌지
extern bool g_bGameClear;
extern bool g_bReplaying; // 리플레이 중인지
extern float g_fVolume; // 사운드 볼륨. 추후 배경음악은 분리해야 할 필요성이 있다.
extern bool g_bStageClear; // 스테이지 클리어
extern bool g_bMonoChrome; // 게임 클리어를 확인하는 변수. 이걸로 흑백 전환도 제어 한다.
extern int g_iStageReplayCount; // 스테이지 리플 몇번했는지 확인
extern int g_iCurStage;
extern ULONGLONG g_fStageLimit; // 스테이지 제한 시간 ( 모든 스테이지가 공유한다. )

enum OBJID { OBJ_FANBLADE, OBJ_MONSTER, OBJ_PLAYER, OBJ_ITEM, OBJ_BULLET, OBJ_HEAD, OBJ_BUTTON, OBJ_FANBLADE_FG, OBJ_MOUSE,
	OBJ_MONSTER_HITBOX, OBJ_PLAYER_HITBOX,
	OBJ_END };
// OBJ FOREGROUND는 가장 마지막에 렌더해야 한다..
// 여기엔 일단 Fan Fg 하나만 집어넣긴 함.
// OBJ_LASER 는 추후 레이저 구현할 때 사용할 계획.
enum DIRECTION { DIR_LEFT, DIR_UP, DIR_DOWN, DIR_RIGHT, DIR_END };
enum CHANNELID { SOUND_EFFECT, SOUND_EFEECT_LOOP, SOUND_BGM, SOUND_SLOW_MOTION, SOUND_WALLGRAP, SOUND_END };

enum MAP_OBJ_TYPE
{
	TYPE_WALL,
	TYPE_MONSTER_WALL, // 몬스터만 지나갈 수 없는 벽을 만듬.
	TYPE_GROUND,
	TYPE_CEILING,
	TYPE_PORTAL,
	TYPE_LASER,
	TYPE_END,
};
//wall과 ground를 구분할지는 모르겠지만.. 일단 Ceiling은 적용.

template<typename T>
void Safe_Delete(T& p)
{
	if (p)
	{
		delete p;
		p = nullptr;
	}
}

typedef struct HitInfo
{
	float fX, fY;
	DIRECTION eDirection;

	HitInfo(float _fX, float _fY, DIRECTION _Dir)
		:fX(_fX), fY(_fY), eDirection(_Dir) {
	}

} HITPOINT;

typedef struct tagInfo
{
	float	fX;
	float	fY;
	float	fCX;
	float	fCY;

} INFO;

typedef struct tagLinePoint
{
	tagLinePoint() { ZeroMemory(this, sizeof(tagLinePoint)); }
	tagLinePoint(float _fX, float _fY)
		:fX(_fX), fY(_fY) {
	}

	float		fX, fY;

} LINEPOINT;

typedef struct tagFrame
{
	short iStart; // 몇번 인덱스부터 출력
	short iEnd; // 몇번 인덱스까지 출력
	short iMotion; // 현재 출력할 모션( 몇번 모션을 출력 )
	ULONGLONG dwFrameSpeed; // 애니메이션이 도는 속도
	ULONGLONG dwTime; // 애니메이션 재생 시간 체크를 위한 변수. 딱히, 의미는 없다.
} FRAME;