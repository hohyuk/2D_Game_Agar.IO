#include "stdafx.h"
#include "GameScene.h"
#include"GameFramework.h"
#include "GameObj.h"
#include "Player.h"
#include "Item.h"
#include "Virus.h"
#include "Trap.h"

CGameScene::CGameScene(CGameFramework * ptFW)
	:CScene(ptFW)
{
}

CGameScene::~CGameScene()
{
	Release();
}

void CGameScene::Create(HWND hWnd)
{
	// Player
	playerCount = 0;
	pPlayer[playerCount++] = new CPlayer();
	ItemEatCount = 0;
	// Item
	ItemCount = 0;
	fItemCreateTime = 0;
	fItemDelTime = 0;
	CreateItem();
	// Virus
	VirusCount = 0;
	fVirusCreateTime = 0.f;
	// Trap
	TrapCount = 0;
	fTrapCreateTime = 0.f;
	while (TrapCount < 3) pTrap[TrapCount++] = new CTrap();
	// Font
	m_pGameFramework->PlayerSize = m_fSize = NOMALSIZE;
	m_pGameFramework->ElapsedTime = 0;

	bPlayerDivision = false;
	bGameStart = true;
}

void CGameScene::Draw(HDC hdc)
{
	RectBoard(hdc);

	for (int i = 0; i < ITEM_ALLNUM; ++i)
		if (pItem[i] != nullptr)
			pItem[i]->Draw(hdc);

	for (int i = 0; i < VIRUS_ALLNUM; ++i)
		if (pVirus[i] != nullptr)
			pVirus[i]->Draw(hdc);

	for (int i = 0; i < PLAYER_NUM; ++i)
		if (pPlayer[i] != nullptr)
			pPlayer[i]->Draw(hdc);

	for (int i = 0; i < TRAP_NUM; ++i)
		if (pTrap[i] != nullptr)
			pTrap[i]->Draw(hdc);

	FontDraw(hdc);
}

void CGameScene::Update(const float ftime)
{
	if (bGameStart)
	{
		Timer(ftime);
		if (playerCount == 0)
		{
			m_pGameFramework->PlayerSize = m_fSize;
			m_pGameFramework->ChangeScene(ENUM_SCENEID::LOGOSCENE);
		}
		for (int i = 0; i < PLAYER_NUM; ++i)
		{
			if (pPlayer[i] != nullptr)
			{
				pPlayer[i]->Update(ftime);
				ItemCollision(ftime, pPlayer[i]);
				TrapCollision(pPlayer[i], i);
				VirusCollision(pPlayer[i]);
				if (pPlayer[i]->GetDelete())
				{
					PlayerList(i);
					break;
				}
				PlayerCollision(pPlayer[i]);
			}
		}

		for (int i = 0; i < VIRUS_ALLNUM; ++i)
		{
			if (pVirus[i] != nullptr)
			{
				if (pVirus[i]->GetTarget() == ENUM_TARGETID::NON_TARGET)
				{
					VirusAI(pVirus[i]);
				}
				else
				{
					pVirus[i]->Update(ftime);
					TrapCollision(pVirus[i], i);
					VirusAndVirusCollision(pVirus[i]);
					DeleteObj();
				}
			}
		}

		for (int i = 0; i < TRAP_NUM; ++i)
		{
			if (pTrap[i] != nullptr)
			{
				pTrap[i]->Update(ftime);
			}
		}
	}
}

void CGameScene::Message(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (wParam == 's')		// 멈춤과 시작
		bGameStart = !bGameStart;
	else if (wParam == 'i')
	{
		Release();
		Create(hWnd);
	}
	if (bGameStart)
	{
		switch (message)
		{
		case WM_LBUTTONDOWN:
		{
			int Count = playerCount;
			for (int i = 0; i < Count; ++i)
			{
				if (pPlayer[i] != nullptr)
				{
					if (pPlayer[i]->GetSize() >= 20 && playerCount < PLAYER_NUM - 1)
					{
						pPlayer[i]->SetSize(-pPlayer[i]->GetSize() / 2.f);
						pPlayer[playerCount++] = new CPlayer(pPlayer[i]->GetPos(), pPlayer[i]->GetDir(), pPlayer[i]->GetSize());
					}
				}
			}
		}
		break;
		case WM_RBUTTONDOWN:
			if (ItemEatCount > 0)
				ItemOut();
			break;
		case WM_MOUSEMOVE:
			for (int i = 0; i < PLAYER_NUM; ++i)
				if (pPlayer[i] != nullptr)
					pPlayer[i]->MouseMove(lParam);
			break;
		default:
			break;
		}
	}
}

void CGameScene::Release()
{
	for (int i = 0; i < PLAYER_NUM; ++i)
	{
		if (pPlayer[i] != nullptr)
		{
			delete pPlayer[i];
			pPlayer[i] = nullptr;
		}
	}

	for (int i = 0; i < VIRUS_ALLNUM; ++i)
	{
		if (pVirus[i] != nullptr)
		{
			delete pVirus[i];
			pVirus[i] = nullptr;
		}
	}

	for (int i = 0; i < ITEM_ALLNUM; ++i)
	{
		if (pItem[i] != nullptr)
		{
			delete pItem[i];
			pItem[i] = nullptr;
		}
	}

	for (int i = 0; i < TRAP_NUM; ++i)
	{
		if (pTrap[i] != nullptr)
		{
			delete pTrap[i];
			pTrap[i] = nullptr;
		}
	}
}

void CGameScene::RectBoard(HDC hdc) const
{
	HBRUSH hBrush, oldBrush;
	hBrush = CreateSolidBrush(RGB(180, 240, 225));
	oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	Rectangle(hdc, 0, 0, CLIENT_WIDTH, CLIENT_HEIGHT);
	SelectObject(hdc, oldBrush);
	DeleteObject(hBrush);

	HPEN hPen, oldPen;
	hPen = CreatePen(PS_SOLID, 2, RGB(60, 225, 180));
	oldPen = (HPEN)SelectObject(hdc, hPen);
	for (int i = 0; i < HEIGHT_LINE; ++i)
	{
		MoveToEx(hdc, 0, i * RECT_SIZE, NULL);
		LineTo(hdc, 0 + CLIENT_WIDTH, i * RECT_SIZE);
	}
	for (int i = 0; i < WIDTH_LINE; ++i)
	{
		MoveToEx(hdc, i * RECT_SIZE, 0, NULL);
		LineTo(hdc, i * RECT_SIZE, CLIENT_HEIGHT);
	}
	SelectObject(hdc, oldPen);
	DeleteObject(hPen);
}

void CGameScene::FontDraw(HDC hdc) const
{
	HFONT hFont, oldFont;
	RECT rect{ 0,0,300,30 };
	wchar_t wcTmp[128];
	hFont = CreateFont(25, 0, 0, 0, 800, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, TEXT("Arial Rounded MT Bold"));
	oldFont = (HFONT)SelectObject(hdc, hFont);
	SetBkMode(hdc, TRANSPARENT);		// 바탕 투명
										// Time
	swprintf(wcTmp, 128, L"Time : %.2f 'S", m_pGameFramework->ElapsedTime);
	SetTextColor(hdc, RGB(255, 100, 0));
	DrawText(hdc, wcTmp, -1, &rect, NULL);
	// Size
	rect.top += 30, rect.bottom += 30;
	swprintf(wcTmp, 128, L"Size : %.2f", m_fSize);
	DrawText(hdc, wcTmp, -1, &rect, NULL);
	SelectObject(hdc, oldFont);
	DeleteObject(hFont);
}

void CGameScene::Timer(const float ftime)
{
	m_pGameFramework->ElapsedTime += ftime;
	fItemCreateTime += ftime;
	fItemDelTime += ftime;
	fVirusCreateTime += ftime;
	fTrapCreateTime += ftime;
	if (fItemCreateTime > 3.0f) CreateItem();
	if (fItemDelTime > 1.5f)  DeleteItem();
	if (fVirusCreateTime > 4.0f) CreateVirus();
	if (fTrapCreateTime > 7.0f) CreateTrap();
}

void CGameScene::CreateItem()
{
	for (int i = 0; i < ITEM_NUM; ++i)
		if (pItem[i] == nullptr)
		{
			pItem[i] = new CItem();
			fItemCreateTime = 0.f;
		}
}

void CGameScene::CreateVirus()
{
	for (int i = 0; i < VIRUS_NUM; ++i)
	{
		if (pVirus[i] == nullptr)
		{
			pVirus[i] = new CVirus();
			fVirusCreateTime = 0.f;
			++VirusCount;
			break;
		}
	}
}

void CGameScene::CreateTrap()
{
	for (int i = 0; i < TRAP_NUM; ++i)
	{
		if (pTrap[i] == nullptr)
		{
			pTrap[i] = new CTrap();
			fTrapCreateTime = 0.f;
			++TrapCount;
			break;
		}
	}
}

void CGameScene::DeleteItem()
{
	if (ItemCount < ITEM_ALLNUM)
	{
		if (pItem[ItemCount] != nullptr)
		{
			delete pItem[ItemCount];
			pItem[ItemCount] = nullptr;
			++ItemCount;
			fItemDelTime = 0.f; // 삭제 시간 초기화
		}
		else
			++ItemCount;
	}
	else
		ItemCount = 0;
}

void CGameScene::ItemCollision(const float ftime, CPlayer *& p)
{
	for (int i = 0; i < ITEM_ALLNUM; ++i)
	{
		if (pItem[i] != nullptr && p != nullptr)
		{
			pItem[i]->Update(ftime);
			if (!pItem[i]->GetOut() && Collision(p->GetPos(), pItem[i]->GetPos(), p->GetSize() + pItem[i]->GetSize()))
			{
				p->SetSize(pItem[i]->GetSize() / 4.f);
				m_fSize += pItem[i]->GetSize() / 4.f;	// 점수 누적
				delete pItem[i];
				pItem[i] = nullptr;
				++ItemEatCount;
			}
		}
	}
}

void CGameScene::ItemOut()
{
	for (int i = 0; i < PLAYER_NUM; ++i)
	{
		for (int j = 0; j < ITEM_ALLNUM; ++j)
		{
			bool limit1 = pPlayer[i] != nullptr &&  pPlayer[i]->GetSize() > NOMALSIZE;
			bool limit2 = pItem[j]==nullptr;
			if (limit1&&limit2 )
			{
				pItem[j] = new CItem(Point{ pPlayer[i]->GetPos() + pPlayer[i]->GetDir() * pPlayer[i]->GetSize() }, pPlayer[i]->GetDir());
				pPlayer[i]->SetSize(-pItem[j]->GetSize() / 4.f);
				m_fSize -= pItem[i]->GetSize() / 4.f;	// 점수 감소
				--ItemEatCount;
				break;
			}
		}
	}
}

void CGameScene::VirusCollision(CPlayer *& p)
{
	for (int i = 0; i < VIRUS_ALLNUM; ++i)
	{
		if (pVirus[i] != nullptr && p != nullptr)
		{
			if (Collision(p->GetPos(), pVirus[i]->GetPos(), p->GetSize() + pVirus[i]->GetSize()))
			{
				if (p->GetSize() > pVirus[i]->GetSize())
				{
					p->SetSize(pVirus[i]->GetSize() / 4.f);
					VirusList(i);
					m_fSize += p->GetSize();		// 점수 누적
				}
				else
				{
					// Player Delete
					pVirus[i]->SetSize(p->GetSize() / 4.f);
					p->SetDelete(true);
				}
			}
		}
	}
}

void CGameScene::PlayerCollision(CPlayer *& p)
{
	for (int i = 0; i < PLAYER_NUM; ++i)
	{
		if (pPlayer[i] != nullptr && p!=nullptr)
		{
			bool limit = p->GetRespawn() && pPlayer[i]->GetRespawn() && (p != pPlayer[i]);
			if (limit)
			{
				if (Collision(p->GetPos(), pPlayer[i]->GetPos(), p->GetSize()))
				{
					// 두 플레이어끼리 크기 비교
					if (p < pPlayer[i])
						Swap(p, pPlayer[i]);
					p->SetSize(pPlayer[i]->GetSize());
					PlayerList(i);
					return;
				}
			}
		}
	}
}

void CGameScene::VirusAndVirusCollision(CVirus *& v)
{
	for (int i = 0; i < VIRUS_ALLNUM; ++i)
	{
		if (pVirus[i] != nullptr && v != nullptr)
		{
			bool limit = v->GetRespawn() && pVirus[i]->GetRespawn() && (v != pVirus[i]);
			if (limit)
			{
				if (Collision(v->GetPos(), pVirus[i]->GetPos(), v->GetSize()))
				{
					// 두 플레이어끼리 크기 비교
					if (v->GetSize() < pVirus[i]->GetSize())
						Swap(v, pVirus[i]);
					v->SetSize(pVirus[i]->GetSize());
					VirusList(i);
					return;
				}
			}
		}
	}
}

void CGameScene::VirusAI(CVirus * v)
{
	// 1. 플레이어 표적만들기.
	for (int i = 0; i < PLAYER_NUM; ++i)
	{
		if (pPlayer[i] != nullptr && v != nullptr)
		{
			if (v->GetSize() > pPlayer[i]->GetSize())
			{
				if (Length(v->GetPos() - pPlayer[i]->GetPos()) < PLAYER_RANGE)
				{
					v->SetPlayerInfo(pPlayer[i]);
					v->SetTarget(ENUM_TARGETID::PLAYER_TARGET);
					return;
				}
			}
		}
	}
	// 2. 바이러스 표적만들기.
	for (int i = 0; i < VIRUS_ALLNUM; ++i)
	{
		if (pVirus[i] != nullptr && v != nullptr && v!=pVirus[i])
		{
			if (v->GetSize() > pVirus[i]->GetSize())
			{
				if (Length(v->GetPos() - pVirus[i]->GetPos()) < VIRUS_RANGE)
				{
					v->SetVirusInfo(pVirus[i]);
					v->SetTarget(ENUM_TARGETID::VIRUSTARGET);
					return;
				}
			}
		}
	}
	// 3. 아이템 표적만들기.
	int targetSave = 0;
	float minLength = -1;	// 초기값을 -1을 주는 이유는 temp첫번째 값을 넣기위해서.(거리상 -1이 나올일이없다.)

	for (int i = 0; i < ITEM_ALLNUM; ++i)
	{
		if (pItem[i] != nullptr && v != nullptr)
		{
			// 아이템들과 바이러스의 최소값을 구한다.
			float temp = Length(v->GetPos() - pItem[i]->GetPos());
			if (minLength == -1 || minLength > temp)
			{
				minLength = temp;
				targetSave = i;
			}
		}
	}

	if (pItem[targetSave] != nullptr)
	{
		v->SetItemInfo(pItem[targetSave]);
		v->SetTarget(ENUM_TARGETID::ITEM_TARGET);
	}
}

void CGameScene::TrapCollision(CPlayer *& p, int num)
{
	for (int i = 0; i < TRAP_NUM; ++i)
	{
		if (pTrap[i] != nullptr && p!=nullptr)
		{
			if (Collision(p->GetPos(), pTrap[i]->GetPos(), p->GetSize() + pTrap[i]->GetSize()))
			{
				if (p->GetSize() > pTrap[i]->GetSize())
				{
					float size = p->GetSize() / 4.f;
					PlayerList(num);
					for (int j = 0; j < 5; ++j)
					{
						if (pPlayer[j] == nullptr && playerCount < PLAYER_NUM - 1)
						{
							Point dir = Point{ cosf(float(rand() % 360) * PI / 180), -sinf(float(rand() % 360) * PI / 180) };
							pPlayer[playerCount++] = new CPlayer(pTrap[i]->GetPos() + dir * size, dir, size);
						}
					}
					delete pTrap[i];
					pTrap[i] = nullptr;
				}
			}
		}
	}
}

void CGameScene::TrapCollision(CVirus *& v, int num)
{
	for (int i = 0; i < TRAP_NUM; ++i)
	{
		if (pTrap[i] != nullptr && v != nullptr)
		{
			if (Collision(v->GetPos(), pTrap[i]->GetPos(), v->GetSize() + pTrap[i]->GetSize()))
			{
				if (v->GetSize() > pTrap[i]->GetSize())
				{
					float size = v->GetSize() / 4.f;
					VirusList(num);
					int count = 0;
					
					for (int j = 0; j < VIRUS_ALLNUM; ++j)
					{
						if (pVirus[j] == nullptr && VirusCount < VIRUS_ALLNUM - 1 && count < 5)
						{
							Point dir = Point{ cosf(float(rand() % 360) * PI / 180), -sinf(float(rand() % 360) * PI / 180) };
							pVirus[VirusCount++] = new CVirus(pTrap[i]->GetPos() + dir * size, dir, size);
							++count;
						}
					}
					delete pTrap[i];
					pTrap[i] = nullptr;
				}
			}
		}
	}
}

void CGameScene::DeleteObj()
{
	for (int i = 0; i < PLAYER_NUM; ++i)
		if (pPlayer[i] != nullptr)
			if (pPlayer[i]->GetDelete())
			{
				m_fSize -= pPlayer[i]->GetSize() / 4.f;	// 점수 감소
				delete pPlayer[i];
				pPlayer[i] = nullptr;
				--playerCount;
			}
	for (int i = 0; i < ITEM_ALLNUM; ++i)
		if (pItem[i] != nullptr)
			if (pItem[i]->GetDelete())
			{
				delete pItem[i];
				pItem[i] = nullptr;
			}
	for (int i = 0; i < VIRUS_ALLNUM; ++i)
		if (pVirus[i] != nullptr)
			if (pVirus[i]->GetDelete())
			{
				delete pVirus[i];
				pVirus[i] = nullptr;
				--VirusCount;
			}
}

void CGameScene::PlayerList(int num)
{
	delete pPlayer[num];
	pPlayer[num] = nullptr;
	int temp = num;
	while (pPlayer[temp + 1] != nullptr)
	{
		Swap(pPlayer[temp], pPlayer[temp + 1]);
		++temp;
	}
	--playerCount;
}

void CGameScene::VirusList(int num)
{
	delete pVirus[num];
	pVirus[num] = nullptr;
	int temp = num;
	while (pVirus[temp + 1] != nullptr)
	{
		Swap(pVirus[temp], pVirus[temp + 1]);
		++temp;
	}
	--VirusCount;
}
