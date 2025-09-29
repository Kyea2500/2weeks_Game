#include "player.h"
#include "../../../プロジェクトに追加すべきファイル_VC用/DxLib.h"
#include "../../InputDevice/game.h"
#include "../../InputDevice/Pad/Pad.h"

namespace
{
	// プレイヤーの初期座標
	constexpr float kInitialPosX = 320.0f;
	constexpr float kInitialPosY = 240.0f;
	// プレイヤーの移動速度
	constexpr float kPlayerSpeed = 100.0f;
	constexpr float kEngineBoost = 5.0f;

	// プレイヤーのデッドスピード
	constexpr float kPlayerDeadSpeed = 2.0f;

	// プレイヤーの半径
	constexpr int kPlayerRadius = 20;

	// プレイヤーの色
	constexpr int kPlayerColor = 0xff2525; // 赤色


	// エンジン部分のアニメーションのコマ数
	constexpr int kEngineIdleAnimeNum = 2;
	constexpr int kEngineMoveAnimeNum = 3;

	// 爆発のアニメーションのコマ数
	constexpr int kExplosionAnimeNum = 8;

	// アニメーションフレーム速度
	constexpr int kAnimeFrameSpeed = 5;
	constexpr int kExplosionFrameSpeed = 8;

	// プレイヤーの画像サイズ
	constexpr int kPlayerImageWidth = 48;
	constexpr int kPlayerImageHeight = 48;

	// プレイヤーの拡大率
	constexpr double kPlayerScale = 2.5;
	// プレイヤーの回転角度
	constexpr double kPlayerAngle = 1.6;

	// エンジンの位置調整
	constexpr int kEngineOffsetX = 15;
	constexpr int kEngineOffsetY = 30;

	// 半分にする
	constexpr int half = 2;

	// 弾が出ない時間最大値
	constexpr float kShotTimerMax = 20.0f;

	// 弾関連
	constexpr int kShotImageWidth = 76;
	constexpr int kShotImageHeight = 98;
	// 弾の半径
	constexpr int kShotRadius = 10;
	// 弾の当たり判定の色
	constexpr int kShotColor = 0xffff00; // 黄色


	constexpr float kSpeed = 10.0f;
	// 拡大・縮小率
	constexpr float kScale = 0.5f;
	constexpr int MaxShot = 20;

	// プレイヤーの体力
	constexpr int kPlayerLifeMax = 3;
	constexpr int kPlayerLifeMidium = 2;
	constexpr int kPlayerLifeLow = 1;

	// ダメージを受けたときの点滅フレーム数
	constexpr int kDamageBlinkFrame = 60;

	// 爆発のサイズ
	constexpr int kExplosionSize = 32;

	// バリアのサイズ
	constexpr int kBarrierSize = 88;
	constexpr int kBarrierRadius = 40;

	// バリアの使用回数
	constexpr int kBarrierMaxCount = 3;
	constexpr float kBarrierBlinkFrame = 60.0f; // バリアの無敵時間
}

player::player()
	: m_pos(0.0f, 0.0f), // プレイヤーの初期座標を設定
	speed(kPlayerSpeed), // プレイヤーの移動速度を設定
	m_playerHandle(-1), // プレイヤーの画像ハンドルを初期化
	m_playerDamage1Handle(-1), // プレイヤーのダメージ1画像ハンドルを初期化
	m_playerDamage2Handle(-1), // プレイヤーのダメージ2画像ハンドルを初期化
	m_playerDeadHandle(-1), // プレイヤーの死亡画像ハンドルを初期化
	m_engineHandleIdle(-1), // エンジン部分の画像ハンドルを初期化
	m_engineHandleMove(-1), // エンジン部分の移動中画像ハンドルを初期化
	m_explosionHandle(-1), // 爆発エフェクトの画像ハンドルを初期化
	m_barrierHandle(-1), // バリアの画像ハンドルを初期化
	m_animFrame(0), // アニメーションフレームを初期化
	m_explosionAnimFrame(0), // 爆発エフェクトのアニメーションフレームを初期化
	m_isMove(false), // プレイヤーの移動状態を初期化
	m_isLastShot(false), // 最後に撃った弾がどれかを初期化
	m_isDead(false), // プレイヤーの死亡状態を初期化
	m_isBarrier(false), // バリアの状態を初期化
	m_isBarrierHit(false), // バリアで攻撃を防いだかどうかを初期化
	m_shotTimer(0), // プレイヤーの射撃タイマーを初期化
	m_playerLife(kPlayerLifeMax),//プレイヤーの体力を初期化
	m_blinkFrameCount(0), // 無敵時間の初期化
	m_barrierCount(3), // バリアの使用回数を初期化
	m_barrierBlinkFrameCount(kBarrierBlinkFrame) // バリアの無敵時間の初期化
{
	for (int i = 0; i <= MaxShot; i++)
	{
		m_shotPos[i] = Vec2(m_pos); // 弾の初期座標を設定
		m_shotHandle[i] = -1; // 弾の画像ハンドルを初期化
		m_isShot[i] = false; // プレイヤーの射撃状態を初期化
	}
}

player::~player()
{
}

void player::Init()
{
	// プレイヤーの初期座標を設定
	m_pos.x = kInitialPosX; // 画面の中央に配置
	m_pos.y = kInitialPosY; // 画面の中央に配置
	// プレイヤーの画像を読み込む
	m_playerHandle = LoadGraph("Date/Player/Player_Full.png");
	m_playerDamage1Handle = LoadGraph("Date/Player/Player_Damage1.png");
	m_playerDamage2Handle = LoadGraph("Date/Player/Player_Damage2.png");
	m_playerDeadHandle = LoadGraph("Date/Player/Player_Damage3.png");
	// エンジン部分の画像を読み込む
	m_engineHandleIdle = LoadGraph("Date/Effects/Player_IdleEngine.png");
	m_engineHandleMove = LoadGraph("Date/Effects/Player_MoveingEngine.png");

	// 爆発エフェクトの画像を読み込む
	m_explosionHandle = LoadGraph("Date/Effects/explosion.png");

	// バリアの画像を読み込む
	m_barrierHandle = LoadGraph("Date/Effects/Barrier2.png");

	m_animFrame = 0; // アニメーションフレームを初期化
	m_isMove = false; // プレイヤーの移動状態を初期化
	m_shotTimer = 0; // プレイヤーの射撃タイマーを初期化

	for (int i = 0; i <= MaxShot; i++)
	{
		m_shotPos[i] = Vec2(0.0f, 0.0f); // 弾の初期座標を設定
		m_shotHandle[i] = LoadGraph("Date/Player/Player_shot_1.png"); // 弾の画像を読み込む
		m_isShot[i] = false; // プレイヤーの射撃状態を初期化
	}
}

void player::End()
{
	// プレイヤーの画像を解放
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



	// エンジン部分の画像を解放
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

	// 弾の画像を解放
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
	// アニメーションフレームの更新
	m_animFrame++;
	int totalAnimeFrame = kAnimeFrameSpeed * kEngineIdleAnimeNum;
	if (m_isMove)
	{
		totalAnimeFrame = kAnimeFrameSpeed * kEngineMoveAnimeNum;
	}

	// アニメーションフレームをループさせる
	if (m_animFrame >= totalAnimeFrame)
	{
		m_animFrame = 0;
	}

	


	if (m_playerLife>0)
	{
		// 入力状態の更新
		Pad::Update();

		// 無敵時間の更新
		m_blinkFrameCount--;
		if (m_blinkFrameCount < 0)
		{
			m_blinkFrameCount = 0;
		}
		
		// プレイヤーの更新処理
		UpdateMove();
		// プレイヤーの射撃処理
		m_shotTimer++;
		// ゲームスタートと同時に弾が出ないようにタイマーを置く
		if (m_shotTimer >= kShotTimerMax)
		{
			UpdateShot();
			m_shotTimer = kShotTimerMax; // タイマーが増えすぎないように抑制
		}

		// バリアの更新処理
		UpdateBarrier();

		if (Pad::IsPress(PAD_INPUT_3))
		{
			m_playerLife = 0; // デバッグ用に体力を0にする
		}


	}
	if (m_playerLife <= 0)
	{
		UpdateDead();
		// 爆発エフェクトのアニメーションフレームの更新
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
			m_shotPos[i] = m_pos; // プレイヤーの位置に戻す
			m_isShot[i] = false; // 弾が撃たれていない状態にする
		}
		// 最初に見つけた弾だけ処理する
		break;
	}
}

void player::Damage()
{
	// ダメージを受けたときの処理
	if (m_blinkFrameCount > 0) return;

	if(m_isBarrier)
	{
		// バリアが有効な場合、ダメージを防ぐ
		m_isBarrierHit = true; // バリアで攻撃を防いだことを記録
		m_isBarrier = false; // バリアを無効化
		m_barrierBlinkFrameCount = 0; // バリアの無敵時間をリセット
		m_barrierCount = 0;	// バリアの使用回数を0にする(つまり一度だけ)
		// 防ぐのに成功した場合、ダメージを食らわずに無敵時間が発生する
		// ただし、無敵時間は半分になる
		m_blinkFrameCount = kDamageBlinkFrame/half;

		return;
	}
	// 無敵時間(点滅する時間)を設定する
	m_blinkFrameCount = kDamageBlinkFrame;
	// 体力を1減らす
	m_playerLife--;
	if (m_playerLife <= 0)
	{
		m_playerLife = 0;
	}
}

void player::UpdateMove()
{
	// プレイヤーの移動処理
	// 移動ベクトルを初期化
	Vec2 move(0.0f, 0.0f);
	m_isMove = false;
	move.x = 0.0f;
	move.y = 0.0f;

	// プレイヤーの移動方向の向きを初期化
	if (Pad::IsPress(PAD_INPUT_LEFT))
	{
		// 左に移動
		move.x -= speed;
		m_isMove = true;
	}
	if (Pad::IsPress(PAD_INPUT_DOWN))
	{
		// 下に移動
		move.y += speed;
		m_isMove = true;
	}
	if (Pad::IsPress(PAD_INPUT_RIGHT))
	{
		// 右に移動
		move.x += speed;
		m_isMove = true;
	}
	if (Pad::IsPress(PAD_INPUT_UP))
	{
		// 上に移動
		move.y -= speed;
		m_isMove = true;
	}
	// 移動ベクトルを正規化
	if (move.Length() > 0.0f)
	{
		move.Normalize();
	}

	move.x *= kEngineBoost; // 瞬間移動の距離を増やす
	move.y *= kEngineBoost; // 瞬間移動の距離を増やす

	// プレイヤーの位置を更新
	m_pos += move;

	// 画面外に出ないように制限
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
	// 弾を撃つ処理
	if (Pad::IsPress(PAD_INPUT_A))
	{
		// プレイヤーの射撃処理
		for (int i = 0; i <= MaxShot; i++)
		{
			// 前のフレームで押されていたかの確認。
			if (m_isLastShot == false)
			{ 				// 前のフレームで押されていなかったら次のループへ
				if (m_isShot[i] == true)
				{
					// 最後に撃った弾があれば次の弾を撃つ
					continue; // 次のループへ
				}
				m_isShot[i] = true;
				m_shotPos[i] = Vec2(m_pos); // 弾の位置をプレイヤーの位置に設定
				m_isLastShot = true; // 最後に撃った弾を保存
				break; // 1発だけ撃つ
			}

			else
			{
				break; // 前のフレームで押されていたら何もしない
			}

			m_isLastShot = true; // 前のフレームで押されていたら最後に撃った弾を保存
		}
	}

	else
	{
		// Aボタンが押されていなければ最後に撃った弾の保存をリセット
		m_isLastShot= false;
	}

	// 弾の更新処理
	for(int i = 0; i <= MaxShot; i++)
	{
		if(m_isShot[i]==true)
		{
			// 弾の位置を更新
			m_shotPos[i].x += 15.0f; // 右方向に移動
			// 画面外に出たら弾をリセット
			if (m_shotPos[i].x > Game::kScreenWidth)
			{
				m_shotPos[i] = m_pos; // プレイヤーの位置に戻す
				m_isShot[i] = false; // 弾が撃たれていない状態にする
				
			}
		}
	}
}

void player::UpdateBarrier()
{
	// バリアの更新処理
	// バリアが有効な場合、無敵時間をカウントダウン
	if (m_isBarrier)
	{
		m_barrierBlinkFrameCount--;
		// バリアは時間切れの時に解除される
		if (m_barrierBlinkFrameCount <= 0)
		{
			m_isBarrier = false; // バリアを無効化
			m_barrierBlinkFrameCount = kBarrierBlinkFrame; // バリアの無敵時間をリセット
		}
	}
	// バリアの使用処理
	if (Pad::IsPress(PAD_INPUT_4))
	{
		// バリアが使用可能で、バリアがゲーム中に一度でも防いでおらず、バリアが有効でない場合にバリアを有効化
		if (m_barrierCount > 0&&m_isBarrier == false)
		{
			m_isBarrier = true; // バリアを有効化
			m_barrierCount--; // バリアの使用回数を減らす
		}
	}

}

void player::DrawPlayer()
{
	// 体力を視認できるように
	DrawFormatString(10, 10, GetColor(255, 255, 255), "Player Life:%d", m_playerLife);

	// プレイヤーの点滅処理
	if (((int)m_blinkFrameCount / (int)half) % 2 == 1)
	{
		return;
	}

	// 描画処理
	// プレイヤーの描画処理
	DrawCircle(m_pos.x, m_pos.y, kPlayerRadius, kPlayerColor, FALSE); // 赤い円を描画

	if (m_playerLife == kPlayerLifeMax)
	{
		// 円の中心に描画
		DrawRotaGraph(m_pos.x, m_pos.y, kPlayerScale, kPlayerAngle, m_playerHandle, TRUE);
	}
	else if (m_playerLife == kPlayerLifeMidium)
	{
		// 円の中心に描画
		DrawRotaGraph(m_pos.x, m_pos.y, kPlayerScale, kPlayerAngle, m_playerDamage1Handle, TRUE);
	}
	else if (m_playerLife == kPlayerLifeLow)
	{
		// 円の中心に描画
		DrawRotaGraph(m_pos.x, m_pos.y, kPlayerScale, kPlayerAngle, m_playerDamage2Handle, TRUE);
	}


	else if (m_playerLife <= 0)
	{
		// 円の中心に描画
		DrawRotaGraph(m_pos.x, m_pos.y, kPlayerScale, kPlayerAngle, m_playerDeadHandle, TRUE);
	}
}

void player::DrawEngine()
{
	if (((int)m_blinkFrameCount / (int)half) % 2 == 1)
	{
		return;
	}
	// プレイヤーのアニメーション処理
	int animeIndex = m_animFrame / kEngineIdleAnimeNum;
	// 移動中ならエンジンのアニメーションを変更
	if (m_isMove)
	{
		animeIndex = m_animFrame / kEngineMoveAnimeNum;
	}

	// 使用するエンジン部分の画像ハンドルを決定
	int useHandle = m_engineHandleIdle;
	if (m_isMove)
	{
		useHandle = m_engineHandleMove;
	}


	// エンジン部分の画像を描画
	if (m_engineHandleIdle != -1)
	{
		DrawRectRotaGraph(static_cast<int>(m_pos.x - kPlayerImageWidth / kEngineOffsetX), static_cast<int>(m_pos.y - kPlayerImageHeight / kEngineOffsetY),
			animeIndex * kPlayerImageWidth, 0, kPlayerImageWidth, kPlayerImageHeight, kPlayerScale, kPlayerAngle,
			useHandle, true, true, false);
	}
}

void player::DrawShot()
{
	// 弾の描画処理
	for(int i=0;i<=MaxShot;i++)
	{
		if (m_isShot[i]==true)
		{
			DrawCircle(m_shotPos[i].x, m_shotPos[i].y, kShotRadius, kShotColor, FALSE); // 黄色い円を描画
			// 弾の画像を描画
			DrawRectRotaGraph(static_cast<int>(m_shotPos[i].x), static_cast<int>(m_shotPos[i].y),
				0, 0, kShotImageWidth, kShotImageHeight, kScale, 0.0f,
				m_shotHandle[i], true, true, false);
		}
	}
}

void player::DrawBarrier()
{
	// バリアの使用回数を視認できるように
	if (m_barrierCount > 0)
	{
		DrawFormatString(10, 50, GetColor(255, 255, 255), "バリアは残り%d回まで使用可能", m_barrierCount);
	}

	// バリアが使用できない場合の表示
	if(m_barrierCount==0&&m_isBarrierHit==false)
	{
		DrawFormatString(10, 50, GetColor(255, 255, 255), "バリアを使い切りました。バリアを使用できません");
	}

	// バリアが攻撃を防いだ場合の表示
	if (m_isBarrierHit)
	{
		DrawFormatString(10, 50, GetColor(255, 255, 255), "バリアで攻撃を防ぎました。バリアを使用できません。");
	}

	// バリアを使用中の表示処理
	if(m_isBarrier&&m_isBarrierHit==false)
	{
		DrawFormatString(10, 70, GetColor(255, 255, 255), "バリア展開");
	}


	// バリアの描画処理
	if (m_isBarrier)
	{
		// バリアの点滅処理
		if (((int)m_barrierBlinkFrameCount / (int)half) % 2 == 1)
		{
			return;
		}
		// バリアの画像を描画
		if (m_barrierHandle != -1)
		{
			DrawRectRotaGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
				0, 0, kBarrierSize, kBarrierSize, 1.5, 0.0f,
				m_barrierHandle, TRUE);
		}
		// バリアの当たり判定を描画
		DrawCircle(m_pos.x, m_pos.y, kBarrierRadius, 0x00ffff, FALSE); // 水色の円を描画
	}
}

void player::UpdateDead()
{
	// 発射されている弾はすべてリセット
	for (int i = 0; i <= MaxShot; i++)
	{
		if (m_isShot[i] == true)
		{
			m_shotPos[i] = m_pos; // プレイヤーの位置に戻す
			m_isShot[i] = false; // 弾が撃たれていない状態にする
		}
	}

	// プレイヤーの移動処理
	Vec2 move(0.0f, 0.0f);
	move.x -= kSpeed; // 左に移動
	move.y += kSpeed; // 下に移動
	// 移動ベクトルを正規化
	if (move.Length() > 0.0f)
	{
		move.Normalize();
	}
	move.x *= kEngineBoost; // 瞬間移動の距離を増やす
	move.y *= kEngineBoost; // 瞬間移動の距離を増やす

	// プレイヤーの位置を更新
	m_pos += move;
	if (m_pos.x < -kPlayerImageWidth || m_pos.y > Game::kScreenHeight + kPlayerImageHeight)
	{
		// 画面外に出たら死亡状態をtrueにする
		m_isDead = true;
	}
}

void player::DrawDead()
{
	// プレイヤーが死亡したことを表示
	DrawFormatString(10, 50, GetColor(255, 255, 255), "撃墜されてしまいました");
	// 爆発のアニメーション処理
	int animeIndex = m_explosionAnimFrame / kExplosionAnimeNum;
	// プレイヤーの位置に爆発ハンドルを描画
	if (m_explosionHandle != -1)
	{
		DrawRectRotaGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
			animeIndex * kExplosionSize, 0, kExplosionSize, kExplosionSize, kPlayerScale, 0.0, m_explosionHandle, TRUE);
	}
	
}


