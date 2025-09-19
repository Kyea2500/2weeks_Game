#pragma once
#include "../../InputDevice/Vec2.h"
class Bg
{
	public:
	Bg();
	~Bg();
	void Init();
	void End();
	void Update();
	void Draw();
private:
	void DrawBg();

private:
	int m_BgImage;
	int m_BgHandle;
	Vec2 m_pos;
	Vec2 m_posScroll;
	bool m_is2Scroll;
};

