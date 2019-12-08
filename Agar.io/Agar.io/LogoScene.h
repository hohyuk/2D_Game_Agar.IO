#pragma once
#include "Scene.h"
class CBitMap;
class CLogoScene : public CScene
{
	map<string, CBitMap*> m_MapBmp;
public:
	CLogoScene(CGameFramework * ptFW);
	virtual ~CLogoScene();

	virtual void Create(HWND hWnd) override;
	virtual void Draw(HDC hdc) override;
	virtual void Update(const float ftime) override;
	virtual void Message(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
	virtual void Release() override;
};