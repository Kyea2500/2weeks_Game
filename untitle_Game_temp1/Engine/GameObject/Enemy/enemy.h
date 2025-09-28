#pragma once
#include "../../InputDevice/Vec2.h"
class enemy
{
public:
	// �R���X�g���N�^
	enemy();
	// �f�X�g���N�^
	~enemy();
	// ����������
	void Init();
	// �I������
	void End();
	// �X�V����
	void Update();
	// �`�揈��
	void Draw();
public:
	// �����蔻��̎���
	float EnemyGetPosX();
	float EnemyGetPosY();
	float GetEnemyRadius();

	// �G���G�̓����蔻��
	float EnemyUnitGetPosX();
	float EnemyUnitGetPosY();
	float GetEnemyUnitRadius();

	// �������^�C�v�̎G���G�̓����蔻��
	float EnemyMiniUnitGetPosX();
	float EnemyMiniUnitGetPosY();
	float GetEnemyMiniUnitRadius();

	// �G���_���[�W���󂯂��Ƃ��̏���
	void EnemysDamage();
	// �G���G���_���[�W���󂯂��Ƃ��̏���(����)	
	void EnemyUnitsDamage();

	// bosus�̗̑͂��擾
	int GetEnemyLife() { return m_Life; }
	bool GetEnemyDead() { return m_Dead; }

private:
	void UpdateEnemy();
	void UpdateEnemyUnit();
	void UpdateEnemyMiniUnit();

	void DrawEnemy();
	void DrawEnemyUnit();
	void DrawEnemyMiniUnit();

	void UpdateEnemyBullet();
	void DrawEnemyBullet();

	void UpdateDead();
	void DrawDead();

private:
	// �G�̈ʒu
	Vec2 m_pos;
	// �G�̑��x
	int m_speed;
	// �G�̉摜
	int m_enemyHandle;
	// �G�̗̑�
	int m_Life;
	bool m_isEnemyUnitReleased; // �Ō�̎�i�̎G���G����x�ł������ꂽ���ǂ���

	// �G���G
	// �G���G�̈ʒu
	Vec2 m_enemyUnitPos[2];
	// �G���G�̉摜
	int m_enemyUnitHandle[2];
	// �������^�C�v�̎G���G
	// �������^�C�v�̎G���G�̈ʒu
	Vec2 m_enemyMiniUnitPos[3];
	// �������^�C�v�̎G���G�̉摜
	int m_enemyMiniUnitHandle[3];

	// ����
	int m_explosionHandle; // �����G�t�F�N�g�̉摜�n���h��

	// �����̃A�j���[�V�����t���[��
	int m_explosionAnimFrame;

	// �e�̉摜
	int m_enemyBulletHandle[3];
	// �e�̈ʒu
	Vec2 m_enemyBulletPos[3];
	// �e�̑��x
	int m_enemyBulletSpeed;
	// �e�������ꂽ���ǂ���
	bool m_isEnemyBullet[3];
	// �e�̔��ˊԊu�^�C�}�[
	int m_enemyBulletTimer;

	// ���G���Ԃ̐ݒ�
	int m_blinkFrameCount;

	bool m_Dead; // �G������ł��邩�ǂ���
	// �G�����񂾌�Ɉ�x��Ɍ���������
	bool m_isDeadUpOnce;
};