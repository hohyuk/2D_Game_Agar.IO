#include "stdafx.h"
#include "LogoScene.h"
#include"GameFramework.h"
#include "BitMap.h"

CLogoScene::CLogoScene(CGameFramework * ptFW)
	:CScene(ptFW)
{
}

CLogoScene::~CLogoScene()
{
}

void CLogoScene::Create(HWND hWnd)
{
	m_MapBmp["GameOver"] = (new CBitMap)->LoadBmp(hWnd, (PTCHAR)L"../Texture/GameOver.bmp");
}

void CLogoScene::Draw(HDC hdc)
{
	BitBlt(hdc, 0, 0, CLIENT_WIDTH, CLIENT_HEIGHT, m_MapBmp["GameOver"]->GetMemDC(), 0, 0, SRCCOPY);
	
	HFONT hFont, oldFont;
	RECT rect{ 50,500,400,560 };
	wchar_t wcTmp[128];
	hFont = CreateFont(50, 0, 0, 0, 800, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, TEXT("Arial Rounded MT Bold"));
	oldFont = (HFONT)SelectObject(hdc, hFont);
	SetBkMode(hdc, TRANSPARENT);		// ¹ÙÅÁ Åõ¸í
										// Time
	swprintf(wcTmp, 128, L"Time : %.2f 'S", m_pGameFramework->ElapsedTime);
	SetTextColor(hdc, RGB(255, 200, 0));
	DrawText(hdc, wcTmp, -1, &rect, NULL);
	// Size
	rect.left += 400, rect.right += 400;
	swprintf(wcTmp, 128, L"Size : %.2f", m_pGameFramework->PlayerSize);
	DrawText(hdc, wcTmp, -1, &rect, NULL);
	SelectObject(hdc, oldFont);
	DeleteObject(hFont);
}

void CLogoScene::Update(const float ftime)
{
}

void CLogoScene::Message(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (wParam == 'q')
		PostQuitMessage(0);
	else if (wParam == 'i')
		m_pGameFramework->ChangeScene(ENUM_SCENEID::GAMESCENE);
}

void CLogoScene::Release()
{
}
