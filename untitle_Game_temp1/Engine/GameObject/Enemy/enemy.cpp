#include "enemy.h"
#include "DxLib.h"
#include "../../InputDevice/game.h"
namespace
	{
	// �G�̏������W
	constexpr float kInitialPosX = 100.0f;
	constexpr float kInitialPosY = 100.0f;
	// �G�̈ړ����x
	constexpr float kEnemySpeed = 5.0f;
	// �G�̔��a
	constexpr int kEnemyRadius = 100;
	// �G�̐F
	constexpr int kEnemyColor = 0x2525ff; // �F

	constexpr float half = 2.0f;

	// �G�̉��c��
	constexpr int kEnemyGraphWidth = 120;
	constexpr int kEnemyGraphHeight = 120;

	// �G�̊g�嗦
	constexpr double kEnemyScale = 2.0;

	// �G�̗̑�
	constexpr int kEnemyLife = 30;
	// �_���[�W���󂯂��Ƃ��̓_�Ńt���[����
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
	// �G�̏������W��ݒ�
	m_pos.x = Game::kScreenWidth - kInitialPosX; // ��ʒ[�ɔz�u
	m_pos.y = Game::kScreenHeight / half; // ��ʒ[�ɔz�u
	// �G�̈ړ����x��ݒ�
	m_speed = kEnemySpeed;
	// �G�̉摜��ǂݍ���
	m_enemyHandle = LoadGraph("Date/Enemy/Boss_ship.png");
}

void enemy::End()
{
}

void enemy::Update()
{
	// ���G���Ԃ̍X�V
	m_blinkFrameCount--;
	if (m_blinkFrameCount < 0)
	{
		m_blinkFrameCount = 0;
	}

	// �G�͏㉺�Ɉړ�����
	m_pos.y += m_speed;
	// ��ʂ̏�[�ɓ��B�����牺�Ɉړ�
	if (m_pos.y > Game::kScreenHeight - kEnemyRadius)
	{
		m_speed = -kEnemySpeed; // ���x�𔽓]���ď�Ɉړ�
	}
	// ��ʂ̉��[�ɓ��B�������Ɉړ�
	if (m_pos.y < kEnemyRadius)
	{
		m_speed = kEnemySpeed; // ���x�𔽓]���ĉ��Ɉړ�
	}
}

void enemy::Draw()
{
	// �~�̒��S�ɕ`��
	DrawRectRotaGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
		0,0,kEnemyGraphWidth, kEnemyGraphHeight, kEnemyScale, 0.0, m_enemyHandle, TRUE);
	// �G�̕`�揈��
	DrawCircle(m_pos.x, m_pos.y, kEnemyRadius, kEnemyColor, FALSE); // ���~��`��
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
	// ���G����(�_�ł��鎞��)��ݒ肷��
	m_blinkFrameCount = kDamageBlinkFrame;
	m_Life--;
	// �_���[�W���󂯂��Ƃ��̏���
	m_Life--;
	if (m_Life <= 0)
	{
		m_Life = 0;
		// �����ɓG���|���ꂽ�Ƃ��̏�����ǉ�
	}
}
