#include "SceneMain.h"
#include "DxLib.h"
#include "../../InputDevice/game.h"
#include "../../GameObject/Player/player.h"
#include "../../GameObject/Enemy/enemy.h"
#include "../../GameObject/Bg/Bg.h"
#include <cmath>

namespace
{
	// �����ɂ���
	constexpr int half = 2;
	constexpr int kMaxShot = 20;
}


SceneMain::SceneMain()
	: m_pPlayer(nullptr), // �v���C���[�̃|�C���^��������
	m_pEnemy(nullptr),    // �G�̃|�C���^��������
	m_pBg(nullptr)       // �w�i�̃|�C���^��������
{
}

SceneMain::~SceneMain()
{
}

void SceneMain::Init()
{
	// �v���C���[�̏�����
	m_pPlayer = std::make_shared<player>();
	m_pPlayer->Init();
	// �G�̏�����
	m_pEnemy = std::make_shared<enemy>();
	m_pEnemy->Init();
	// �w�i�̏�����
	m_pBg = std::make_shared<Bg>();
	m_pBg->Init();
}

void SceneMain::End()
{
}

SceneManager::SceneKind SceneMain::Update()
{
	// ���͏�Ԃ̍X�V
	Pad::Update();
	// �w�i�̍X�V����
	m_pBg->Update();
	// �v���C���[�̍X�V����
	m_pPlayer->Update();
	// �G�̍X�V����
	m_pEnemy->Update();

	HitPlayerEnemy();
	HitPlayerShot();

	// �v���C���[�̗̑͂�0�ȉ��Ȃ�Q�[���I�[�o�[��
	if(m_pPlayer->GatPlayerDead()==true)
	{
		// �V�[�����Q�[���I�[�o�[�ɕύX
		return SceneManager::SceneKind::kSceneGameOver;
	}


	// �G�̗̑͂�0�ȉ��Ȃ�Q�[���N���A��
	if(m_pEnemy->GetEnemyDead()==true)
	{
		// �V�[�����Q�[���N���A�ɕύX
		return SceneManager::SceneKind::kSceneGameClear;
	}

	return SceneManager::SceneKind::kSceneMain;

}

void SceneMain::Draw()
{
	// �w�i�̕`�揈��
	m_pBg->Draw();
	// �v���C���[�̕`�揈��
	m_pPlayer->Draw();
	// �G�̕`�揈��
	m_pEnemy->Draw();
}

void SceneMain::HitPlayerShot()
{
	float dx = m_pPlayer->ShotGetPosX() - m_pEnemy->EnemyGetPosX();
	float dy = m_pPlayer->ShotGetPosY() - m_pEnemy->EnemyGetPosY();
	float distance = sqrtf(dx * dx + dy * dy);
	if (distance <= m_pPlayer->GetShotRadius() + m_pEnemy->GetEnemyRadius())
	{
		// �Փ˂����ꍇ�̏���
		// �����ɏՓˎ��̏�����ǉ�
		m_pPlayer->HitShot();
		m_pEnemy->EnemysDamage();
	}
		
	
}

void SceneMain::HitPlayerEnemy()
{
	float dx = m_pPlayer->PlayerGetPosX() - m_pEnemy->EnemyGetPosX();
	float dy = m_pPlayer->PlayerGetPosY() - m_pEnemy->EnemyGetPosY();
	float distance = sqrtf(dx * dx + dy * dy);
	if (distance <= m_pPlayer->GetPlayerRadius() + m_pEnemy->GetEnemyRadius())
	{
		// �Փ˂����ꍇ�̏���
		// �����ɏՓˎ��̏�����ǉ�
		m_pPlayer->Damage();
	}
}
