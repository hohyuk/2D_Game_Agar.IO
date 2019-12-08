#include "stdafx.h"
#include "Player.h"


CPlayer::CPlayer()
{
	m_pos = Point{ CLIENT_WIDTH / 2, CLIENT_HEIGHT / 2 };
	m_dir = Point{ 0,0 };
	m_size = NOMALSIZE;
	bRespawn = true;
	Create();
}

CPlayer::CPlayer(Point pos, Point dir, float size)
{
	m_pos = pos;
	m_dir = dir;
	m_size = size;
	bRespawn = false;
	CreateTime = 1.f;
	Create();
}

CPlayer::~CPlayer()
{
}

void CPlayer::Create()
{
	pColor = RGB(30, 100, 10);
	bColor = RGB(80, 250, 55);
	m_scale = m_size;
	bScale = false;
}

void CPlayer::MouseMove(LPARAM lParam)
{
	m_mouse=Point{ float(LOWORD(lParam)) ,float(HIWORD(lParam)) };
	if (bRespawn)
		bDirection = true;	// 마우스가 움직이면 방향을 구한다.
}

void CPlayer::Update(const float ftime)
{
	CGameObj::RestrictedArea();
	CGameObj::SizeChange(ftime);

	if (bDirection)
	{
		m_dir = Point{ m_mouse - m_pos };
		m_dir = normalize(m_dir);
		if (Collision(m_mouse, m_pos, m_size)) bDirection = false;
	}

	if (!bRespawn) // 분열된것이라면...
	{
		CreateTime -= ftime;
		if (CreateTime <= 0.f)
			bRespawn = true;
		m_fspeed = SPEED / (m_size*2);
	}
	else
		m_fspeed = SPEED / m_size;

	if (m_fspeed < 10.f) m_fspeed = 10.f;
	m_pos += m_dir * m_fspeed * ftime;
}
