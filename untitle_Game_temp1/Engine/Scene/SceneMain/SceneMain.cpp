#include "SceneMain.h"
#include "DxLib.h"
#include "../../InputDevice/game.h"
#include "../../GameObject/Player/player.h"
#include "../../GameObject/Enemy/enemy.h"
#include "../../GameObject/Bg/Bg.h"
#include <cmath>

namespace
{
	// 半分にする
	constexpr int half = 2;
	constexpr int kMaxShot = 20;
}


SceneMain::SceneMain()
	: m_pPlayer(nullptr), // プレイヤーのポインタを初期化
	m_pEnemy(nullptr),    // 敵のポインタを初期化
	m_pBg(nullptr)       // 背景のポインタを初期化
{
}

SceneMain::~SceneMain()
{
}

void SceneMain::Init()
{
	// プレイヤーの初期化
	m_pPlayer = std::make_shared<player>();
	m_pPlayer->Init();
	// 敵の初期化
	m_pEnemy = std::make_shared<enemy>();
	m_pEnemy->Init();
	// 背景の初期化
	m_pBg = std::make_shared<Bg>();
	m_pBg->Init();
}

void SceneMain::End()
{
}

SceneManager::SceneKind SceneMain::Update()
{
	// 入力状態の更新
	Pad::Update();
	// 背景の更新処理
	m_pBg->Update();
	// プレイヤーの更新処理
	m_pPlayer->Update();
	// 敵の更新処理
	m_pEnemy->Update();

	HitPlayerEnemy();
	HitPlayerShot();

	// プレイヤーの体力が0以下ならゲームオーバーへ
	if(m_pPlayer->GatPlayerDead()==true)
	{
		// シーンをゲームオーバーに変更
		return SceneManager::SceneKind::kSceneGameOver;
	}


	// 敵の体力が0以下ならゲームクリアへ
	if(m_pEnemy->GetEnemyDead()==true)
	{
		// シーンをゲームクリアに変更
		return SceneManager::SceneKind::kSceneGameClear;
	}

	return SceneManager::SceneKind::kSceneMain;

}

void SceneMain::Draw()
{
	// 背景の描画処理
	m_pBg->Draw();
	// プレイヤーの描画処理
	m_pPlayer->Draw();
	// 敵の描画処理
	m_pEnemy->Draw();
}

void SceneMain::HitPlayerShot()
{
	float dx = m_pPlayer->ShotGetPosX() - m_pEnemy->EnemyGetPosX();
	float dy = m_pPlayer->ShotGetPosY() - m_pEnemy->EnemyGetPosY();
	float distance = sqrtf(dx * dx + dy * dy);
	if (distance <= m_pPlayer->GetShotRadius() + m_pEnemy->GetEnemyRadius())
	{
		// 衝突した場合の処理
		// ここに衝突時の処理を追加
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
		// 衝突した場合の処理
		// ここに衝突時の処理を追加
		m_pPlayer->Damage();
	}
}
