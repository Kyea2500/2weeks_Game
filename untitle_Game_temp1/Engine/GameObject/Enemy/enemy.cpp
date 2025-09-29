#include "enemy.h"
#include "../../../プロジェクトに追加すべきファイル_VC用/DxLib.h"
#include "../../InputDevice/game.h"
namespace
	{
	// 敵の初期座標
	constexpr float kInitialPosX = 100.0f;
	constexpr float kInitialPosY = 100.0f;
	// 敵の移動速度
	constexpr float kEnemySpeed = 5.0f;
	// 敵の半径
	constexpr int kEnemyRadius = 100;
	// 敵の色
	constexpr int kEnemyColor = 0x2525ff; // 青色

	constexpr float half = 2.0f;

	// 敵の横縦比
	constexpr int kEnemyGraphWidth = 120;
	constexpr int kEnemyGraphHeight = 120;

	// 雑魚敵の横縦比
	constexpr int kEnemyUnitGraphWidth = 64;
	constexpr int kEnemyUnitGraphHeight = 64;

	// 小さいタイプの雑魚敵の横縦比
	constexpr int kEnemyMiniUnitGraphWidth = 32;
	constexpr int kEnemyMiniUnitGraphHeight = 32;

	// 敵の拡大率
	constexpr double  kEnemyScale = 2.0;

	// 敵の体力
	constexpr int kEnemyLife = 30;
	// ダメージを受けたときの点滅フレーム数
	constexpr int kDamageBlinkFrame = 60;
 
	// 雑魚敵の数はそれぞれ異なる
	constexpr int kEnrmyUnitMax = 2;
	constexpr int kEnemyMiniUnitMax = 3;

	// 弾の最大数
	constexpr int kEnemyBulletMax = 20;

	// 爆発のアニメーションのコマ数
	constexpr int kExplosionAnimeNum = 8;
	constexpr int kExplosionFrameSpeed = 8;
	// 爆発のサイズ
	constexpr int kExplosionSize = 32;




}
enemy::enemy() :m_pos(0.0f,0.0f),
	m_speed(kEnemySpeed),
	m_enemyHandle(-1),
	m_Life(kEnemyLife),
	m_blinkFrameCount(0),
	m_isEnemyUnitReleased(false),
	m_enemyBulletSpeed(10),
	m_enemyBulletTimer(0),
	m_Dead(false),
	m_isDeadUpOnce(false)
{
	for (int i = 0; i <= kEnrmyUnitMax; i++)
	{
		// 雑魚敵の画像ハンドルを初期化
		m_enemyUnitHandle[i] = -1;
		m_enemyUnitPos[i] = Vec2(0.0f, 0.0f);
	}

	for (int i = 0; i <= kEnemyMiniUnitMax; i++)
	{
		// 小さいタイプの雑魚敵の画像ハンドルを初期化
		m_enemyMiniUnitHandle[i] = -1;
		m_enemyMiniUnitPos[i] = Vec2(0.0f, 0.0f);
	}
}

enemy::~enemy()
{
}

void enemy::Init()
{
	// 敵の初期座標を設定
	m_pos.x = Game::kScreenWidth - kInitialPosX; // 画面端に配置
	m_pos.y = Game::kScreenHeight / half; // 画面端に配置
	// 敵の移動速度を設定
	m_speed = kEnemySpeed;
	// 敵の体力を設定
	m_Life = kEnemyLife;

	// 敵の画像を読み込む
	m_enemyHandle = LoadGraph("Date/Enemy/Boss_ship.png");

	// 雑魚敵の画像を読み込む
	for (int i = 0; i <= kEnrmyUnitMax; i++)
	{
		m_enemyUnitHandle[i] = LoadGraph("Date/Enemy/Enemy_Ship.png");
		m_enemyUnitPos[i] = Vec2(0.0f,0.0f);
	}
	// 小さいタイプの雑魚敵の画像を読み込む
	for (int i = 0; i <= kEnemyMiniUnitMax; i++)
	{
		m_enemyMiniUnitHandle[i] = LoadGraph("Date/Enemy/Enemy_mineShip");
		m_enemyMiniUnitPos[i] = Vec2(0.0f, 0.0f);
	}
	
}

void enemy::End()
{
	// 敵の画像を解放
	if (m_enemyHandle != -1)
	{
		m_enemyHandle = -1;
		DeleteGraph(m_enemyHandle);
	}

	// 雑魚敵の画像を解放
	for (int i = 0; i <= kEnrmyUnitMax; i++)
	{
		if (m_enemyUnitHandle[i] != -1)
		{
			m_enemyUnitHandle[i] = -1;
			DeleteGraph(m_enemyUnitHandle[i]);
		}
	}
	for (int i = 0; i <= kEnemyMiniUnitMax; i++)
	{
		if (m_enemyMiniUnitHandle[i] != -1)
		{
			m_enemyMiniUnitHandle[i] = -1;
			DeleteGraph(m_enemyMiniUnitHandle[i]);
		}
	}
}

void enemy::Update()
{
	if (m_Life > 0)
	{
		UpdateEnemy();
		UpdateEnemyUnit();
		UpdateEnemyMiniUnit();
		UpdateEnemyBullet();
	}

	// 敵の体力が0以下なら倒されたことにする
	if (m_Life <= 0)
	{
		// 爆発エフェクトのアニメーションフレームの更新
		m_explosionAnimFrame++;
		int explosionTotalAnimeFrame = kExplosionFrameSpeed * kExplosionAnimeNum;
		UpdateDead();
	}
}

void enemy::Draw()
{
	DrawEnemy();
	DrawEnemyUnit();
	DrawEnemyMiniUnit();
	DrawEnemyBullet();
}

// 敵(Boss)の当たり判定の実装
float enemy::EnemyGetPosX()
{
	return m_pos.x - kEnemyGraphWidth / half;
}

float enemy::EnemyGetPosY()
{
	return m_pos.y - kEnemyGraphHeight / half;
}

float enemy::GetEnemyRadius()
{
	return kEnemyRadius;
}

// 雑魚敵の当たり判定
float enemy::EnemyUnitGetPosX()
{
	return 0.0f;
}

float enemy::EnemyUnitGetPosY()
{
	return 0.0f;
}

float enemy::GetEnemyUnitRadius()
{
	return 0.0f;
}

// 小さいタイプの雑魚敵の当たり判定
float enemy::EnemyMiniUnitGetPosX()
{
	return 0.0f;
}

float enemy::EnemyMiniUnitGetPosY()
{
	return 0.0f;
}

float enemy::GetEnemyMiniUnitRadius()
{
	return 0.0f;
}

// 敵がダメージを受けたときの処理
void enemy::EnemysDamage()
{
	if (m_blinkFrameCount > 0) return;
	// 無敵時間(点滅する時間)を設定する
	m_blinkFrameCount = kDamageBlinkFrame;
	m_Life--;
	// 小さいタイプの雑魚敵を放つ
	UpdateEnemyMiniUnit();

	if (m_Life <= 0)
	{
		m_Life = 0;
		// ここに敵が倒されたときの処理を追加
	}
}

// 雑魚敵がダメージを受けたときの処理(共通)
void enemy::EnemyUnitsDamage()
{
}

// 敵(Boss)の更新処理
void enemy::UpdateEnemy()
{
	// 無敵時間の更新
	m_blinkFrameCount--;
	if (m_blinkFrameCount < 0)
	{
		m_blinkFrameCount = 0;
	}

	// 敵は上下に移動する
	m_pos.y += m_speed;
	// 画面の上端に到達したら下に移動
	if (m_pos.y > Game::kScreenHeight - kEnemyRadius)
	{
		m_speed = -kEnemySpeed; // 速度を反転して上に移動
	}
	// 画面の下端に到達したら上に移動
	if (m_pos.y < kEnemyRadius)
	{
		m_speed = kEnemySpeed; // 速度を反転して下に移動
	}
}

// 雑魚敵の更新処理
void enemy::UpdateEnemyUnit()
{
	// Bossの放つ雑魚敵の一つ
	// 体力が半分以下になった時のみ放たれる
	if (m_Life > kEnemyLife / half)return;
	// 一度でも放たれた場合、こいつらは出現しなくなる
	if( m_isEnemyUnitReleased == false)
	{
		m_isEnemyUnitReleased = true;
	}
	else
	{
		return;
	}

	// 雑魚敵は2体いるが、小さい敵と異なり、ランダムな画面外から放たれる
	for (int i = 0; i <= kEnrmyUnitMax; i++)
	{
		// 放たれるときはランダムな画面外から放たれる
		if (m_enemyUnitPos[i].x == 0.0f && m_enemyUnitPos[i].y == 0.0f)
		{
			m_enemyUnitPos[i].x = Game::kScreenWidth + kEnemyRadius; // 画面の右端から放たれる
			m_enemyUnitPos[i].y = static_cast<float>(GetRand(Game::kScreenHeight - kEnemyRadius)); // ランダムなY座標から放たれる
		}
		// 放たれた雑魚敵はBossの前まで左に移動し、Bossより左に行くとBossと同じように上下に移動し、弾を放つ
		m_enemyUnitPos[i].x -= m_speed;
		// Bossの前まで来たら上下に移動する
		if (m_enemyUnitPos[i].x < m_pos.x)
		{
			m_enemyUnitPos[i].x = m_pos.x- kEnemyUnitGraphWidth; // Bossの前で止まる


			// Bossと同じように上下に移動する
			m_enemyUnitPos[i].y -= m_speed;
			// 画面の上端に到達したら下に移動
			if (m_enemyUnitPos[i].y > Game::kScreenHeight - kEnemyRadius)
			{
				m_speed = -kEnemySpeed; // 速度を反転して上に移動
			}
			// 画面の下端に到達したら上に移動
			if (m_enemyUnitPos[i].y < kEnemyRadius)
			{
				m_speed = kEnemySpeed; // 速度を反転して下に移動
			}

			// Bossと同じように移動しだすと弾を放つ
			// ここに弾を放つ処理を追加
			UpdateEnemyBullet();
		}
	}
}

// 小さいタイプの雑魚敵の更新処理
void enemy::UpdateEnemyMiniUnit()
{
	//Bossの放つ小さな敵
	// 小さいタイプの雑魚敵は3体いる
	for (int i = 0; i <= kEnemyMiniUnitMax; i++)
	{
		// 放たれるときはBossの中央から1体ずつずらして放たれる
		if (m_enemyMiniUnitPos[i].x == 0.0f && m_enemyMiniUnitPos[i].y == 0.0f)
		{
			m_enemyMiniUnitPos[i].x = m_pos.x - (i * kEnemyRadius); // Bossの中央から1体ずつずらして放たれる
			m_enemyMiniUnitPos[i].y = m_pos.y + (i * kEnemyRadius); // Bossの中央から1体ずつずらして放たれる
		}

		// 放たれた小さいタイプの雑魚敵は左に移動する
		m_enemyMiniUnitPos[i].x -= m_speed;
		// 画面外に出たら位置をリセット
		if (m_enemyMiniUnitPos[i].x < -kEnemyRadius)
		{
			m_enemyMiniUnitPos[i] = Vec2(0.0f, 0.0f); // 位置をリセット
		}

	}
}

// 敵(Boss)の描画処理
void enemy::DrawEnemy()
{
	// 体力を視認できるように
	DrawFormatString(10, 30, GetColor(255, 255, 255), "Enemy Life:%d", m_Life);

	// 点滅処理
	if (((int)m_blinkFrameCount / (int)half) % 2 == 1)
	{
		return;
	}
	// 円の中心に描画
	DrawRectRotaGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
		0, 0, kEnemyGraphWidth, kEnemyGraphHeight, kEnemyScale, 0.0, m_enemyHandle, TRUE);
	// 敵の描画処理
	DrawCircle(m_pos.x, m_pos.y, kEnemyRadius, kEnemyColor, FALSE); // 青い円を描画
}

void enemy::DrawEnemyUnit()
{
	// 雑魚敵の描画処理
	for (int i = 0; i <= kEnrmyUnitMax; i++)
	{
		if (m_enemyUnitPos[i].x != 0.0f && m_enemyUnitPos[i].y != 0.0f)
		{
			// 円の中心に描画
			DrawRectRotaGraph(static_cast<int>(m_enemyUnitPos[i].x), static_cast<int>(m_enemyUnitPos[i].y),
				0, 0, kEnemyUnitGraphWidth, kEnemyUnitGraphHeight, kEnemyScale, 0.0, m_enemyUnitHandle[i], TRUE);
			// 雑魚敵の描画処理
			DrawCircle(m_enemyUnitPos[i].x, m_enemyUnitPos[i].y, kEnemyRadius / half,0xffff25, FALSE);//黄色い円で描画
		}
	}
}

void enemy::DrawEnemyMiniUnit()
{
	// 小さいタイプの雑魚敵の描画処理
	for (int i = 0; i <= kEnemyMiniUnitMax; i++)
	{
			// 円の中心に描画
			DrawRectRotaGraph(static_cast<int>(m_enemyMiniUnitPos[i].x), static_cast<int>(m_enemyMiniUnitPos[i].y),
				0, 0, kEnemyMiniUnitGraphWidth, kEnemyMiniUnitGraphHeight, kEnemyScale, 0.0, m_enemyMiniUnitHandle[i], TRUE);
			// 小さいタイプの雑魚敵の描画処理
			DrawCircle(m_enemyMiniUnitPos[i].x, m_enemyMiniUnitPos[i].y, kEnemyRadius / 4,0xff0000, FALSE); // 青い円を描画
	}
}

void enemy::UpdateEnemyBullet()
{
	// 時間がたつと放たれる
	if (m_enemyBulletTimer > 0)
	{
		// プレイヤーの射撃処理
		for (int i = 0; i <= kEnemyBulletMax; i++)
		{
			m_isEnemyBullet[i] = true;
			m_enemyBulletPos[i] = Vec2(m_enemyUnitPos[i]); // 弾の位置をプレイヤーの位置に設定
			break; // 1発だけ撃つ
		}

		// 弾の更新処理
		for (int i = 0; i <= kEnemyBulletMax; i++)
		{
			if (m_isEnemyBullet[i] == true)
			{
				// 弾の位置を更新
				m_enemyBulletPos[i].x -= 15.0f; // 右方向に移動
				// 画面外に出たら弾をリセット
				if (m_enemyBulletPos[i].x > Game::kScreenWidth)
				{
					m_enemyBulletPos[i] = m_pos; // プレイヤーの位置に戻す
					m_isEnemyBullet[i] = false; // 弾が撃たれていない状態にする

				}
			}
		}
	}

}

void enemy::DrawEnemyBullet()
{
}

void enemy::UpdateDead()
{
	// 敵は倒されたとしても、倒されていない状態の時と同じく、上下に移動するが
	// 一度だけ上に行った後に下に行き、画面外へ消える
	m_pos.y += m_speed;
	if (m_isDeadUpOnce == false)
	{
		// 画面の上端に到達したら下に移動
		if (m_pos.y > Game::kScreenHeight - kEnemyRadius)
		{
			m_speed = -kEnemySpeed; // 速度を反転して上に移動
		}
		
		if (m_pos.y < kEnemyRadius)
		{
			m_speed = kEnemySpeed; // 速度を反転して下に移動
			m_isDeadUpOnce = true; // 一度上に行ったことを記録
		}
	}
	if (m_isDeadUpOnce == true)
	{
		// 画面の上側に到達したら下に移動
		if (m_pos.y < kEnemyRadius)
		{
			m_speed = kEnemySpeed; // 速度を反転して下に移動
		}

		// 画面の下端に到達したら敵は死ぬ
		if (m_pos.y > Game::kScreenHeight + kEnemyRadius)
		{
			m_Dead = true; // 敵が死んだことを記録
		}
	}

}
void enemy::DrawDead()
{
	if (m_Life <= 0)
	{
		int animeIndex = m_explosionAnimFrame / kExplosionAnimeNum;

		if (m_explosionHandle != -1)
		{
			DrawRectRotaGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
				animeIndex * kExplosionSize, 0, kExplosionSize, kExplosionSize, kEnemyScale, 0.0, m_explosionHandle, TRUE);
		}
	}

}
