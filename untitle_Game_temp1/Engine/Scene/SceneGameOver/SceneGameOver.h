#pragma once
#include "../../Scene/SceneManager/SceneManager.h"
class SceneGameOver
{
public:
	SceneGameOver();
	~SceneGameOver();
	void Init();
	void End();
	SceneManager::SceneKind Update();
	void Draw();
private:

};

