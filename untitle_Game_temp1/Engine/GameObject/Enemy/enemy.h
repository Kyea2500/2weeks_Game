#pragma once
#include "../../InputDevice/Vec2.h"
class enemy
{
public:
	// コンストラクタ
	enemy();
	// デストラクタ
	~enemy();
	// 初期化処理
	void Init();
	// 終了処理
	void End();
	// 更新処理
	void Update();
	// 描画処理
	void Draw();
public:
	// 当たり判定の実装
	float EnemyGetPosX();
	float EnemyGetPosY();
	float GetEnemyRadius();

	// 雑魚敵の当たり判定
	float EnemyUnitGetPosX();
	float EnemyUnitGetPosY();
	float GetEnemyUnitRadius();

	// 小さいタイプの雑魚敵の当たり判定
	float EnemyMiniUnitGetPosX();
	float EnemyMiniUnitGetPosY();
	float GetEnemyMiniUnitRadius();

	// 敵がダメージを受けたときの処理
	void EnemysDamage();
	// 雑魚敵がダメージを受けたときの処理(共通)	
	void EnemyUnitsDamage();

	// bosusの体力を取得
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
	// 敵の位置
	Vec2 m_pos;
	// 敵の速度
	int m_speed;
	// 敵の画像
	int m_enemyHandle;
	// 敵の体力
	int m_Life;
	bool m_isEnemyUnitReleased; // 最後の手段の雑魚敵が一度でも放たれたかどうか

	// 雑魚敵
	// 雑魚敵の位置
	Vec2 m_enemyUnitPos[2];
	// 雑魚敵の画像
	int m_enemyUnitHandle[2];
	// 小さいタイプの雑魚敵
	// 小さいタイプの雑魚敵の位置
	Vec2 m_enemyMiniUnitPos[3];
	// 小さいタイプの雑魚敵の画像
	int m_enemyMiniUnitHandle[3];

	// 爆発
	int m_explosionHandle; // 爆発エフェクトの画像ハンドル

	// 爆発のアニメーションフレーム
	int m_explosionAnimFrame;

	// 弾の画像
	int m_enemyBulletHandle[3];
	// 弾の位置
	Vec2 m_enemyBulletPos[3];
	// 弾の速度
	int m_enemyBulletSpeed;
	// 弾が撃たれたかどうか
	bool m_isEnemyBullet[3];
	// 弾の発射間隔タイマー
	int m_enemyBulletTimer;

	// 無敵時間の設定
	int m_blinkFrameCount;

	bool m_Dead; // 敵が死んでいるかどうか
	// 敵が死んだ後に一度上に向かったか
	bool m_isDeadUpOnce;
};