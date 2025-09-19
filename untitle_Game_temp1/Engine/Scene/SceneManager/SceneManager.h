#pragma once
#include <memory>
class SceneTitle;
class SceneMain;
class SceneGameOver;
class SceneManager
{
public:
	SceneManager();
	~SceneManager();
	void Init();
	void End();
	void Update();
	void Draw();
public:
	enum SceneKind
	{
		kSceneTitle,
		kSceneMain,
		kSceneGameOver,
		kSceneGameClear,
		kSceneNum
	}; private:
		SceneKind m_Kind;
		// 各シーンのポインタ
		SceneTitle* m_pSceneTitle;
		SceneMain* m_pSceneMain;
		SceneGameOver* m_pSceneGameOver;

};

