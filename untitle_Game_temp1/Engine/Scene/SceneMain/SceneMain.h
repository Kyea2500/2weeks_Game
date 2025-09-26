#pragma once
#include "../../InputDevice/Pad/Pad.h"
#include"../SceneManager/SceneManager.h"
#include <memory>
class player;
class enemy;
class Bg;
class SceneMain
{
public:
	SceneMain();
	~SceneMain();
	void Init();
	void End();
	SceneManager::SceneKind Update();
	void Draw();

	void HitPlayerShot();
	void HitPlayerEnemy();
private:
	std::shared_ptr<player> m_pPlayer;
	std::shared_ptr<enemy> m_pEnemy;
	std::shared_ptr<Bg> m_pBg;
};
