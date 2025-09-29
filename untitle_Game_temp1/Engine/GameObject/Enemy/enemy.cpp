#include "enemy.h"
#include "../../../�v���W�F�N�g�ɒǉ����ׂ��t�@�C��_VC�p/DxLib.h"
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

	// �G���G�̉��c��
	constexpr int kEnemyUnitGraphWidth = 64;
	constexpr int kEnemyUnitGraphHeight = 64;

	// �������^�C�v�̎G���G�̉��c��
	constexpr int kEnemyMiniUnitGraphWidth = 32;
	constexpr int kEnemyMiniUnitGraphHeight = 32;

	// �G�̊g�嗦
	constexpr double  kEnemyScale = 2.0;

	// �G�̗̑�
	constexpr int kEnemyLife = 30;
	// �_���[�W���󂯂��Ƃ��̓_�Ńt���[����
	constexpr int kDamageBlinkFrame = 60;
 
	// �G���G�̐��͂��ꂼ��قȂ�
	constexpr int kEnrmyUnitMax = 2;
	constexpr int kEnemyMiniUnitMax = 3;

	// �e�̍ő吔
	constexpr int kEnemyBulletMax = 20;

	// �����̃A�j���[�V�����̃R�}��
	constexpr int kExplosionAnimeNum = 8;
	constexpr int kExplosionFrameSpeed = 8;
	// �����̃T�C�Y
	constexpr int kExplosionSize = 32;




}
enemy::enemy() :m_pos(0.0f,0.0f),
	m_speed(kEnemySpeed),
	m_enemyHandle(-1),
	m_Life(kEnemyLife),
	m_blinkFrameCount(0),
	m_isEnemyUnitReleased(false),
	m_enemyBulletSpeed(10),
	m_enemyBulletTimer(0),
	m_Dead(false),
	m_isDeadUpOnce(false)
{
	for (int i = 0; i <= kEnrmyUnitMax; i++)
	{
		// �G���G�̉摜�n���h����������
		m_enemyUnitHandle[i] = -1;
		m_enemyUnitPos[i] = Vec2(0.0f, 0.0f);
	}

	for (int i = 0; i <= kEnemyMiniUnitMax; i++)
	{
		// �������^�C�v�̎G���G�̉摜�n���h����������
		m_enemyMiniUnitHandle[i] = -1;
		m_enemyMiniUnitPos[i] = Vec2(0.0f, 0.0f);
	}
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
	// �G�̗̑͂�ݒ�
	m_Life = kEnemyLife;

	// �G�̉摜��ǂݍ���
	m_enemyHandle = LoadGraph("Date/Enemy/Boss_ship.png");

	// �G���G�̉摜��ǂݍ���
	for (int i = 0; i <= kEnrmyUnitMax; i++)
	{
		m_enemyUnitHandle[i] = LoadGraph("Date/Enemy/Enemy_Ship.png");
		m_enemyUnitPos[i] = Vec2(0.0f,0.0f);
	}
	// �������^�C�v�̎G���G�̉摜��ǂݍ���
	for (int i = 0; i <= kEnemyMiniUnitMax; i++)
	{
		m_enemyMiniUnitHandle[i] = LoadGraph("Date/Enemy/Enemy_mineShip");
		m_enemyMiniUnitPos[i] = Vec2(0.0f, 0.0f);
	}
	
}

void enemy::End()
{
	// �G�̉摜�����
	if (m_enemyHandle != -1)
	{
		m_enemyHandle = -1;
		DeleteGraph(m_enemyHandle);
	}

	// �G���G�̉摜�����
	for (int i = 0; i <= kEnrmyUnitMax; i++)
	{
		if (m_enemyUnitHandle[i] != -1)
		{
			m_enemyUnitHandle[i] = -1;
			DeleteGraph(m_enemyUnitHandle[i]);
		}
	}
	for (int i = 0; i <= kEnemyMiniUnitMax; i++)
	{
		if (m_enemyMiniUnitHandle[i] != -1)
		{
			m_enemyMiniUnitHandle[i] = -1;
			DeleteGraph(m_enemyMiniUnitHandle[i]);
		}
	}
}

void enemy::Update()
{
	if (m_Life > 0)
	{
		UpdateEnemy();
		UpdateEnemyUnit();
		UpdateEnemyMiniUnit();
		UpdateEnemyBullet();
	}

	// �G�̗̑͂�0�ȉ��Ȃ�|���ꂽ���Ƃɂ���
	if (m_Life <= 0)
	{
		// �����G�t�F�N�g�̃A�j���[�V�����t���[���̍X�V
		m_explosionAnimFrame++;
		int explosionTotalAnimeFrame = kExplosionFrameSpeed * kExplosionAnimeNum;
		UpdateDead();
	}
}

void enemy::Draw()
{
	DrawEnemy();
	DrawEnemyUnit();
	DrawEnemyMiniUnit();
	DrawEnemyBullet();
}

// �G(Boss)�̓����蔻��̎���
float enemy::EnemyGetPosX()
{
	return m_pos.x - kEnemyGraphWidth / half;
}

float enemy::EnemyGetPosY()
{
	return m_pos.y - kEnemyGraphHeight / half;
}

float enemy::GetEnemyRadius()
{
	return kEnemyRadius;
}

// �G���G�̓����蔻��
float enemy::EnemyUnitGetPosX()
{
	return 0.0f;
}

float enemy::EnemyUnitGetPosY()
{
	return 0.0f;
}

float enemy::GetEnemyUnitRadius()
{
	return 0.0f;
}

// �������^�C�v�̎G���G�̓����蔻��
float enemy::EnemyMiniUnitGetPosX()
{
	return 0.0f;
}

float enemy::EnemyMiniUnitGetPosY()
{
	return 0.0f;
}

float enemy::GetEnemyMiniUnitRadius()
{
	return 0.0f;
}

// �G���_���[�W���󂯂��Ƃ��̏���
void enemy::EnemysDamage()
{
	if (m_blinkFrameCount > 0) return;
	// ���G����(�_�ł��鎞��)��ݒ肷��
	m_blinkFrameCount = kDamageBlinkFrame;
	m_Life--;
	// �������^�C�v�̎G���G�����
	UpdateEnemyMiniUnit();

	if (m_Life <= 0)
	{
		m_Life = 0;
		// �����ɓG���|���ꂽ�Ƃ��̏�����ǉ�
	}
}

// �G���G���_���[�W���󂯂��Ƃ��̏���(����)
void enemy::EnemyUnitsDamage()
{
}

// �G(Boss)�̍X�V����
void enemy::UpdateEnemy()
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

// �G���G�̍X�V����
void enemy::UpdateEnemyUnit()
{
	// Boss�̕��G���G�̈��
	// �̗͂������ȉ��ɂȂ������̂ݕ������
	if (m_Life > kEnemyLife / half)return;
	// ��x�ł������ꂽ�ꍇ�A������͏o�����Ȃ��Ȃ�
	if( m_isEnemyUnitReleased == false)
	{
		m_isEnemyUnitReleased = true;
	}
	else
	{
		return;
	}

	// �G���G��2�̂��邪�A�������G�ƈقȂ�A�����_���ȉ�ʊO����������
	for (int i = 0; i <= kEnrmyUnitMax; i++)
	{
		// �������Ƃ��̓����_���ȉ�ʊO����������
		if (m_enemyUnitPos[i].x == 0.0f && m_enemyUnitPos[i].y == 0.0f)
		{
			m_enemyUnitPos[i].x = Game::kScreenWidth + kEnemyRadius; // ��ʂ̉E�[����������
			m_enemyUnitPos[i].y = static_cast<float>(GetRand(Game::kScreenHeight - kEnemyRadius)); // �����_����Y���W����������
		}
		// �����ꂽ�G���G��Boss�̑O�܂ō��Ɉړ����ABoss��荶�ɍs����Boss�Ɠ����悤�ɏ㉺�Ɉړ����A�e�����
		m_enemyUnitPos[i].x -= m_speed;
		// Boss�̑O�܂ŗ�����㉺�Ɉړ�����
		if (m_enemyUnitPos[i].x < m_pos.x)
		{
			m_enemyUnitPos[i].x = m_pos.x- kEnemyUnitGraphWidth; // Boss�̑O�Ŏ~�܂�


			// Boss�Ɠ����悤�ɏ㉺�Ɉړ�����
			m_enemyUnitPos[i].y -= m_speed;
			// ��ʂ̏�[�ɓ��B�����牺�Ɉړ�
			if (m_enemyUnitPos[i].y > Game::kScreenHeight - kEnemyRadius)
			{
				m_speed = -kEnemySpeed; // ���x�𔽓]���ď�Ɉړ�
			}
			// ��ʂ̉��[�ɓ��B�������Ɉړ�
			if (m_enemyUnitPos[i].y < kEnemyRadius)
			{
				m_speed = kEnemySpeed; // ���x�𔽓]���ĉ��Ɉړ�
			}

			// Boss�Ɠ����悤�Ɉړ��������ƒe�����
			// �����ɒe���������ǉ�
			UpdateEnemyBullet();
		}
	}
}

// �������^�C�v�̎G���G�̍X�V����
void enemy::UpdateEnemyMiniUnit()
{
	//Boss�̕������ȓG
	// �������^�C�v�̎G���G��3�̂���
	for (int i = 0; i <= kEnemyMiniUnitMax; i++)
	{
		// �������Ƃ���Boss�̒�������1�̂����炵�ĕ������
		if (m_enemyMiniUnitPos[i].x == 0.0f && m_enemyMiniUnitPos[i].y == 0.0f)
		{
			m_enemyMiniUnitPos[i].x = m_pos.x - (i * kEnemyRadius); // Boss�̒�������1�̂����炵�ĕ������
			m_enemyMiniUnitPos[i].y = m_pos.y + (i * kEnemyRadius); // Boss�̒�������1�̂����炵�ĕ������
		}

		// �����ꂽ�������^�C�v�̎G���G�͍��Ɉړ�����
		m_enemyMiniUnitPos[i].x -= m_speed;
		// ��ʊO�ɏo����ʒu�����Z�b�g
		if (m_enemyMiniUnitPos[i].x < -kEnemyRadius)
		{
			m_enemyMiniUnitPos[i] = Vec2(0.0f, 0.0f); // �ʒu�����Z�b�g
		}

	}
}

// �G(Boss)�̕`�揈��
void enemy::DrawEnemy()
{
	// �̗͂����F�ł���悤��
	DrawFormatString(10, 30, GetColor(255, 255, 255), "Enemy Life:%d", m_Life);

	// �_�ŏ���
	if (((int)m_blinkFrameCount / (int)half) % 2 == 1)
	{
		return;
	}
	// �~�̒��S�ɕ`��
	DrawRectRotaGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
		0, 0, kEnemyGraphWidth, kEnemyGraphHeight, kEnemyScale, 0.0, m_enemyHandle, TRUE);
	// �G�̕`�揈��
	DrawCircle(m_pos.x, m_pos.y, kEnemyRadius, kEnemyColor, FALSE); // ���~��`��
}

void enemy::DrawEnemyUnit()
{
	// �G���G�̕`�揈��
	for (int i = 0; i <= kEnrmyUnitMax; i++)
	{
		if (m_enemyUnitPos[i].x != 0.0f && m_enemyUnitPos[i].y != 0.0f)
		{
			// �~�̒��S�ɕ`��
			DrawRectRotaGraph(static_cast<int>(m_enemyUnitPos[i].x), static_cast<int>(m_enemyUnitPos[i].y),
				0, 0, kEnemyUnitGraphWidth, kEnemyUnitGraphHeight, kEnemyScale, 0.0, m_enemyUnitHandle[i], TRUE);
			// �G���G�̕`�揈��
			DrawCircle(m_enemyUnitPos[i].x, m_enemyUnitPos[i].y, kEnemyRadius / half,0xffff25, FALSE);//���F���~�ŕ`��
		}
	}
}

void enemy::DrawEnemyMiniUnit()
{
	// �������^�C�v�̎G���G�̕`�揈��
	for (int i = 0; i <= kEnemyMiniUnitMax; i++)
	{
			// �~�̒��S�ɕ`��
			DrawRectRotaGraph(static_cast<int>(m_enemyMiniUnitPos[i].x), static_cast<int>(m_enemyMiniUnitPos[i].y),
				0, 0, kEnemyMiniUnitGraphWidth, kEnemyMiniUnitGraphHeight, kEnemyScale, 0.0, m_enemyMiniUnitHandle[i], TRUE);
			// �������^�C�v�̎G���G�̕`�揈��
			DrawCircle(m_enemyMiniUnitPos[i].x, m_enemyMiniUnitPos[i].y, kEnemyRadius / 4,0xff0000, FALSE); // ���~��`��
	}
}

void enemy::UpdateEnemyBullet()
{
	// ���Ԃ����ƕ������
	if (m_enemyBulletTimer > 0)
	{
		// �v���C���[�̎ˌ�����
		for (int i = 0; i <= kEnemyBulletMax; i++)
		{
			m_isEnemyBullet[i] = true;
			m_enemyBulletPos[i] = Vec2(m_enemyUnitPos[i]); // �e�̈ʒu���v���C���[�̈ʒu�ɐݒ�
			break; // 1����������
		}

		// �e�̍X�V����
		for (int i = 0; i <= kEnemyBulletMax; i++)
		{
			if (m_isEnemyBullet[i] == true)
			{
				// �e�̈ʒu���X�V
				m_enemyBulletPos[i].x -= 15.0f; // �E�����Ɉړ�
				// ��ʊO�ɏo����e�����Z�b�g
				if (m_enemyBulletPos[i].x > Game::kScreenWidth)
				{
					m_enemyBulletPos[i] = m_pos; // �v���C���[�̈ʒu�ɖ߂�
					m_isEnemyBullet[i] = false; // �e��������Ă��Ȃ���Ԃɂ���

				}
			}
		}
	}

}

void enemy::DrawEnemyBullet()
{
}

void enemy::UpdateDead()
{
	// �G�͓|���ꂽ�Ƃ��Ă��A�|����Ă��Ȃ���Ԃ̎��Ɠ������A�㉺�Ɉړ����邪
	// ��x������ɍs������ɉ��ɍs���A��ʊO�֏�����
	m_pos.y += m_speed;
	if (m_isDeadUpOnce == false)
	{
		// ��ʂ̏�[�ɓ��B�����牺�Ɉړ�
		if (m_pos.y > Game::kScreenHeight - kEnemyRadius)
		{
			m_speed = -kEnemySpeed; // ���x�𔽓]���ď�Ɉړ�
		}
		
		if (m_pos.y < kEnemyRadius)
		{
			m_speed = kEnemySpeed; // ���x�𔽓]���ĉ��Ɉړ�
			m_isDeadUpOnce = true; // ��x��ɍs�������Ƃ��L�^
		}
	}
	if (m_isDeadUpOnce == true)
	{
		// ��ʂ̏㑤�ɓ��B�����牺�Ɉړ�
		if (m_pos.y < kEnemyRadius)
		{
			m_speed = kEnemySpeed; // ���x�𔽓]���ĉ��Ɉړ�
		}

		// ��ʂ̉��[�ɓ��B������G�͎���
		if (m_pos.y > Game::kScreenHeight + kEnemyRadius)
		{
			m_Dead = true; // �G�����񂾂��Ƃ��L�^
		}
	}

}
void enemy::DrawDead()
{
	if (m_Life <= 0)
	{
		int animeIndex = m_explosionAnimFrame / kExplosionAnimeNum;

		if (m_explosionHandle != -1)
		{
			DrawRectRotaGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
				animeIndex * kExplosionSize, 0, kExplosionSize, kExplosionSize, kEnemyScale, 0.0, m_explosionHandle, TRUE);
		}
	}

}
