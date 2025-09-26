#pragma once
#include "../../InputDevice/Vec2.h"
class player
{
public:
	// コンストラクタ
	player();
	// デストラクタ
	~player();
	// 初期化処理
	void Init();
	// 終了処理
	void End();
	// 更新処理
	void Update();
	// 描画処理
	void Draw();

public:
	// プレイヤーの当たり判定を実装
	// 必要なのは、プレイヤーのXY座標と半径
	float PlayerGetPosX();
	float PlayerGetPosY();
	float GetPlayerRadius();

	// 弾の当たり判定を実装
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
	// プレイヤーの座標
	Vec2 m_pos;
	// プレイヤーの画像処理
	int m_playerHandle;
	int m_playerDamage1Handle;
	int m_playerDamage2Handle;
	int m_playerDeadHandle;
	//　エンジン部分の画像ハンドル
	int m_engineHandleIdle;
	int m_engineHandleMove;

	int m_explosionHandle; // 爆発エフェクトの画像ハンドル

	// バリアの画像ハンドル
	int m_barrierHandle;

	// アニメーション関連
	int m_animFrame; // フレーム数を数える
	int m_explosionAnimFrame; // 爆発エフェクトのアニメーションフレーム数

	// プレイヤーの移動速度
	float speed;
	// プレイヤーの移動状態
	bool m_isMove;	// trueなら移動中		falseなら移動していない

	// 弾の座標
	Vec2 m_shotPos[21];
	// 弾の画像処理
	int m_shotHandle[21];
	// プレイヤーの射撃処理
	bool m_isShot[21]; // trueなら射撃中		falseなら射撃していない
	bool m_isLastShot; // 最後に撃った弾がどれかを保存する

	// ゲーム開始時に弾が出ないよう、すこしだけ時間を空ける
	int m_shotTimer;

	// プレイヤーのHP
	int m_playerLife;
	bool m_isDead; // プレイヤーが死んでいるかどうか

	// 無敵時間の設定
	int m_blinkFrameCount;

	// バリアの有無
	bool m_isBarrier;
	// バリアの使用回数
	int m_barrierCount;
	// バリアの無敵時間
	float m_barrierBlinkFrameCount;
	// バリアで攻撃を防いだかどうか
	bool m_isBarrierHit;
};