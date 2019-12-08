#pragma once
#include "stdafx.h"
class CBitMap
{
private:
	HDC			m_hdc, m_MemDC;
	HBITMAP		m_bitMap, m_oldbitMap;
public:
	CBitMap();
	~CBitMap();

	CBitMap* LoadBmp(HWND hwnd, PTCHAR pFileName);
	void Release();

	HDC GetMemDC()const { return m_MemDC; }
};

