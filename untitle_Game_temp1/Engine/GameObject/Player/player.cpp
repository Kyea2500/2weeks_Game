#include "player.h"
#include "../../../�v���W�F�N�g�ɒǉ����ׂ��t�@�C��_VC�p/DxLib.h"
#include "../../InputDevice/game.h"
#include "../../InputDevice/Pad/Pad.h"

namespace
{
	// �v���C���[�̏������W
	constexpr float kInitialPosX = 320.0f;
	constexpr float kInitialPosY = 240.0f;
	// �v���C���[�̈ړ����x
	constexpr float kPlayerSpeed = 100.0f;
	constexpr float kEngineBoost = 5.0f;

	// �v���C���[�̃f�b�h�X�s�[�h
	constexpr float kPlayerDeadSpeed = 2.0f;

	// �v���C���[�̔��a
	constexpr int kPlayerRadius = 20;

	// �v���C���[�̐F
	constexpr int kPlayerColor = 0xff2525; // �ԐF


	// �G���W�������̃A�j���[�V�����̃R�}��
	constexpr int kEngineIdleAnimeNum = 2;
	constexpr int kEngineMoveAnimeNum = 3;

	// �����̃A�j���[�V�����̃R�}��
	constexpr int kExplosionAnimeNum = 8;

	// �A�j���[�V�����t���[�����x
	constexpr int kAnimeFrameSpeed = 5;
	constexpr int kExplosionFrameSpeed = 8;

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

	// �v���C���[�̗̑�
	constexpr int kPlayerLifeMax = 3;
	constexpr int kPlayerLifeMidium = 2;
	constexpr int kPlayerLifeLow = 1;

	// �_���[�W���󂯂��Ƃ��̓_�Ńt���[����
	constexpr int kDamageBlinkFrame = 60;

	// �����̃T�C�Y
	constexpr int kExplosionSize = 32;

	// �o���A�̃T�C�Y
	constexpr int kBarrierSize = 88;
	constexpr int kBarrierRadius = 40;

	// �o���A�̎g�p��
	constexpr int kBarrierMaxCount = 3;
	constexpr float kBarrierBlinkFrame = 60.0f; // �o���A�̖��G����
}

player::player()
	: m_pos(0.0f, 0.0f), // �v���C���[�̏������W��ݒ�
	speed(kPlayerSpeed), // �v���C���[�̈ړ����x��ݒ�
	m_playerHandle(-1), // �v���C���[�̉摜�n���h����������
	m_playerDamage1Handle(-1), // �v���C���[�̃_���[�W1�摜�n���h����������
	m_playerDamage2Handle(-1), // �v���C���[�̃_���[�W2�摜�n���h����������
	m_playerDeadHandle(-1), // �v���C���[�̎��S�摜�n���h����������
	m_engineHandleIdle(-1), // �G���W�������̉摜�n���h����������
	m_engineHandleMove(-1), // �G���W�������̈ړ����摜�n���h����������
	m_explosionHandle(-1), // �����G�t�F�N�g�̉摜�n���h����������
	m_barrierHandle(-1), // �o���A�̉摜�n���h����������
	m_animFrame(0), // �A�j���[�V�����t���[����������
	m_explosionAnimFrame(0), // �����G�t�F�N�g�̃A�j���[�V�����t���[����������
	m_isMove(false), // �v���C���[�̈ړ���Ԃ�������
	m_isLastShot(false), // �Ō�Ɍ������e���ǂꂩ��������
	m_isDead(false), // �v���C���[�̎��S��Ԃ�������
	m_isBarrier(false), // �o���A�̏�Ԃ�������
	m_isBarrierHit(false), // �o���A�ōU����h�������ǂ�����������
	m_shotTimer(0), // �v���C���[�̎ˌ��^�C�}�[��������
	m_playerLife(kPlayerLifeMax),//�v���C���[�̗̑͂�������
	m_blinkFrameCount(0), // ���G���Ԃ̏�����
	m_barrierCount(3), // �o���A�̎g�p�񐔂�������
	m_barrierBlinkFrameCount(kBarrierBlinkFrame) // �o���A�̖��G���Ԃ̏�����
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
	m_playerDamage1Handle = LoadGraph("Date/Player/Player_Damage1.png");
	m_playerDamage2Handle = LoadGraph("Date/Player/Player_Damage2.png");
	m_playerDeadHandle = LoadGraph("Date/Player/Player_Damage3.png");
	// �G���W�������̉摜��ǂݍ���
	m_engineHandleIdle = LoadGraph("Date/Effects/Player_IdleEngine.png");
	m_engineHandleMove = LoadGraph("Date/Effects/Player_MoveingEngine.png");

	// �����G�t�F�N�g�̉摜��ǂݍ���
	m_explosionHandle = LoadGraph("Date/Effects/explosion.png");

	// �o���A�̉摜��ǂݍ���
	m_barrierHandle = LoadGraph("Date/Effects/Barrier2.png");

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
	if (m_playerDamage1Handle != -1)
	{
		DeleteGraph(m_playerDamage1Handle);
		m_playerDamage1Handle = -1;
	}
	if (m_playerDamage2Handle != -1)
	{
		DeleteGraph(m_playerDamage2Handle);
		m_playerDamage2Handle = -1;
	}
	if (m_playerDeadHandle != -1)
	{
		DeleteGraph(m_playerDeadHandle);
		m_playerDeadHandle = -1;
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
	// �A�j���[�V�����t���[���̍X�V
	m_animFrame++;
	int totalAnimeFrame = kAnimeFrameSpeed * kEngineIdleAnimeNum;
	if (m_isMove)
	{
		totalAnimeFrame = kAnimeFrameSpeed * kEngineMoveAnimeNum;
	}

	// �A�j���[�V�����t���[�������[�v������
	if (m_animFrame >= totalAnimeFrame)
	{
		m_animFrame = 0;
	}

	


	if (m_playerLife>0)
	{
		// ���͏�Ԃ̍X�V
		Pad::Update();

		// ���G���Ԃ̍X�V
		m_blinkFrameCount--;
		if (m_blinkFrameCount < 0)
		{
			m_blinkFrameCount = 0;
		}
		
		// �v���C���[�̍X�V����
		UpdateMove();
		// �v���C���[�̎ˌ�����
		m_shotTimer++;
		// �Q�[���X�^�[�g�Ɠ����ɒe���o�Ȃ��悤�Ƀ^�C�}�[��u��
		if (m_shotTimer >= kShotTimerMax)
		{
			UpdateShot();
			m_shotTimer = kShotTimerMax; // �^�C�}�[�����������Ȃ��悤�ɗ}��
		}

		// �o���A�̍X�V����
		UpdateBarrier();

		if (Pad::IsPress(PAD_INPUT_3))
		{
			m_playerLife = 0; // �f�o�b�O�p�ɑ̗͂�0�ɂ���
		}


	}
	if (m_playerLife <= 0)
	{
		UpdateDead();
		// �����G�t�F�N�g�̃A�j���[�V�����t���[���̍X�V
		m_explosionAnimFrame++;
		int explosionTotalAnimeFrame = kExplosionFrameSpeed * kExplosionAnimeNum;
		
	}
}

void player::Draw()
{
	DrawPlayer();
	if (m_playerLife > 0)
	{
		DrawEngine();
		DrawShot();
		DrawBarrier();
	}
	if (m_playerLife <= 0)
	{
		DrawDead();
	}
}

float player::PlayerGetPosX()
{
	return m_pos.x - kPlayerImageWidth / half;
}

float player::PlayerGetPosY()
{
	return m_pos.y - kPlayerImageHeight / half;
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
			return m_shotPos[i].x - kShotImageWidth / half;
		}
	}
}

float player::ShotGetPosY()
{
	for(int i=0;i<=MaxShot;i++)
	{
		if (m_isShot[i]==true)
		{
			return m_shotPos[i].y - kShotImageHeight / half;
		}
	}
}

float player::GetShotRadius()
{
	return kShotRadius;
}

void player::HitShot()
{
	for(int i=0;i<=MaxShot;i++)
	{
		if (m_isShot[i]==true)
		{
			m_shotPos[i] = m_pos; // �v���C���[�̈ʒu�ɖ߂�
			m_isShot[i] = false; // �e��������Ă��Ȃ���Ԃɂ���
		}
		// �ŏ��Ɍ������e������������
		break;
	}
}

void player::Damage()
{
	// �_���[�W���󂯂��Ƃ��̏���
	if (m_blinkFrameCount > 0) return;

	if(m_isBarrier)
	{
		// �o���A���L���ȏꍇ�A�_���[�W��h��
		m_isBarrierHit = true; // �o���A�ōU����h�������Ƃ��L�^
		m_isBarrier = false; // �o���A�𖳌���
		m_barrierBlinkFrameCount = 0; // �o���A�̖��G���Ԃ����Z�b�g
		m_barrierCount = 0;	// �o���A�̎g�p�񐔂�0�ɂ���(�܂��x����)
		// �h���̂ɐ��������ꍇ�A�_���[�W��H��킸�ɖ��G���Ԃ���������
		// �������A���G���Ԃ͔����ɂȂ�
		m_blinkFrameCount = kDamageBlinkFrame/half;

		return;
	}
	// ���G����(�_�ł��鎞��)��ݒ肷��
	m_blinkFrameCount = kDamageBlinkFrame;
	// �̗͂�1���炷
	m_playerLife--;
	if (m_playerLife <= 0)
	{
		m_playerLife = 0;
	}
}

void player::UpdateMove()
{
	// �v���C���[�̈ړ�����
	// �ړ��x�N�g����������
	Vec2 move(0.0f, 0.0f);
	m_isMove = false;
	move.x = 0.0f;
	move.y = 0.0f;

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

	move.x *= kEngineBoost; // �u�Ԉړ��̋����𑝂₷
	move.y *= kEngineBoost; // �u�Ԉړ��̋����𑝂₷

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
}

void player::UpdateBarrier()
{
	// �o���A�̍X�V����
	// �o���A���L���ȏꍇ�A���G���Ԃ��J�E���g�_�E��
	if (m_isBarrier)
	{
		m_barrierBlinkFrameCount--;
		// �o���A�͎��Ԑ؂�̎��ɉ��������
		if (m_barrierBlinkFrameCount <= 0)
		{
			m_isBarrier = false; // �o���A�𖳌���
			m_barrierBlinkFrameCount = kBarrierBlinkFrame; // �o���A�̖��G���Ԃ����Z�b�g
		}
	}
	// �o���A�̎g�p����
	if (Pad::IsPress(PAD_INPUT_4))
	{
		// �o���A���g�p�\�ŁA�o���A���Q�[�����Ɉ�x�ł��h���ł��炸�A�o���A���L���łȂ��ꍇ�Ƀo���A��L����
		if (m_barrierCount > 0&&m_isBarrier == false)
		{
			m_isBarrier = true; // �o���A��L����
			m_barrierCount--; // �o���A�̎g�p�񐔂����炷
		}
	}

}

void player::DrawPlayer()
{
	// �̗͂����F�ł���悤��
	DrawFormatString(10, 10, GetColor(255, 255, 255), "Player Life:%d", m_playerLife);

	// �v���C���[�̓_�ŏ���
	if (((int)m_blinkFrameCount / (int)half) % 2 == 1)
	{
		return;
	}

	// �`�揈��
	// �v���C���[�̕`�揈��
	DrawCircle(m_pos.x, m_pos.y, kPlayerRadius, kPlayerColor, FALSE); // �Ԃ��~��`��

	if (m_playerLife == kPlayerLifeMax)
	{
		// �~�̒��S�ɕ`��
		DrawRotaGraph(m_pos.x, m_pos.y, kPlayerScale, kPlayerAngle, m_playerHandle, TRUE);
	}
	else if (m_playerLife == kPlayerLifeMidium)
	{
		// �~�̒��S�ɕ`��
		DrawRotaGraph(m_pos.x, m_pos.y, kPlayerScale, kPlayerAngle, m_playerDamage1Handle, TRUE);
	}
	else if (m_playerLife == kPlayerLifeLow)
	{
		// �~�̒��S�ɕ`��
		DrawRotaGraph(m_pos.x, m_pos.y, kPlayerScale, kPlayerAngle, m_playerDamage2Handle, TRUE);
	}


	else if (m_playerLife <= 0)
	{
		// �~�̒��S�ɕ`��
		DrawRotaGraph(m_pos.x, m_pos.y, kPlayerScale, kPlayerAngle, m_playerDeadHandle, TRUE);
	}
}

void player::DrawEngine()
{
	if (((int)m_blinkFrameCount / (int)half) % 2 == 1)
	{
		return;
	}
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

void player::DrawBarrier()
{
	// �o���A�̎g�p�񐔂����F�ł���悤��
	if (m_barrierCount > 0)
	{
		DrawFormatString(10, 50, GetColor(255, 255, 255), "�o���A�͎c��%d��܂Ŏg�p�\", m_barrierCount);
	}

	// �o���A���g�p�ł��Ȃ��ꍇ�̕\��
	if(m_barrierCount==0&&m_isBarrierHit==false)
	{
		DrawFormatString(10, 50, GetColor(255, 255, 255), "�o���A���g���؂�܂����B�o���A���g�p�ł��܂���");
	}

	// �o���A���U����h�����ꍇ�̕\��
	if (m_isBarrierHit)
	{
		DrawFormatString(10, 50, GetColor(255, 255, 255), "�o���A�ōU����h���܂����B�o���A���g�p�ł��܂���B");
	}

	// �o���A���g�p���̕\������
	if(m_isBarrier&&m_isBarrierHit==false)
	{
		DrawFormatString(10, 70, GetColor(255, 255, 255), "�o���A�W�J");
	}


	// �o���A�̕`�揈��
	if (m_isBarrier)
	{
		// �o���A�̓_�ŏ���
		if (((int)m_barrierBlinkFrameCount / (int)half) % 2 == 1)
		{
			return;
		}
		// �o���A�̉摜��`��
		if (m_barrierHandle != -1)
		{
			DrawRectRotaGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
				0, 0, kBarrierSize, kBarrierSize, 1.5, 0.0f,
				m_barrierHandle, TRUE);
		}
		// �o���A�̓����蔻���`��
		DrawCircle(m_pos.x, m_pos.y, kBarrierRadius, 0x00ffff, FALSE); // ���F�̉~��`��
	}
}

void player::UpdateDead()
{
	// ���˂���Ă���e�͂��ׂă��Z�b�g
	for (int i = 0; i <= MaxShot; i++)
	{
		if (m_isShot[i] == true)
		{
			m_shotPos[i] = m_pos; // �v���C���[�̈ʒu�ɖ߂�
			m_isShot[i] = false; // �e��������Ă��Ȃ���Ԃɂ���
		}
	}

	// �v���C���[�̈ړ�����
	Vec2 move(0.0f, 0.0f);
	move.x -= kSpeed; // ���Ɉړ�
	move.y += kSpeed; // ���Ɉړ�
	// �ړ��x�N�g���𐳋K��
	if (move.Length() > 0.0f)
	{
		move.Normalize();
	}
	move.x *= kEngineBoost; // �u�Ԉړ��̋����𑝂₷
	move.y *= kEngineBoost; // �u�Ԉړ��̋����𑝂₷

	// �v���C���[�̈ʒu���X�V
	m_pos += move;
	if (m_pos.x < -kPlayerImageWidth || m_pos.y > Game::kScreenHeight + kPlayerImageHeight)
	{
		// ��ʊO�ɏo���玀�S��Ԃ�true�ɂ���
		m_isDead = true;
	}
}

void player::DrawDead()
{
	// �v���C���[�����S�������Ƃ�\��
	DrawFormatString(10, 50, GetColor(255, 255, 255), "���Ă���Ă��܂��܂���");
	// �����̃A�j���[�V��������
	int animeIndex = m_explosionAnimFrame / kExplosionAnimeNum;
	// �v���C���[�̈ʒu�ɔ����n���h����`��
	if (m_explosionHandle != -1)
	{
		DrawRectRotaGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
			animeIndex * kExplosionSize, 0, kExplosionSize, kExplosionSize, kPlayerScale, 0.0, m_explosionHandle, TRUE);
	}
	
}


