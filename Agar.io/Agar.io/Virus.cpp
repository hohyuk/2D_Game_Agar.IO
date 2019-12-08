#include "stdafx.h"
#include "Virus.h"
#include "Player.h"
#include "Item.h"

CVirus::CVirus()
{
	m_size = NOMALSIZE;
	m_pos = Point{ (rand() % (CLIENT_WIDTH - int(m_size))) + m_size, (rand() % (CLIENT_HEIGHT - int(m_size))) + m_size };
	m_dir = Point{ 0,0 };
	bRespawn = true;
	Create();
}

CVirus::CVirus(Point pos, Point dir, float size)
{
	m_size = size;
	m_pos = pos;
	m_dir = dir;
	m_scale = m_size;
	bRespawn = false;
	CreateTime = 1.f;
	Create();
}

CVirus::~CVirus()
{
}

void CVirus::Create()
{
	bScale = false;
	m_scale = m_size;
	pColor = RGB(255, 0, 0);
	bColor = RGB(250, 50, 5);
	m_TARGETID = ENUM_TARGETID::NON_TARGET;
}

void CVirus::Update(const float ftime)
{
	CGameObj::RestrictedArea();
	CGameObj::SizeChange(ftime);

	if (m_TARGETID == ENUM_TARGETID::PLAYER_TARGET && m_pPlayer != nullptr)
	{
		if (Length(m_pos - m_pPlayer->GetPos()) > PLAYER_RANGE)
		{
			m_TARGETID = ENUM_TARGETID::NON_TARGET;
			return;
		}
		m_pPlayer->SetDelete(TargetMove(m_pPlayer->GetPos(), m_pPlayer->GetSize()));
	}
	else if (m_TARGETID == ENUM_TARGETID::ITEM_TARGET && m_pItem != nullptr)
	{
		m_pItem->SetDelete(TargetMove(m_pItem->GetPos(), m_pItem->GetSize()));
	}
	else if (m_TARGETID == ENUM_TARGETID::VIRUSTARGET && m_pVirus != nullptr)
	{
		if (Length(m_pos - m_pVirus->GetPos()) > VIRUS_RANGE)
		{
			m_TARGETID = ENUM_TARGETID::NON_TARGET;
			return;
		}
		m_pVirus->SetDelete(TargetMove(m_pVirus->GetPos(), m_size + m_pVirus->GetSize()));
	}
	else
	{
		m_pPlayer = nullptr;
		m_pItem = nullptr;
		m_pVirus = nullptr;
		m_TARGETID = ENUM_TARGETID::NON_TARGET;
		m_dir = Point{ 0,0 };
	}
	if (!bRespawn) // 분열된것이라면...
	{
		CreateTime -= ftime;
		if (CreateTime <= 0.f)
			bRespawn = true;
		m_fspeed = SPEED / (m_size * 2);
	}
	else
		m_fspeed = SPEED / m_size;

	if (m_fspeed < 10.f) m_fspeed = 10.f;
	m_pos += m_dir * m_fspeed * ftime;
}

bool CVirus::TargetMove(const Point & p, const float & size)
{
	m_dir = Point{ p - m_pos };
	m_dir = normalize(m_dir);
	if (Collision(p, m_pos, m_size))
	{
		m_size += size / 4.f;
		m_TARGETID = ENUM_TARGETID::NON_TARGET;
		return true;
	}
	else
		return false;
}

