#include "SceneGameOver.h"
#include "../../プロジェクトに追加すべきファイル_VC用/DxLib.h"
#include "../../InputDevice/game.h"
#include "../../InputDevice/Pad/Pad.h"

SceneGameOver::SceneGameOver()
{
}

SceneGameOver::~SceneGameOver()
{
}

void SceneGameOver::Init()
{
}

void SceneGameOver::End()
{
}

SceneManager::SceneKind SceneGameOver::Update()
{
	// 入力状態の更新
	Pad::Update();
	
	if (Pad::IsPress(PAD_INPUT_1))
	{
		// 続ける
		return SceneManager::SceneKind::kSceneMain;
	}
	if (Pad::IsPress(PAD_INPUT_2))
	{
		// タイトルへ
		return SceneManager::SceneKind::kSceneTitle;
	}
	if (Pad::IsPress(PAD_INPUT_3))
	{
		// 終了
		PostQuitMessage(0);
	}

	// ゲームオーバーシーンの更新処理
	return SceneManager::SceneKind::kSceneGameOver;
}

void SceneGameOver::Draw()
{
	// ゲームオーバー画面の描画
	DrawString(250, 200, "Game Over", GetColor(255, 0, 0));
	DrawString(200, 300, "Press 1 to Continue", GetColor(255, 255, 255));
	DrawString(200, 350, "Press 2 to Title", GetColor(255, 255, 255));
	DrawString(200, 400, "Press 3 to Exit", GetColor(255, 255, 255));
}