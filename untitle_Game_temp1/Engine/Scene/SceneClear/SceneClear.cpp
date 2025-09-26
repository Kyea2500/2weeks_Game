#include "SceneClear.h"
#include "DxLib.h"
#include "../../InputDevice/game.h"
#include "../../InputDevice/Pad/Pad.h"

SceneClear::SceneClear()
{
}

SceneClear::~SceneClear()
{
}
void SceneClear::Init()
{
}
void SceneClear::End()
{
}
SceneManager::SceneKind SceneClear::Update()
{
	// 入力状態の更新
	Pad::Update();
	if (Pad::IsTrigger(PAD_INPUT_1))
	{
		// タイトルへ
		return SceneManager::SceneKind::kSceneTitle;
	}
	if (Pad::IsTrigger(PAD_INPUT_2))
	{
		// 終了
		PostQuitMessage(0);
	}
	// ゲームクリアシーンの更新処理
	return SceneManager::SceneKind::kSceneGameClear;
}
void SceneClear::Draw()
{
	// ゲームクリア画面の描画
	DrawString(250, 200, "Game Clear", GetColor(0, 255, 0));
	DrawString(200, 300, "Press 1 to Title", GetColor(255, 255, 255));
	DrawString(200, 350, "Press 2 to Exit", GetColor(255, 255, 255));
}
