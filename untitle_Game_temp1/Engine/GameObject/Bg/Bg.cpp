#include "Bg.h"
#include "DxLib.h"
#include "../../InputDevice/game.h"

namespace 
{
	// ägëÂÅEèkè¨ó¶
	constexpr double kBgScale = 8.0;
	// îwåiÇÃâ°ècî‰
	constexpr int kBgGraphWidth = 320;
	constexpr int kBgGraphHeight = 320;
}

Bg::Bg() :
	m_pos(0.0f,0.0f),
	m_BgImage(-1),
	m_posScroll(0.0f, 0.0f),
	m_BgHandle(-1),
	m_is2Scroll(false)
{
}

Bg::~Bg()
{
	DeleteGraph(m_BgImage);
}

void Bg::Init()
{
	m_BgImage = LoadGraph("Date/Map/BackGround.png");
	m_BgHandle = LoadGraph("Date/Map/BackGround.png");
}
void Bg::End()
{
}
void Bg::Update()
{
	if (m_is2Scroll==false)
	{
		// îwåiÇÃçXêVèàóù
		m_pos.x -= 3.0f; // ç∂Ç…à⁄ìÆ
		if (m_pos.x <= -Game::kScreenWidth+(kBgGraphWidth))
		{
			m_posScroll.x = 0.0f;
			m_posScroll.x -= 0.5f;
			if(m_pos.x <= -Game::kScreenWidth)
			{
				m_pos.x = 0.0f;
				m_is2Scroll = true;
			}
		}
	}
	else
	{
		m_posScroll.x -= 3.0f; // ç∂Ç…à⁄ìÆ
		if (m_posScroll.x <= -Game::kScreenWidth + (kBgGraphWidth))
		{
			m_pos.x = 0.0f;
			m_pos.x -= 0.5f;
			if (m_posScroll.x <= -Game::kScreenWidth)
			{
				m_posScroll.x = 0.0f;
				m_is2Scroll = false;
			}
		}
	}
	

}


void Bg::Draw()
{
	DrawBg();
}

void Bg::DrawBg()
{
	if (m_is2Scroll == false)
	{
		// îwåiÇÃï`âÊèàóù
		DrawRectRotaGraph(static_cast<int>(m_pos.x + Game::kScreenWidth), static_cast<int>(m_pos.y),
			0.0, 0.0, kBgGraphWidth, kBgGraphWidth,
			kBgScale, 0.0, m_BgImage, TRUE,FALSE,FALSE);
	}
	if(m_is2Scroll==true)
	{
		// îwåiÇÃï`âÊèàóù
		DrawRectRotaGraph(static_cast<int>(m_posScroll.x + Game::kScreenWidth), static_cast<int>(m_posScroll.y),
			0.0, 0.0, kBgGraphWidth, kBgGraphWidth,
			kBgScale, 0.0, m_BgHandle, TRUE,TRUE,TRUE);
	}
}