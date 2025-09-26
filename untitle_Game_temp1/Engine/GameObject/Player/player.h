#pragma once
#include "../../InputDevice/Vec2.h"
class player
{
public:
	// �R���X�g���N�^
	player();
	// �f�X�g���N�^
	~player();
	// ����������
	void Init();
	// �I������
	void End();
	// �X�V����
	void Update();
	// �`�揈��
	void Draw();

public:
	// �v���C���[�̓����蔻�������
	// �K�v�Ȃ̂́A�v���C���[��XY���W�Ɣ��a
	float PlayerGetPosX();
	float PlayerGetPosY();
	float GetPlayerRadius();

	// �e�̓����蔻�������
	float ShotGetPosX();
	float ShotGetPosY();
	float GetShotRadius();

	void HitShot();
	void Damage();

	bool GatPlayerDead() { return  m_isDead; }

private:
	void UpdateMove();
	void UpdateShot();
	void UpdateBarrier();

	void DrawPlayer();
	void DrawEngine();
	void DrawShot();
	void DrawBarrier();

	void UpdateDead();
	void DrawDead();

private:
	// �v���C���[�̍��W
	Vec2 m_pos;
	// �v���C���[�̉摜����
	int m_playerHandle;
	int m_playerDamage1Handle;
	int m_playerDamage2Handle;
	int m_playerDeadHandle;
	//�@�G���W�������̉摜�n���h��
	int m_engineHandleIdle;
	int m_engineHandleMove;

	int m_explosionHandle; // �����G�t�F�N�g�̉摜�n���h��

	// �o���A�̉摜�n���h��
	int m_barrierHandle;

	// �A�j���[�V�����֘A
	int m_animFrame; // �t���[�����𐔂���
	int m_explosionAnimFrame; // �����G�t�F�N�g�̃A�j���[�V�����t���[����

	// �v���C���[�̈ړ����x
	float speed;
	// �v���C���[�̈ړ����
	bool m_isMove;	// true�Ȃ�ړ���		false�Ȃ�ړ����Ă��Ȃ�

	// �e�̍��W
	Vec2 m_shotPos[21];
	// �e�̉摜����
	int m_shotHandle[21];
	// �v���C���[�̎ˌ�����
	bool m_isShot[21]; // true�Ȃ�ˌ���		false�Ȃ�ˌ����Ă��Ȃ�
	bool m_isLastShot; // �Ō�Ɍ������e���ǂꂩ��ۑ�����

	// �Q�[���J�n���ɒe���o�Ȃ��悤�A�������������Ԃ��󂯂�
	int m_shotTimer;

	// �v���C���[��HP
	int m_playerLife;
	bool m_isDead; // �v���C���[������ł��邩�ǂ���

	// ���G���Ԃ̐ݒ�
	int m_blinkFrameCount;

	// �o���A�̗L��
	bool m_isBarrier;
	// �o���A�̎g�p��
	int m_barrierCount;
	// �o���A�̖��G����
	float m_barrierBlinkFrameCount;
	// �o���A�ōU����h�������ǂ���
	bool m_isBarrierHit;
};