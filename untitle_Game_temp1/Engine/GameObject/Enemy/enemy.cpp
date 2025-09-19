#include "enemy.h"
#include "DxLib.h"
#include "../../InputDevice/game.h"
namespace
	{
	// “G‚Ì‰ŠúÀ•W
	constexpr float kInitialPosX = 100.0f;
	constexpr float kInitialPosY = 100.0f;
	// “G‚ÌˆÚ“®‘¬“x
	constexpr float kEnemySpeed = 5.0f;
	// “G‚Ì”¼Œa
	constexpr int kEnemyRadius = 100;
	// “G‚ÌF
	constexpr int kEnemyColor = 0x2525ff; // ÂF

	constexpr float half = 2.0f;

	// “G‚Ì‰¡c”ä
	constexpr int kEnemyGraphWidth = 120;
	constexpr int kEnemyGraphHeight = 120;

	// “G‚ÌŠg‘å—¦
	constexpr double kEnemyScale = 2.0;

	// “G‚Ì‘Ì—Í
	constexpr int kEnemyLife = 30;
	// ƒ_ƒ[ƒW‚ğó‚¯‚½‚Æ‚«‚Ì“_–ÅƒtƒŒ[ƒ€”
	constexpr int kDamageBlinkFrame = 60;

}
enemy::enemy() :m_pos(0.0f,0.0f),
	m_speed(kEnemySpeed),
	m_enemyHandle(-1),
	m_Life(kEnemyLife)
{
}

enemy::~enemy()
{
}

void enemy::Init()
{
	// “G‚Ì‰ŠúÀ•W‚ğİ’è
	m_pos.x = Game::kScreenWidth - kInitialPosX; // ‰æ–Ê’[‚É”z’u
	m_pos.y = Game::kScreenHeight / half; // ‰æ–Ê’[‚É”z’u
	// “G‚ÌˆÚ“®‘¬“x‚ğİ’è
	m_speed = kEnemySpeed;
	// “G‚Ì‰æ‘œ‚ğ“Ç‚İ‚Ş
	m_enemyHandle = LoadGraph("Date/Enemy/Boss_ship.png");
}

void enemy::End()
{
}

void enemy::Update()
{
	// –³“GŠÔ‚ÌXV
	m_blinkFrameCount--;
	if (m_blinkFrameCount < 0)
	{
		m_blinkFrameCount = 0;
	}

	// “G‚Íã‰º‚ÉˆÚ“®‚·‚é
	m_pos.y += m_speed;
	// ‰æ–Ê‚Ìã’[‚É“’B‚µ‚½‚ç‰º‚ÉˆÚ“®
	if (m_pos.y > Game::kScreenHeight - kEnemyRadius)
	{
		m_speed = -kEnemySpeed; // ‘¬“x‚ğ”½“]‚µ‚Äã‚ÉˆÚ“®
	}
	// ‰æ–Ê‚Ì‰º’[‚É“’B‚µ‚½‚çã‚ÉˆÚ“®
	if (m_pos.y < kEnemyRadius)
	{
		m_speed = kEnemySpeed; // ‘¬“x‚ğ”½“]‚µ‚Ä‰º‚ÉˆÚ“®
	}
}

void enemy::Draw()
{
	// ‰~‚Ì’†S‚É•`‰æ
	DrawRectRotaGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
		0,0,kEnemyGraphWidth, kEnemyGraphHeight, kEnemyScale, 0.0, m_enemyHandle, TRUE);
	// “G‚Ì•`‰æˆ—
	DrawCircle(m_pos.x, m_pos.y, kEnemyRadius, kEnemyColor, FALSE); // Â‚¢‰~‚ğ•`‰æ
}

float enemy::EnemyGetPosX()
{
	return m_pos.x;
}

float enemy::EnemyGetPosY()
{
	return m_pos.x;
}

float enemy::GetEnemyRadius()
{
	return kEnemyRadius;
}

void enemy::EnemysDamage()
{
	if (m_blinkFrameCount > 0) return;
	// –³“GŠÔ(“_–Å‚·‚éŠÔ)‚ğİ’è‚·‚é
	m_blinkFrameCount = kDamageBlinkFrame;
	m_Life--;
	// ƒ_ƒ[ƒW‚ğó‚¯‚½‚Æ‚«‚Ìˆ—
	m_Life--;
	if (m_Life <= 0)
	{
		m_Life = 0;
		// ‚±‚±‚É“G‚ª“|‚³‚ê‚½‚Æ‚«‚Ìˆ—‚ğ’Ç‰Á
	}
}
