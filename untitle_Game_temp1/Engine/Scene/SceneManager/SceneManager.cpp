#include "../SceneManager/SceneManager.h"
#include "../SceneTitle/SceneTitle.h"
#include "../SceneMain/SceneMain.h"
#include "../SceneGameOver/SceneGameOver.h"
#include "../SceneClear/SceneClear.h"
#include "../../InputDevice/Pad/Pad.h"

SceneManager::SceneManager() :
	m_Kind(kSceneTitle), // 初期シーンをタイトルに設定
	m_pSceneTitle(nullptr), // タイトルシーンのポインタを初期化
	m_pSceneMain(nullptr),  // メインシーンのポインタを初期化
	m_pSceneGameOver(nullptr), // ゲームオーバーシーンのポインタを初期化
	m_pSceneClear(nullptr)
{
}

SceneManager::~SceneManager()
{
	// 各シーンのポインタを解放
	if (m_pSceneTitle != nullptr)
	{
		m_pSceneTitle = nullptr;
		delete m_pSceneTitle;
	}
	else if (m_pSceneMain != nullptr)
	{
		m_pSceneMain = nullptr;
		delete m_pSceneMain;
	}
	else if (m_pSceneGameOver != nullptr)
	{
		m_pSceneGameOver = nullptr;
		delete m_pSceneGameOver;
	}
	else if (m_pSceneClear != nullptr)
	{
		m_pSceneClear = nullptr;
		delete m_pSceneClear;
	}
}

void SceneManager::Init()
{
	// シーンの初期化
	switch (m_Kind)
	{
	case SceneKind::kSceneTitle:
		m_pSceneTitle = new SceneTitle();
		m_pSceneTitle->Init();
		break;
	case SceneKind::kSceneMain:
		m_pSceneMain = new SceneMain();
		m_pSceneMain->Init();
		break;
	case SceneKind::kSceneGameOver:
		m_pSceneGameOver = new SceneGameOver();
		m_pSceneGameOver->Init();
		break;
	case SceneKind::kSceneGameClear:
		m_pSceneClear = new SceneClear();
		m_pSceneClear->Init();
		break;
	default:
		break;
	}
}

void SceneManager::End()
{// シーンの終了処理
	switch (m_Kind)
	{
	case SceneKind::kSceneTitle:
		m_pSceneTitle->End();
		delete m_pSceneTitle;
		m_pSceneTitle = nullptr;
		break;
	case SceneKind::kSceneMain:
		m_pSceneMain->End();
		delete m_pSceneMain;
		m_pSceneMain = nullptr;
		break;
	case SceneKind::kSceneGameOver:
		m_pSceneGameOver->End();
		delete m_pSceneGameOver;
		m_pSceneGameOver = nullptr;
		break;
	case SceneKind::kSceneGameClear:
		m_pSceneClear->End();
		delete m_pSceneClear;
		m_pSceneClear = nullptr;
		break;
	default:
		break;
	}
}

void SceneManager::Update()
{
	Pad::Update();
	SceneKind nextKind = m_Kind;
	// シーンの更新
	switch (m_Kind)
	{
	case SceneKind::kSceneTitle:
		nextKind=m_pSceneTitle->Update();
		break;
	case SceneKind::kSceneMain:
		nextKind = m_pSceneMain->Update();
		break;
	case SceneKind::kSceneGameOver:
		nextKind = m_pSceneGameOver->Update();
		break;
	case SceneKind::kSceneGameClear:
		nextKind = m_pSceneClear->Update();
		break;
	default:
		break;
	}

	if (nextKind != m_Kind)
	{
		// 現在実行中のシーン(m_kind)の終了処理
		End();
		// 次のシーンに切り替え
		m_Kind = nextKind;
		// 切り替え後のシーンの初期化
		Init();
	}
}


void SceneManager::Draw()
{
	// シーンの描画
	switch (m_Kind)
	{
	case SceneKind::kSceneTitle:
		m_pSceneTitle->Draw();
		break;
	case SceneKind::kSceneMain:
		m_pSceneMain->Draw();
		break;
	case SceneKind::kSceneGameOver:
		m_pSceneGameOver->Draw();
		break;
	case SceneKind::kSceneGameClear:
		m_pSceneClear->Draw();
		break;
	default:
		break;
	}
}
