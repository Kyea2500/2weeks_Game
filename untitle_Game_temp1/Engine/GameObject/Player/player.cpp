#include "player.h"
#include "DxLib.h"
#include "../../InputDevice/game.h"
#include "../../InputDevice/Pad/Pad.h"

namespace
{
	// �v���C���[�̏������W
	constexpr float kInitialPosX = 320.0f;
	constexpr float kInitialPosY = 240.0f;
	// �v���C���[�̈ړ����x
	constexpr float kPlayerSpeed = 1024.0f;

	// �v���C���[�̔��a
	constexpr int kPlayerRadius = 20;

	// �v���C���[�̐F
	constexpr int kPlayerColor = 0xff2525; // �ԐF

	const int kPlayerDamage1 = LoadGraph("Date/Player/Player_Damage.png");
	const int kPlayerDamage2 = LoadGraph("Date/Player/Player_Damage2.png");
	const int kPlayerDamage3 = LoadGraph("Date/Player/Player_Damage3.png");

	// �G���W�������̃A�j���[�V�����̃R�}��
	constexpr int kEngineIdleAnimeNum = 2;
	constexpr int kEngineMoveAnimeNum = 3;

	// �A�j���[�V�����t���[�����x
	constexpr int kAnimeFrameSpeed = 5;

	// �v���C���[�̉摜�T�C�Y
	constexpr int kPlayerImageWidth = 48;
	constexpr int kPlayerImageHeight = 48;

	// �v���C���[�̊g�嗦
	constexpr double kPlayerScale = 2.5;
	// �v���C���[�̉�]�p�x
	constexpr double kPlayerAngle = 1.6;

	// �G���W���̈ʒu����
	constexpr int kEngineOffsetX = 15;
	constexpr int kEngineOffsetY = 30;

	// �����ɂ���
	constexpr int half = 2;

	// �e���o�Ȃ����ԍő�l
	constexpr float kShotTimerMax = 20.0f;

	// �e�֘A
	constexpr int kShotImageWidth = 76;
	constexpr int kShotImageHeight = 98;
	// �e�̔��a
	constexpr int kShotRadius = 10;
	// �e�̓����蔻��̐F
	constexpr int kShotColor = 0xffff00; // ���F


	constexpr float kSpeed = 10.0f;
	// �g��E�k����
	constexpr float kScale = 0.5f;
	constexpr int MaxShot = 20;

}

player::player()
	: m_pos(0.0f, 0.0f), // �v���C���[�̏������W��ݒ�
	speed(kPlayerSpeed), // �v���C���[�̈ړ����x��ݒ�
	m_playerHandle(-1), // �v���C���[�̉摜�n���h����������
	m_engineHandleIdle(-1), // �G���W�������̉摜�n���h����������
	m_engineHandleMove(-1), // �G���W�������̈ړ����摜�n���h����������
	m_animFrame(0), // �A�j���[�V�����t���[����������
	m_isMove(false), // �v���C���[�̈ړ���Ԃ�������
	m_isLastShot(false), // �Ō�Ɍ������e���ǂꂩ��������
	m_shotTimer(0) // �v���C���[�̎ˌ��^�C�}�[��������
{
	for (int i = 0; i <= MaxShot; i++)
	{
		m_shotPos[i] = Vec2(m_pos); // �e�̏������W��ݒ�
		m_shotHandle[i] = -1; // �e�̉摜�n���h����������
		m_isShot[i] = false; // �v���C���[�̎ˌ���Ԃ�������
	}
}

player::~player()
{
}

void player::Init()
{
	// �v���C���[�̏������W��ݒ�
	m_pos.x = kInitialPosX; // ��ʂ̒����ɔz�u
	m_pos.y = kInitialPosY; // ��ʂ̒����ɔz�u
	// �v���C���[�̉摜��ǂݍ���
	m_playerHandle = LoadGraph("Date/Player/Player_Full.png");
	// �G���W�������̉摜��ǂݍ���
	m_engineHandleIdle = LoadGraph("Date/Effects/Player_IdleEngine.png");
	m_engineHandleMove = LoadGraph("Date/Effects/Player_MoveingEngine.png");

	m_animFrame = 0; // �A�j���[�V�����t���[����������
	m_isMove = false; // �v���C���[�̈ړ���Ԃ�������
	m_shotTimer = 0; // �v���C���[�̎ˌ��^�C�}�[��������

	for (int i = 0; i <= MaxShot; i++)
	{
		m_shotPos[i] = Vec2(0.0f, 0.0f); // �e�̏������W��ݒ�
		m_shotHandle[i] = LoadGraph("Date/Player/Player_shot_1.png"); // �e�̉摜��ǂݍ���
		m_isShot[i] = false; // �v���C���[�̎ˌ���Ԃ�������
	}
}

void player::End()
{
	// �v���C���[�̉摜�����
	if (m_playerHandle != -1)
	{
		DeleteGraph(m_playerHandle);
		m_playerHandle = -1;
	}
	// �G���W�������̉摜�����
	if (m_engineHandleIdle != -1)
	{
		DeleteGraph(m_engineHandleIdle);
		m_engineHandleIdle = -1;
	}
	if (m_engineHandleMove != -1)
	{
		DeleteGraph(m_engineHandleMove);
		m_engineHandleMove = -1;
	}

	// �e�̉摜�����
	for (int i = 0; i <= MaxShot; i++)
	{
		if (m_shotHandle[i] != -1)
		{
			DeleteGraph(m_shotHandle[i]);
			m_shotHandle[i] = -1;
		}
	}
}

void player::Update()
{

	// ���͏�Ԃ̍X�V
	Pad::Update();
	
	m_animFrame++;
	int totalAnimeFrame = kAnimeFrameSpeed * kEngineIdleAnimeNum;
	if (m_isMove)
	{
		totalAnimeFrame = kAnimeFrameSpeed * kEngineMoveAnimeNum;
	}

	// �A�j���[�V�����t���[�������[�v������
	if(m_animFrame >= totalAnimeFrame)
	{
		m_animFrame = 0;
	}

	// �v���C���[�̍X�V����
	UpdateMove();
	// �v���C���[�̎ˌ�����
	m_shotTimer++;
	// �Q�[���X�^�[�g�Ɠ����ɒe���o�Ȃ��悤�Ƀ^�C�}�[��u��
	if (m_shotTimer>= kShotTimerMax)
	{
		UpdateShot();
		m_shotTimer = kShotTimerMax; // �^�C�}�[�����������Ȃ��悤�ɗ}��
	}
}

void player::Draw()
{
	DrawPlayer();
	DrawEngine();
	DrawShot();
}

float player::PlayerGetPosX()
{
	return m_pos.x;
}

float player::PlayerGetPosY()
{
	return m_pos.y;
}

float player::GetPlayerRadius()
{
	return kPlayerRadius;
}

float player::ShotGetPosX()
{
	for(int i=0;i<=MaxShot;i++)
	{
		if (m_isShot[i]==true)
		{
			return m_shotPos[i].x;
		}
	}
}

float player::ShotGetPosY()
{
	for(int i=0;i<=MaxShot;i++)
	{
		if (m_isShot[i]==true)
		{
			return m_shotPos[i].y;
		}
	}
}

float player::GetShotRadius()
{
	for(int i=0;i<=MaxShot;i++)
	{
		if (m_isShot[i]==true)
		{
			return kShotRadius;
		}
	}
}


void player::UpdateMove()
{
	// �v���C���[�̈ړ�����
	// �ړ��x�N�g����������
	Vec2 move(0.0f, 0.0f);
	m_isMove = false;

	// �v���C���[�̈ړ������̌�����������
	if (Pad::IsPress(PAD_INPUT_LEFT))
	{
		// ���Ɉړ�
		move.x -= speed;
		m_isMove = true;
	}
	if (Pad::IsPress(PAD_INPUT_DOWN))
	{
		// ���Ɉړ�
		move.y += speed;
		m_isMove = true;
	}
	if (Pad::IsPress(PAD_INPUT_RIGHT))
	{
		// �E�Ɉړ�
		move.x += speed;
		m_isMove = true;
	}
	if (Pad::IsPress(PAD_INPUT_UP))
	{
		// ��Ɉړ�
		move.y -= speed;
		m_isMove = true;
	}
	// �ړ��x�N�g���𐳋K��
	if (move.Length() > 0.0f)
	{
		move.Normalize();
	}

	// �v���C���[�̈ʒu���X�V
	m_pos += move;

	// ��ʊO�ɏo�Ȃ��悤�ɐ���
	if (m_pos.x < kPlayerImageWidth/half)
	{
		m_pos.x = kPlayerImageWidth/half;
	}
	if (m_pos.x > Game::kScreenWidth - kPlayerImageWidth / half)
	{
		m_pos.x = kPlayerImageWidth / half;
	}
	if (m_pos.y < kPlayerImageHeight/half)
	{
		m_pos.y = kPlayerImageHeight/half;
	}
	if (m_pos.y > Game::kScreenHeight- kPlayerImageHeight / half)
	{
		m_pos.y = Game::kScreenHeight - kPlayerImageHeight / half;
	}

}

void player::UpdateShot()
{
	// �e��������
	if (Pad::IsPress(PAD_INPUT_A))
	{
		// �v���C���[�̎ˌ�����
		for (int i = 0; i <= MaxShot; i++)
		{
			// �O�̃t���[���ŉ�����Ă������̊m�F�B
			if (m_isLastShot == false)
			{ 				// �O�̃t���[���ŉ�����Ă��Ȃ������玟�̃��[�v��
				if (m_isShot[i] == true)
				{
					// �Ō�Ɍ������e������Ύ��̒e������
					continue; // ���̃��[�v��
				}
				m_isShot[i] = true;
				m_shotPos[i] = Vec2(m_pos); // �e�̈ʒu���v���C���[�̈ʒu�ɐݒ�
				m_isLastShot = true; // �Ō�Ɍ������e��ۑ�
				break; // 1����������
			}

			else
			{
				break; // �O�̃t���[���ŉ�����Ă����牽�����Ȃ�
			}

			m_isLastShot = true; // �O�̃t���[���ŉ�����Ă�����Ō�Ɍ������e��ۑ�
		}
	}

	else
	{
		// A�{�^����������Ă��Ȃ���΍Ō�Ɍ������e�̕ۑ������Z�b�g
		m_isLastShot= false;
	}

	// �e�̍X�V����
	for(int i = 0; i <= MaxShot; i++)
	{
		if(m_isShot[i]==true)
		{
			// �e�̈ʒu���X�V
			m_shotPos[i].x += 15.0f; // �E�����Ɉړ�
			// ��ʊO�ɏo����e�����Z�b�g
			if (m_shotPos[i].x > Game::kScreenWidth)
			{
				m_shotPos[i] = m_pos; // �v���C���[�̈ʒu�ɖ߂�
				m_isShot[i] = false; // �e��������Ă��Ȃ���Ԃɂ���
				
			}
		}
	}

	// �e�̘A�˂�

	
}

void player::DrawPlayer()
{
	// �`�揈��
	// �v���C���[�̕`�揈��
	DrawCircle(m_pos.x, m_pos.y, kPlayerRadius, kPlayerColor, FALSE); // �Ԃ��~��`��

	// �v���C���[�̉摜��`��
	if (m_playerHandle != -1)
	{
		// �~�̒��S�ɕ`��
		DrawRotaGraph(m_pos.x, m_pos.y, kPlayerScale, kPlayerAngle, m_playerHandle, TRUE);
	}
}

void player::DrawEngine()
{
	// �v���C���[�̃A�j���[�V��������
	int animeIndex = m_animFrame / kEngineIdleAnimeNum;
	// �ړ����Ȃ�G���W���̃A�j���[�V������ύX
	if (m_isMove)
	{
		animeIndex = m_animFrame / kEngineMoveAnimeNum;
	}

	// �g�p����G���W�������̉摜�n���h��������
	int useHandle = m_engineHandleIdle;
	if (m_isMove)
	{
		useHandle = m_engineHandleMove;
	}


	// �G���W�������̉摜��`��
	if (m_engineHandleIdle != -1)
	{
		DrawRectRotaGraph(static_cast<int>(m_pos.x - kPlayerImageWidth / kEngineOffsetX), static_cast<int>(m_pos.y - kPlayerImageHeight / kEngineOffsetY),
			animeIndex * kPlayerImageWidth, 0, kPlayerImageWidth, kPlayerImageHeight, kPlayerScale, kPlayerAngle,
			useHandle, true, true, false);
	}
}

void player::DrawShot()
{
	// �e�̕`�揈��
	for(int i=0;i<=MaxShot;i++)
	{
		if (m_isShot[i]==true)
		{
			DrawCircle(m_shotPos[i].x, m_shotPos[i].y, kShotRadius, kShotColor, FALSE); // ���F���~��`��
			// �e�̉摜��`��
			DrawRectRotaGraph(static_cast<int>(m_shotPos[i].x), static_cast<int>(m_shotPos[i].y),
				0, 0, kShotImageWidth, kShotImageHeight, kScale, 0.0f,
				m_shotHandle[i], true, true, false);
		}
	}
}
