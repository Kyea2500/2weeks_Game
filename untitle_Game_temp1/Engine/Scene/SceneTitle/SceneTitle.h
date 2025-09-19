#pragma once
#include"../../InputDevice/Vec2.h"
#include "../../Scene/SceneManager/SceneManager.h"
class SceneTitle
{
public:
	SceneTitle();
	~SceneTitle();
	void Init();
	void End();
	SceneManager::SceneKind Update();
	void Draw();
private:
	Vec2 m_textpos;
	float m_textVecX;
	float m_textVecY;
};