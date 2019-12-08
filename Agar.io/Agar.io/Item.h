#pragma once
#include "GameObj.h"
class CItem :public CGameObj
{
	bool bOut;	// Player∞° Item¿ª ª—∏±∂ß
	
public:
	CItem();
	CItem(Point pos, Point dir);
	virtual ~CItem();

	virtual void Create() override;
	virtual void Update(const float ftime) override;

	bool GetOut()const { return bOut; }
};

