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
private:
	// �G�̈ʒu
	Vec2 m_pos;
	// �G�̑��x
	int m_speed;
	int m_enemyHandle;

};