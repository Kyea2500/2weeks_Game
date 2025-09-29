#include "SceneClear.h"
#include "../../../�v���W�F�N�g�ɒǉ����ׂ��t�@�C��_VC�p/DxLib.h"
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
	// ���͏�Ԃ̍X�V
	Pad::Update();
	if (Pad::IsPress(PAD_INPUT_1))
	{
		// �^�C�g����
		return SceneManager::SceneKind::kSceneTitle;
	}
	if (Pad::IsPress(PAD_INPUT_2))
	{
		// �I��
		PostQuitMessage(0);
	}

	// �Ȃ������܂��ڍs���ł��Ȃ�
	// �Q�[���N���A�V�[���̍X�V����
	return SceneManager::SceneKind::kSceneGameClear;
}
void SceneClear::Draw()
{
	// �Q�[���N���A��ʂ̕`��
	DrawString(250, 200, "Game Clear", GetColor(0, 255, 0));
	DrawString(200, 300, "Press 1 to Title", GetColor(255, 255, 255));
	DrawString(200, 350, "Press 2 to Exit", GetColor(255, 255, 255));
}
