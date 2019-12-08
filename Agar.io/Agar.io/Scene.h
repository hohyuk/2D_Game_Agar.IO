#pragma once
#include "stdafx.h"

class CGameFramework;
class CScene
{
protected:
	CGameFramework * m_pGameFramework;
public:
	CScene(CGameFramework * ptFW) :m_pGameFramework(ptFW) {};
	virtual ~CScene() {};

	virtual void Create(HWND hWnd) = 0;
	virtual void Draw(HDC hdc) = 0;
	virtual void Update(const float ftime) = 0;
	virtual void Message(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;
	virtual void Release() = 0;
};