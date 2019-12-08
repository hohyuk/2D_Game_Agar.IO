#pragma once
#include "GameObj.h"
class CPlayer;
class CItem;
class CVirus :public CGameObj
{
	CPlayer* m_pPlayer{ nullptr };
	CItem* m_pItem{ nullptr };
	CVirus* m_pVirus{ nullptr };
	ENUM_TARGETID m_TARGETID{ ENUM_TARGETID::NON_TARGET };
public:
	CVirus();
	CVirus(Point pos, Point dir, float size);
	virtual~CVirus();

	virtual void Create() override;
	virtual void Update(const float ftime)override;

	ENUM_TARGETID GetTarget()const { return m_TARGETID; }
	void SetTarget(ENUM_TARGETID t) { m_TARGETID = t; }
	void SetPlayerInfo(CPlayer* p) { m_pPlayer = p; }
	void SetItemInfo(CItem* i) { m_pItem = i; }
	void SetVirusInfo(CVirus* v) { m_pVirus = v; }

private:
	bool TargetMove(const Point & p, const float & size);
};

