#pragma once
#include "GameObj.h"
class CPlayer:public CGameObj
{
	Point m_mouse;
	bool bDirection{ false };
public:
	CPlayer();
	CPlayer(Point pos, Point dir, float size);
	bool operator<(const CPlayer& other) const
	{
		return m_size < other.m_size;
	}
	virtual ~CPlayer();

	virtual void Create() override;
	void MouseMove(LPARAM lParam);
	virtual void Update(const float ftime) override;
};

