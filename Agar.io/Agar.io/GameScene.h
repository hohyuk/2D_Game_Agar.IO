#pragma once
#include "Scene.h"
class CGameObj;
class CPlayer;
class CItem;
class CVirus;
class CTrap;

class CGameScene:public CScene
{
	CPlayer* pPlayer[PLAYER_NUM]{ nullptr };
	CItem* pItem[ITEM_ALLNUM]{ nullptr };
	CVirus* pVirus[VIRUS_ALLNUM]{ nullptr };
	CTrap* pTrap[TRAP_NUM]{ nullptr };
	// Count
	int playerCount;
	int ItemCount;
	int ItemEatCount;
	int VirusCount;
	int TrapCount;
	// Time
	float fItemCreateTime;
	float fItemDelTime;
	float fVirusCreateTime;
	float fTrapCreateTime;
	// Font
	float m_fSize;

	// 분열
	bool bPlayerDivision;
	// Game UI
	bool bGameStart;
public:
	CGameScene(CGameFramework * ptFW);
	virtual ~CGameScene();

	virtual void Create(HWND hWnd) override;
	virtual void Draw(HDC hdc) override;
	virtual void Update(const float ftime) override;
	virtual void Message(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
	virtual void Release() override;

private:
	void RectBoard(HDC hdc) const;
	void FontDraw(HDC hdc)const;
	void Timer(const float ftime);
	void CreateItem();
	void CreateVirus();
	void CreateTrap();
	void DeleteItem();
	void ItemCollision(const float ftime, CPlayer*& p);
	void ItemOut();
	void VirusCollision(CPlayer*& p);
	void PlayerCollision(CPlayer*& p);
	void VirusAndVirusCollision(CVirus*& v);
	void VirusAI(CVirus* v);
	void TrapCollision(CPlayer*& p, int num);		// 플레이어와 트랩 충돌
	void TrapCollision(CVirus*& p, int num);		// 바이러스와 트랩 충돌
	void DeleteObj();
	void PlayerList(int num);
	void VirusList(int num);
};

