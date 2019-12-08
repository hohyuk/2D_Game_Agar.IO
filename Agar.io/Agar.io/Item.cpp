#include "stdafx.h"
#include "Item.h"


CItem::CItem()
{
	Create();		// size때문에 위에다 놓는다.
	bOut = false;
	m_pos = Point{ (rand() % (CLIENT_WIDTH - int(m_size))) + m_size, (rand() % (CLIENT_HEIGHT - int(m_size))) + m_size };
}

CItem::CItem(Point pos, Point dir)
{
	Create();
	bOut = true;
	m_pos = pos;
	m_dir = dir;
	m_fspeed = 200.f;
}

CItem::~CItem()
{
}

void CItem::Create()
{
	m_size = 5;
	m_scale = m_size;
	bScale = false;
	pColor = RGB(255, 150, 50);
	bColor = RGB(rand() % 255, rand() % 255, rand() % 255);
}

void CItem::Update(const float ftime)
{
	CGameObj::RestrictedArea();
	if (bOut)
	{
		if (CreateTime > 0.5f)
			bOut = false;
		else
		{
			CreateTime += ftime;
			m_pos += m_dir * m_fspeed * ftime;
		}
	}
}
