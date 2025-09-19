#include "player.h"
#include "DxLib.h"
#include "../../InputDevice/game.h"
#include "../../InputDevice/Pad/Pad.h"

namespace
{
	// プレイヤーの初期座標
	constexpr float kInitialPosX = 320.0f;
	constexpr float kInitialPosY = 240.0f;
	// プレイヤーの移動速度
	constexpr float kPlayerSpeed = 1024.0f;

	// プレイヤーの半径
	constexpr int kPlayerRadius = 20;

	// プレイヤーの色
	constexpr int kPlayerColor = 0xff2525; // 赤色

	const int kPlayerDamage1 = LoadGraph("Date/Player/Player_Damage.png");
	const int kPlayerDamage2 = LoadGraph("Date/Player/Player_Damage2.png");
	const int kPlayerDamage3 = LoadGraph("Date/Player/Player_Damage3.png");

	// エンジン部分のアニメーションのコマ数
	constexpr int kEngineIdleAnimeNum = 2;
	constexpr int kEngineMoveAnimeNum = 3;

	// アニメーションフレーム速度
	constexpr int kAnimeFrameSpeed = 5;

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

}

player::player()
	: m_pos(0.0f, 0.0f), // プレイヤーの初期座標を設定
	speed(kPlayerSpeed), // プレイヤーの移動速度を設定
	m_playerHandle(-1), // プレイヤーの画像ハンドルを初期化
	m_engineHandleIdle(-1), // エンジン部分の画像ハンドルを初期化
	m_engineHandleMove(-1), // エンジン部分の移動中画像ハンドルを初期化
	m_animFrame(0), // アニメーションフレームを初期化
	m_isMove(false), // プレイヤーの移動状態を初期化
	m_isLastShot(false), // 最後に撃った弾がどれかを初期化
	m_shotTimer(0) // プレイヤーの射撃タイマーを初期化
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
	// エンジン部分の画像を読み込む
	m_engineHandleIdle = LoadGraph("Date/Effects/Player_IdleEngine.png");
	m_engineHandleMove = LoadGraph("Date/Effects/Player_MoveingEngine.png");

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

	// 入力状態の更新
	Pad::Update();
	
	m_animFrame++;
	int totalAnimeFrame = kAnimeFrameSpeed * kEngineIdleAnimeNum;
	if (m_isMove)
	{
		totalAnimeFrame = kAnimeFrameSpeed * kEngineMoveAnimeNum;
	}

	// アニメーションフレームをループさせる
	if(m_animFrame >= totalAnimeFrame)
	{
		m_animFrame = 0;
	}

	// プレイヤーの更新処理
	UpdateMove();
	// プレイヤーの射撃処理
	m_shotTimer++;
	// ゲームスタートと同時に弾が出ないようにタイマーを置く
	if (m_shotTimer>= kShotTimerMax)
	{
		UpdateShot();
		m_shotTimer = kShotTimerMax; // タイマーが増えすぎないように抑制
	}
}

void player::Draw()
{
	DrawPlayer();
	DrawEngine();
	DrawShot();
}

float player::PlayerGetPosX()
{
	return m_pos.x;
}

float player::PlayerGetPosY()
{
	return m_pos.y;
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
			return m_shotPos[i].x;
		}
	}
}

float player::ShotGetPosY()
{
	for(int i=0;i<=MaxShot;i++)
	{
		if (m_isShot[i]==true)
		{
			return m_shotPos[i].y;
		}
	}
}

float player::GetShotRadius()
{
	for(int i=0;i<=MaxShot;i++)
	{
		if (m_isShot[i]==true)
		{
			return kShotRadius;
		}
	}
}


void player::UpdateMove()
{
	// プレイヤーの移動処理
	// 移動ベクトルを初期化
	Vec2 move(0.0f, 0.0f);
	m_isMove = false;

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

	// 弾の連射を

	
}

void player::DrawPlayer()
{
	// 描画処理
	// プレイヤーの描画処理
	DrawCircle(m_pos.x, m_pos.y, kPlayerRadius, kPlayerColor, FALSE); // 赤い円を描画

	// プレイヤーの画像を描画
	if (m_playerHandle != -1)
	{
		// 円の中心に描画
		DrawRotaGraph(m_pos.x, m_pos.y, kPlayerScale, kPlayerAngle, m_playerHandle, TRUE);
	}
}

void player::DrawEngine()
{
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
