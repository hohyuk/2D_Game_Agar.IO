#include "stdafx.h"
#include "GameObj.h"


CGameObj::CGameObj()
{
}


CGameObj::~CGameObj()
{
}

void CGameObj::Draw(HDC hdc)
{
	hPen = CreatePen(PS_SOLID, 2, pColor);
	oldPen = (HPEN)SelectObject(hdc, hPen);
	hBrush = CreateSolidBrush(bColor);
	oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	Ellipse(hdc, int(m_pos.x - m_scale), int(m_pos.y - m_scale), int(m_pos.x + m_scale), int(m_pos.y + m_scale));
	SelectObject(hdc, oldBrush);
	DeleteObject(hBrush);
	SelectObject(hdc, oldPen);
	DeleteObject(hPen);

	if (bScale)
	{
		HPEN newpen = CreatePen(PS_SOLID, 2, RGB(192, 192, 192));
		HPEN oldpen = (HPEN)SelectObject(hdc, newpen);
		auto old = SelectObject(hdc, GetStockObject(NULL_BRUSH));
		Ellipse(hdc, int(m_pos.x - m_size), int(m_pos.y - m_size), int(m_pos.x + m_size), int(m_pos.y + m_size));
		SelectObject(hdc, old);
		SelectObject(hdc, oldpen);
		DeleteObject(newpen);
	}
}

void CGameObj::RestrictedArea()
{
	if (m_pos.x < m_size)
		m_pos.x = m_size;
	else if (m_pos.x > CLIENT_WIDTH - m_size)
		m_pos.x = CLIENT_WIDTH - m_size;
	if (m_pos.y < m_size)
		m_pos.y = m_size;
	else if (m_pos.y > CLIENT_HEIGHT - m_size)
		m_pos.y = CLIENT_HEIGHT - m_size;
}

void CGameObj::SizeChange(const float ftime)
{
	if (m_size >= MAX_SIZE)
	{
		m_size = m_scale = MAX_SIZE;
		return;
	}

	if (bScale)
		EffectTime += ftime;
	if (EffectTime > 0.5f)
	{
		bScale = false;
		EffectTime = 0.f;
	}

	if (m_size > m_scale)
	{
		bScale = true;
		m_scale += ftime * 50;
		if (m_scale > m_size)
			m_scale = m_size;
	}
	else if (m_size < m_scale)
	{
		bScale = true;
		m_scale -= ftime * 50;
		if (m_scale < m_size)
			m_scale = m_size;
	}
}
