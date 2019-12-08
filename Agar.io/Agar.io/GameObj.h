#pragma once

class CGameObj
{
protected:
	HPEN hPen, oldPen;
	HBRUSH hBrush, oldBrush;
	COLORREF pColor;	// �� ��
	COLORREF bColor;	// �귯�� ��
	Point m_pos;
	Point m_dir;
	float m_size;
	float m_scale;
	float m_fspeed;
	
	bool bScale;		// size ��ȭ
	bool bRespawn;		// �п��Ǿ��ִ��� Ȯ��.
	bool bDelete{ false };
	float CreateTime{ 0 };
	float EffectTime{ 0 };
public:
	CGameObj();
	virtual ~CGameObj();

	virtual void Create() = 0;
	virtual void Draw(HDC hdc);
	virtual void Update(const float ftime) = 0;

	Point GetPos()const { return m_pos; }
	Point GetDir()const { return m_dir; }
	float GetSize()const { return m_size; }
	float GetSpeed()const { return m_fspeed; }
	bool GetRespawn()const { return bRespawn; }
	bool GetDelete()const { return bDelete; }

	void SetDelete(bool d) { bDelete = d; }
	void SetSize(float s) { m_size += s; }			// size�� speed�� �ݺ���Ѵ�.
protected:
	void RestrictedArea();
	void SizeChange(const float ftime);
};

