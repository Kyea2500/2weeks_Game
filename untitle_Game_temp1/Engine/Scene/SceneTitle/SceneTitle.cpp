#include "SceneTitle.h"
#include "DxLib.h"
#include "../../InputDevice/Pad/Pad.h"

namespace
{
		constexpr float kMoveVec = 6.0f;
		constexpr float kMaxPosX = 1200.0f;
		constexpr float kMaxPosY = 630.0f;

		constexpr  int kColor = 0xffff00;
}

SceneTitle::SceneTitle() :m_textpos(0, 0), 
m_textVecX(kMoveVec),
m_textVecY(kMoveVec)
{
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Init()
{

	m_textpos.x = 0.0f;
	m_textpos.y = 0.0f;
}

void SceneTitle::End()
{
}

SceneManager::SceneKind SceneTitle::Update()
{
	m_textpos.x += m_textVecX;
	m_textpos.y += m_textVecY;

	// 画面外に出ないようにする(X軸)
	if (m_textpos.x < 0.0f)
	{
		m_textpos.x = 0.0f;
		m_textVecX = kMoveVec;
	}
	if (m_textpos.x > kMaxPosX)
	{
		m_textpos.x = kMaxPosX;
		m_textVecX = -kMoveVec;
	}

	// 画面外に出ないようにする(Y軸)
	if (m_textpos.y < 0.0f)
	{
		m_textpos.y = 0.0f;
		m_textVecY = kMoveVec;
	}
	if (m_textpos.y > kMaxPosY)
	{
		m_textpos.y = kMaxPosY;
		m_textVecY = -kMoveVec;
	}

	// タイトルシーンの更新処理
	if(Pad::IsTrigger(PAD_INPUT_1))
	{
		// ボタンが押されたらメインシーンへ遷移
		return SceneManager::SceneKind::kSceneMain;
	}
	return SceneManager::SceneKind::kSceneTitle;
}


void SceneTitle::Draw()
{
	DrawString(m_textpos.x, m_textpos.y, "Title Scene", kColor); // タイトルテキストを描画
}
