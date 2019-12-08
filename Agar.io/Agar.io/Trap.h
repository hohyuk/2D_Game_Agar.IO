#pragma once
#include "GameObj.h"
class CTrap:public CGameObj
{
	bool bAnim;
public:
	CTrap();
	~CTrap();

	virtual void Create() override;
	virtual void Draw(HDC hdc);
	virtual void Update(const float ftime) override;

private:
	void CreateDraw(HDC hdc);
	bool Animation(const float ftime);
};

