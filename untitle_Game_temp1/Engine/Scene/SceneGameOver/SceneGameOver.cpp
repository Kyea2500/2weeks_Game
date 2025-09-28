#include "SceneGameOver.h"
#include "../../�v���W�F�N�g�ɒǉ����ׂ��t�@�C��_VC�p/DxLib.h"
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
	// ���͏�Ԃ̍X�V
	Pad::Update();
	
	if (Pad::IsPress(PAD_INPUT_1))
	{
		// ������
		return SceneManager::SceneKind::kSceneMain;
	}
	if (Pad::IsPress(PAD_INPUT_2))
	{
		// �^�C�g����
		return SceneManager::SceneKind::kSceneTitle;
	}
	if (Pad::IsPress(PAD_INPUT_3))
	{
		// �I��
		PostQuitMessage(0);
	}

	// �Q�[���I�[�o�[�V�[���̍X�V����
	return SceneManager::SceneKind::kSceneGameOver;
}

void SceneGameOver::Draw()
{
	// �Q�[���I�[�o�[��ʂ̕`��
	DrawString(250, 200, "Game Over", GetColor(255, 0, 0));
	DrawString(200, 300, "Press 1 to Continue", GetColor(255, 255, 255));
	DrawString(200, 350, "Press 2 to Title", GetColor(255, 255, 255));
	DrawString(200, 400, "Press 3 to Exit", GetColor(255, 255, 255));
}