#include "stdafx.h"
#include "Trap.h"


CTrap::CTrap()
{
	Create();
}


CTrap::~CTrap()
{
}

void CTrap::Create()
{
	m_size = TRAP_SIZE + 10;
	m_scale = TRAP_SIZE - 15;
	bScale = false;
	pColor = RGB(0, 0, 0);
	bColor = RGB(rand() % 255, rand() % 255, rand() % 255);
	m_pos = Point{ (rand() % (CLIENT_WIDTH - int(m_size))) + m_size, (rand() % (CLIENT_HEIGHT - int(m_size))) + m_size };
	bAnim = true;
	m_fspeed = 150.f;
}

void CTrap::Draw(HDC hdc)
{
	if (bAnim)
		CreateDraw(hdc);
	else
	{
		// 큰 원
		SetROP2(hdc, R2_MASKPEN);
		hPen = CreatePen(PS_SOLID, 2, pColor);
		oldPen = (HPEN)SelectObject(hdc, hPen);
		hBrush = CreateHatchBrush(HS_DIAGCROSS, RGB(0, 0, 0));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		Ellipse(hdc, int(m_pos.x - m_size), int(m_pos.y - m_size), int(m_pos.x + m_size), int(m_pos.y + m_size));
		SelectObject(hdc, oldBrush);
		DeleteObject(hBrush);
		SelectObject(hdc, oldPen);
		DeleteObject(hPen);

		// 작은 원
		SetROP2(hdc, R2_COPYPEN);
		hPen = CreatePen(PS_SOLID, 2, pColor);
		oldPen = (HPEN)SelectObject(hdc, hPen);
		hBrush = CreateSolidBrush(RGB(0, 0, 0));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		Ellipse(hdc, int(m_pos.x - m_scale), int(m_pos.y - m_scale), int(m_pos.x + m_scale), int(m_pos.y + m_scale));
		SelectObject(hdc, oldBrush);
		DeleteObject(hBrush);
		SelectObject(hdc, oldPen);
		DeleteObject(hPen);
	}
}

void CTrap::Update(const float ftime)
{
	EffectTime += ftime;
	CGameObj::RestrictedArea();
	if(bAnim) bAnim = Animation(ftime);
	else
	{
		if (EffectTime > 2.f)
		{
			m_dir = Point{ cosf(float(rand() % 360) * PI / 180), -sinf(float(rand() % 360) * PI / 180) };
			EffectTime = 0.f;
		}
		m_pos += m_dir * m_fspeed * ftime;
	}
}

void CTrap::CreateDraw(HDC hdc)
{
	// 큰원
	hPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 210));
	oldPen = (HPEN)SelectObject(hdc, hPen);
	auto old = SelectObject(hdc, GetStockObject(NULL_BRUSH));
	Ellipse(hdc, int(m_pos.x - m_size), int(m_pos.y - m_size), int(m_pos.x + m_size), int(m_pos.y + m_size));
	SelectObject(hdc, old);
	SelectObject(hdc, oldPen);
	DeleteObject(hPen);

	// 작은 원
	hPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 210));
	oldPen = (HPEN)SelectObject(hdc, hPen);
	old = SelectObject(hdc, GetStockObject(NULL_BRUSH));
	Ellipse(hdc, int(m_pos.x - m_scale), int(m_pos.y - m_scale), int(m_pos.x + m_scale), int(m_pos.y + m_scale));

	SelectObject(hdc, old);
	SelectObject(hdc, oldPen);
	DeleteObject(hPen);
}

bool CTrap::Animation(const float ftime)
{
	if (m_size > TRAP_SIZE || m_scale < TRAP_SIZE)
	{
		m_size -= ftime * 10;
		m_scale += ftime * 10;
		return true;
	}
	else
	{
		m_size = TRAP_SIZE;
		m_scale = m_size - 10;
		return false;
	}
}
