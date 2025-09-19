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
private:
	// 敵の位置
	Vec2 m_pos;
	// 敵の速度
	int m_speed;
	int m_enemyHandle;

};