#pragma once
#include "../../Scene/SceneManager/SceneManager.h"
class SceneClear
{
	public:
	SceneClear();
	~SceneClear();
	void Init();
	void End();
	SceneManager::SceneKind Update();
	void Draw();
};

